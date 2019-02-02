#include "name.h"

// Qt headers
#include <QFile>
#include <QTextCodec>
#include <QTextStream>

// --- Static data: Containers --- //
QVector<CommonName> CommonName::db;
QVector<FirstName> FirstName::db;
QVector<SecondName> SecondName::db;

// --- Static data: Unique name counts --- //
qint32 CommonName::s_UniqueNameCount = 0;
qint32 FirstName::s_UniqueNameCount = 0;
qint32 SecondName::s_UniqueNameCount = 0;


/* =================== */
/*      Name Data      */
/* =================== */

// --- Default constructor (base class) --- //
Name::Name() :
    UID(-1),
    Count(0)
{
}

// --- Default constructor (Common name) --- //
CommonName::CommonName() :
    Name()
{
    ID = CommonName::db.size(); // Allocate next ID to item
}

// --- Default constructor (First name) --- //
FirstName::FirstName() :
    Name()
{
    ID = FirstName::db.size(); // Allocate next ID to item
}

// --- Default constructor (Second name) --- //
SecondName::SecondName() :
    Name()
{
    ID = SecondName::db.size(); // Allocate next ID to item
}


/* ================= */
/*      File I/O     */
/* ================= */

// --- Read row of data --- //
void Name::read(QDataStream &in, QHash<QString, qint32> &names)
{
    NameText.read(in, STANDARD_TEXT_LENGTH);
    in >> ID >> Nation >> Count;

    // Use the NONE flag as the unique ID if the name string is empty
    if(NameText.isEmpty())
    {
        UID = NONE;
        return;
    }

    // Find unique ID
    const QString simpleName = NameText.getSimpleString();
    const qint32 result = names.value(simpleName, NONE);

    // If a unique ID already exists for the name, use the existing unique ID
    if(result > NONE)
    {
        UID = result;
    }
    // If not: Generate a unique ID
    else
    {
        UID = names.size();
        names.insert(simpleName, UID);
    }
}

// --- Read synonym data into a hash table --- //
void Name::readSynonyms(const QString &fileName, QHash<QString, qint32> &names)
{
    // File
    QFile file(QString("%1.txt").arg(fileName));

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream;
    stream.setCodec(QTextCodec::codecForName("Latin-1"));    // ASCII
    stream.setDevice(&file);

    // Parse each row of data
    while(!stream.atEnd())
    {
        const qint32 groupUid = names.size();   // Next Group UID

        QString row = stream.readLine();
        QVector<QStringRef> items = row.splitRef(',', QString::SkipEmptyParts, Qt::CaseInsensitive);

        const qint32 sz = items.size();
        for(qint32 i = 0; i < sz; ++i)
        {
            names.insert(String::toSimpleString(items[i].toString()), groupUid);
        }
    }

    // Return whether the data contains at least one item
    file.close();
}

// --- Read all data --- //
qint32 CommonName::readAll(QDataStream &in)
{    
    // Hash table containing unique names list
    QHash<QString, qint32> names;

    // Load the synonym data
    Name::readSynonyms("common_name", names);

    // Read each row of the table
    while(!in.atEnd())
    {
        CommonName tmp;
        tmp.read(in, names);
        db.push_back(tmp);
    }

    s_UniqueNameCount = names.size();
    return db.size();
}

// --- Read all data --- //
qint32 FirstName::readAll(QDataStream &in)
{
    // Hash table containing unique names list
    QHash<QString, qint32> names;

    // Load the synonym data
    Name::readSynonyms("first_name", names);

    // Read each row of the table
    while(!in.atEnd())
    {
        FirstName tmp;
        tmp.read(in, names);
        db.push_back(tmp);
    }

    s_UniqueNameCount = names.size();
    return db.size();
}

// --- Read all data --- //
qint32 SecondName::readAll(QDataStream &in)
{
    // Hash table containing unique names list
    QHash<QString, qint32> names;

    // Load the synonym data
    Name::readSynonyms("second_name", names);

    // Read each row of the table
    while(!in.atEnd())
    {
        SecondName tmp;
        tmp.read(in, names);
        db.push_back(tmp);
    }

    s_UniqueNameCount = names.size();
    return db.size();
}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Get name text for models --- //
QString Name::get()
{
    return NameText.get();
}

// --- Get name --- //
QString CommonName::getName(const qint32 &id)
{
    if(id >= 0 && id < db.size())
        return db[id].NameText.get();
    else
        return "";
}

// --- Get name --- //
QString FirstName::getName(const qint32 &id)
{
    if(id >= 0 && id < db.size())
        return db[id].NameText.get();
    else
        return "";
}

// --- Get name --- //
QString SecondName::getName(const qint32 &id)
{
    if(id >= 0 && id < db.size())
        return db[id].NameText.get();
    else
        return "";
}

// --- Get first and second name UID --- //
qint64 Name::getFirstAndSecondNameUID(const qint32 &firstNameId, const qint32 &secondNameId)
{
    qint32 firstNameUid = -1;
    qint32 secondNameUid = -1;

    if(firstNameId > NONE && firstNameId < FirstName::db.size())
        firstNameUid = FirstName::db[firstNameId].UID;

    if(secondNameId > NONE && secondNameId < SecondName::db.size())
        secondNameUid = SecondName::db[secondNameId].UID;

    qint64 uid = static_cast<qint64>(firstNameUid);
    uid = uid << 32;
    uid += static_cast<qint64>(secondNameUid);
    return uid;
}

// --- Get unique name count --- //
qint32 CommonName::getUniqueNameCount()
{
    return s_UniqueNameCount;
}

// --- Get unique name count --- //
qint32 FirstName::getUniqueNameCount()
{
    return s_UniqueNameCount;
}

// --- Get unique name count --- //
qint32 SecondName::getUniqueNameCount()
{
    return s_UniqueNameCount;
}
