#include "duplicate_manager.h"

// Qt headers
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QTextCodec>
#include <QTextStream>

/* =========================== */
/*      Duplicate Manager      */
/* =========================== */

// --- Default constructor --- //
DuplicateManager::DuplicateManager() :
    m_ActualDuplicateCount(0),
    m_CurrentGroup(-1)
{

}


/* ================ */
/*      Groups      */
/* ================ */

// --- Add a new group in readiness for adding IDs --- //
bool DuplicateManager::writeData(const bool &exportAll)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    QFile f(QString("%1/Duplicate Report.csv")
            .arg(settings.value("database_path", "C:").toString()));

    if(!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        qDebug() << QString("ERROR: Unable to write spreadsheet to %1").arg(f.fileName());
        QCoreApplication::processEvents();
        return false;
    }

    qDebug() << QString("Saving spreadsheet: %1").arg(f.fileName());
    QCoreApplication::processEvents();


    // Text stream (for writing text to the file)
    QTextStream s;
    s.setCodec(QTextCodec::codecForName("Latin-1"));    // ASCII
    s.setDevice(&f);

    const qint32 size = m_Groups.size();
    const QChar d = ',';

    // Actual duplicates only
    if(!exportAll)
    {
        s << "Group" << d << "Staff ID" << d
          << "Common Name" << d << "Forename" << d << "Surname" << d
          << "DOB" << d << "YOB" << d
          << "Club" << d << "League" << d
          << "Full Name UID" << d << "Common Name ID"
          << "\n";

        qint32 id = 0;  // Group Id

        for(qint32 i = 0; i < size; ++i)
        {
            if(m_Groups[i].containsDuplicates())
            {
                m_Groups[i].writeActualDuplicatesOnly(s, id);
                ++id;
            }
        }
    }
    // Actual and potential duplicates
    else
    {
        s << "Group" << d << "Staff ID" << d
          << "Common Name" << d << "Forename" << d << "Surname" << d
          << "DOB" << d << "YOB" << d
          << "Club" << d << "League" << d
          << "Full Name UID" << d << "Common Name ID" << d
          << "Is Duplicate?"
          << "\n";

        for(qint32 i = 0; i < size; ++i)
        {
            m_Groups[i].writeAll(s, i);
        }
    }

    s.flush();
    f.close();

    qDebug() << QString("Spreadsheet saved.");
    QCoreApplication::processEvents();
    return true;
}


/* ================ */
/*      Groups      */
/* ================ */

// --- Add a new group in readiness for adding IDs --- //
void DuplicateManager::addNewGroup()
{
    DuplicateGroup grp;
    m_Groups.push_back(grp);
    ++m_CurrentGroup;
}

// --- Add an ID to the current group --- //
void DuplicateManager::addToGroup(const qint32 &id)
{
    m_Groups[m_CurrentGroup].add(id);
}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Get actual duplicate count --- //
qint32 DuplicateManager::getActualDuplicateCount()
{
    return  m_ActualDuplicateCount;
}

// --- Add a new group in readiness for adding IDs --- //
QVector<qint32> *DuplicateManager::getGroupIdList(const qint32 &group)
{
    if(group >= 0 && group < m_Groups.size())
        return m_Groups[group].getIdList();
    else
        return nullptr;
}

// --- Get Duplicate Group size/count --- //
qint32 DuplicateManager::size()
{
    return m_Groups.size();
}


/* ================== */
/*      Set Data      */
/* ================== */

// --- Flag two duplicates --- //
void DuplicateManager::flagDuplicates(const qint32 &group, const qint32 &idFirst, const qint32 &idSecond)
{
    if(group >= 0 && group < m_Groups.size())
    {
        if(m_Groups[group].flagDuplicate(idFirst) == true)  // Increment the counter if this is the first time this group has a duplicate
            ++m_ActualDuplicateCount;

        m_Groups[group].flagDuplicate(idSecond);
    }
}
