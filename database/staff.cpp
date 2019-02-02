#include "staff.h"

// Qt headers
#include <algorithm>
#include <QCoreApplication>
#include <QDebug>
#include <QLocale>
#include <QProgressDialog>

// Application headers
#include "name.h"

// --- Static data --- //
QVector<Staff> Staff::db;
DuplicateManager *Staff::s_Duplicates = nullptr;

/* ==================== */
/*      Staff Data      */
/* ==================== */

// --- Default constructor --- //
Staff::Staff()
{
}


/* =================== */
/*      Duplicates     */
/* =================== */

// --- Find duplicate staff --- //
void Staff::findDuplicateStaff()
{
    // New line / separator
    qDebug() << "\n";

    // Duplicate Manager
    if(s_Duplicates != nullptr)
        delete s_Duplicates;
    s_Duplicates = new DuplicateManager();

    // Find possible duplicates (pass 1 and pass 2)
    findDuplicateStaff_Standard_Pass1();
    findDuplicateStaff_Standard_Pass2();
    findDuplicateStaff_SimilarStrings();

    // Restore the original ordering of the table
    std::sort(db.begin(), db.end(), Staff::sortStandard);

    // Check whether any potential duplicate staff are duplicates and flag them
    checkDuplicateStaff();

    // Save the spreadsheet
    s_Duplicates->writeData(false);

    // Tidy up
    delete s_Duplicates;
}

// --- Check whether any potential duplicate staff are actually duplicates --- //
void Staff::checkDuplicateStaff()
{
    const qint32 size = s_Duplicates->size();
    const qint32 yobThreshold = 1;  // Maximum difference between years of birth

    QLocale locale;
    qDebug() << "\n";
    qDebug() << QString("%1 groups of potential duplicates found").arg(locale.toString(size));
    qDebug() << "Searching for actual duplicates...";
    QCoreApplication::processEvents();

    QProgressDialog progress("Searching for actual duplicates...", "Please wait", 0, size);
    progress.setWindowTitle("Searching for actual duplicates...");
    progress.setWindowModality(Qt::WindowModal);
    progress.show();

    for(qint32 group = 0; group < size; ++group)
    {
        const QVector<qint32> *list = s_Duplicates->getGroupIdList(group);
        const qint32 listSize = list->size();

        // Initial progress update: Current iteration
        progress.setLabelText(QString("Group %1: Checking %2 items").arg(locale.toString(group)).arg(locale.toString(listSize)));
        QCoreApplication::processEvents();

        for(qint32 a = 0; a < (listSize - 1); ++a)  // Person A
        {
            const qint16 yob = db[list->at(a)].YearOfBirth;
            const qint32 clubId = db[list->at(a)].ClubContracted;

            for(qint32 b = a+1; b < listSize; ++b)      // Person B
            {
                const qint32 yobDifference = yob - db[list->at(b)].YearOfBirth;

                if((yobDifference >= (0 - yobThreshold) && yobDifference <= yobThreshold) ||  // Year of birth
                   (clubId > -1 && clubId == db[list->at(b)].ClubContracted))   // Club contracted (non-free agents)
                {
                    s_Duplicates->flagDuplicates(group, a, b);
                }
            }
        }

        // Progress update: Increment the progress bar now that the iteration is complete
        progress.setValue(group);
    }

    progress.setValue(size);
    qDebug() << QString("%1 actual duplicate group(s) found.").arg(locale.toString(s_Duplicates->getActualDuplicateCount()));
}

// --- Find duplicate staff: Pass 1 (full name) --- //
void Staff::findDuplicateStaff_Standard_Pass1()
{
    qDebug() << "PASS 1: First and second name potential duplicates...";
    QCoreApplication::processEvents();

    const qint32 size = db.size();    // Table size
    qint32 a = 0;   // Iterator (Person A)
    qint32 b = 0;   // Sub-iterator (Person B)

    // Sort the table in order to make it easier to find duplicates
    std::sort(db.begin(), db.end(), Staff::sortFullName);

    while(a < (size - 1))   // Person A: Iterate up to and including the second to last person in the table
    {
        const qint64 target = db[a].m_FirstAndSecondNameUID;

        if(db[a].FirstNameId >= 0 && db[a].SecondNameId >= 0)   // Skip any persons with no first and second name
        {
            b = a+1;

            while(b < size)     // Person B: Iterate up to and including the last person in the table
            {
                if(target != db[b].m_FirstAndSecondNameUID)
                {
                    --b;
                    break;
                }
                else
                    ++b;
            }

            if(a < b) // Add potential duplicate group
            {
                s_Duplicates->addNewGroup();

                for(int i = a; i <= b; ++i)
                    s_Duplicates->addToGroup(db[i].ID);
            }
        }

        ++b;
        a = b;
    }

    QLocale locale;
    qDebug() << QString("PASS 1: %1 potential duplicates found").arg(locale.toString(s_Duplicates->size()));
}

// --- Find duplicate staff: Pass 2 (common name) --- //
void Staff::findDuplicateStaff_Standard_Pass2()
{
    qDebug() << "PASS 2: Common name potential duplicates...";
    QCoreApplication::processEvents();

    const qint32 initialDuplicateCount = s_Duplicates->size();
    const qint32 size = db.size();    // Table size
    qint32 a = 0;   // Iterator (Person A)
    qint32 b = 0;   // Sub-iterator (Person B)

    // Sort the table in order to make it easier to find duplicates
    std::sort(db.begin(), db.end(), Staff::sortCommonName);

    while(a < (size - 1))   // Person A: Iterate up to and including the second to last person in the table
    {
        const qint32 target = db[a].CommonNameId;

        if(target >= 0 &&
           Staff::getCommonName(a).size() > 0)   // Skip any persons with no common name (i.e. NONE or a blank Common Name text string)
        {
            b = a+1;

            while(b < size)     // Person B: Iterate up to and including the last person in the table
            {
                if(target != db[b].CommonNameId)
                {
                    --b;
                    break;
                }
                else
                    ++b;
            }

            if(a < b) // Add potential duplicate group
            {
                s_Duplicates->addNewGroup();

                for(int i = a; i <= b; ++i)
                    s_Duplicates->addToGroup(db[i].ID);
            }
        }

        ++b;
        a = b;
    }

    QLocale locale;
    qDebug() << QString("PASS 2: %1 potential duplicates found").arg(locale.toString(s_Duplicates->size() - initialDuplicateCount));
}

// --- Find duplicate staff: String similarity algorithm --- //
void Staff::findDuplicateStaff_SimilarStrings()
{
    const qint32 maximumDistance = 1;
    const qint32 size = db.size();    // Table size
    qint32 counter = 0;

    qDebug() << "PASS 3: First and second name string similarity duplicates...";
    QCoreApplication::processEvents();

    QProgressDialog progress("String similarity search...", "Please wait", 0, size);
    progress.setWindowTitle("String similarity search (%1% similarity threshold)...");
    progress.setWindowModality(Qt::WindowModal);
    progress.show();

    // Sort the table in order to make it easier to find duplicates
    std::sort(db.begin(), db.end(), Staff::sortFullName);

    // Iterate through each name
    for(qint32 a = 0; a < size - 1; ++a)
    {
        progress.setValue(a);
        QCoreApplication::processEvents();

        bool firstMatch = false;

        if(db[a].FirstNameId < 0 || db[a].SecondNameId < 0) // Ignore partial names
            continue;

        // Compare against each following name
        for (qint32 b = a + 1; b < size; ++b)
        {
            if(db[b].FirstNameId < 0 || db[b].SecondNameId < 0) // Ignore partial names
                continue;

            // Skip any further checks if the two names begin with different letters (given that the names are listed alphabetically)
            if(db[a].m_FullName.leftRef(1).compare(db[b].m_FullName.leftRef(1), Qt::CaseInsensitive) != 0)
                break;

            // If a match
            if(isSimilar(db[a].m_FullName, db[b].m_FullName, maximumDistance))
            {
                // Add a new group if no previous matches have been found so far
                if(!firstMatch)
                {
                    s_Duplicates->addNewGroup();
                    s_Duplicates->addToGroup(db[a].ID); // Add source name to the group
                    firstMatch = true;
                }

                s_Duplicates->addToGroup(db[b].ID); // Add the compared name to the group
                ++counter;
            }
        }
    }

    progress.setValue(size);

    QLocale locale;
    qDebug() << QString("PASS 3: %1 potential duplicates found").arg(locale.toString(counter));
}

// --- Check whether two strings are sufficiently similar --- //
bool Staff::isSimilar(QString a, QString b, const qint32 &maximumDistance)
{
    const qint32 m(a.size());
    const qint32 n(b.size());

    if( m==0 ) return n;
    if( n==0 ) return m;

    qint32 *costs = new qint32[n + 1];

    for( qint32 k=0; k<=n; k++ ) costs[k] = k;

    qint32 i = 0;
    for ( QString::const_iterator it1 = a.begin(); it1 != a.end(); ++it1, ++i )
    {
    costs[0] = i+1;
    qint32 corner = i;

    qint32 j = 0;
    for ( QString::const_iterator it2 = b.begin(); it2 != b.end(); ++it2, ++j )
    {
      qint32 upper = costs[j+1];
      if( *it1 == *it2 )
      {
          costs[j+1] = corner;
      }
      else
      {
        qint32 t(upper<corner?upper:corner);
        costs[j+1] = (costs[j]<t?costs[j]:t)+1;
      }

      corner = upper;
    }
    }

    qint32 result = costs[n];
    delete [] costs;

    return (result <= maximumDistance);
}


/* ================= */
/*      File I/O     */
/* ================= */

void Staff::read(QDataStream &in)
{
    in  >> ID
        >> FirstNameId
        >> SecondNameId
        >> CommonNameId
        >> DateOfBirth
        >> YearOfBirth
        >> Nation
        >> SecondNation
        >> InternationalApps
        >> InternationalGoals
        >> NationContracted
        >> JobForNation
        >> DateJoinedNation
        >> ContractExpiresNation
        >> ClubContracted
        >> JobForClub
        >> DateJoinedClub
        >> ContractExpiresClub
        >> EstimatedWage
        >> EstimatedValue
        >> Adaptability
        >> Ambition
        >> Determination
        >> Loyalty
        >> Pressure
        >> Professionalism
        >> Sportsmanship
        >> Temperament
        >> PlayingSquad
        >> Classification
        >> ClubValuation
        >> PlayerData
        >> Preferences // Version 0x02 - New ptr type
        >> NonPlayerData
        // Runtime data
        >> EuroSquadFlag;

    // First name and second name UID
    m_FirstAndSecondNameUID = Name::getFirstAndSecondNameUID(FirstNameId, SecondNameId);

    // Full name
    m_FullName = QString("%1 %2").arg(FirstName::getName(FirstNameId)).arg(SecondName::getName(SecondNameId));

    // Validate/set Year of Birth from DOB
    const qint16 yob = DateOfBirth.year();
    if(yob > 1900)
        YearOfBirth = yob;
}

// --- Read all data --- //
qint32 Staff::readAll(QDataStream &in, const qint32 &count)
{
    for(qint32 i = 0; i < count; ++i)
    {
        Staff tmp;
        tmp.read(in);
        db.push_back(tmp);
    }

    return db.size();
}


/* ================= */
/*      File I/O     */
/* ================= */

// --- Sort method (common name) --- //
bool Staff::sortCommonName(const Staff &a, const Staff &b)
{
    // First: Common name
    if(a.CommonNameId != b.CommonNameId)
        return(a.CommonNameId < b.CommonNameId);

    // Second: Year of birth
    else if(a.YearOfBirth != b.YearOfBirth)
        return(a.YearOfBirth < b.YearOfBirth);

    return(a.ID < b.ID);
}

// --- Sort method (full name) --- //
bool Staff::sortFullName(const Staff &a, const Staff &b)
{
    // First: Full name UID
    if(a.m_FirstAndSecondNameUID != b.m_FirstAndSecondNameUID)
        return(a.m_FirstAndSecondNameUID < b.m_FirstAndSecondNameUID);

    // Second: Year of birth
    else if(a.YearOfBirth != b.YearOfBirth)
        return(a.YearOfBirth < b.YearOfBirth);

    return(a.ID < b.ID);
}

// --- Sort method (standard/default) --- //
bool Staff::sortStandard(const Staff &a, const Staff &b)
{
    return(a.ID < b.ID);
}


/* ================= */
/*      Get Data     */
/* ================= */

// --- Sort method (common name) --- //
QString Staff::getClubName(const qint32 &id)
{
    if(id >= 0 && id < db.size())
        return QString::number(db[id].ClubContracted);
    else
        return "n/a";
}

QString Staff::getClubCompName(const qint32 &id)
{
    if(id >= 0 && id < db.size())
        return "tbc";
    else
        return "n/a";
}

QString Staff::getCommonName(const qint32 &id)
{
    if(id >= 0 && id < db.size())
        return CommonName::getName(db[id].CommonNameId);
    else
        return "n/a";
}

qint32 Staff::getCommonNameId(const qint32 &id)
{
    if(id >= 0 && id < db.size())
        return db[id].CommonNameId;
    else
        return -999999999;
}

QDate Staff::getDateOfBirth(const qint32 id)
{
    if(id >= 0 && id < db.size())
        return db[id].DateOfBirth.get();
    else
        return QDate(1,1,1);
}

QString Staff::getFirstName(const qint32 &id)
{
    if(id >= 0 && id < db.size())
        return FirstName::getName(db[id].FirstNameId);
    else
        return "n/a";
}

qint64 Staff::getFullNameUid(const qint32 &id)
{
    if(id >= 0 && id < db.size())
        return db[id].m_FirstAndSecondNameUID;
    else
        return -999999999;
}

QString Staff::getSecondName(const qint32 &id)
{
    if(id >= 0 && id < db.size())
        return SecondName::getName(db[id].SecondNameId);
    else
        return "n/a";
}

qint32 Staff::getYearOfBirth(const qint32 &id)
{
    if(id >= 0 && id < db.size())
        return db[id].YearOfBirth;
    else
        return -9999;
}
