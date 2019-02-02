#include "duplicate_group.h"

// Application headers
#include "../database/staff.h"


/* ========================= */
/*      Duplicate Group      */
/* ========================= */

// --- Default constructor --- //
DuplicateGroup::DuplicateGroup() :
    m_ContainsDuplicates(false)
{

}


/* ================== */
/*      Add Data      */
/* ================== */

// --- Add a potential duplicate --- //
void DuplicateGroup::add(const qint32 &id)
{
    m_IdList.push_back(id);
    m_DuplicateFlag.resize(m_IdList.size());
}


/* ===================== */
/*      Export Data      */
/* ===================== */

// --- Write actual duplicates only --- //
void DuplicateGroup::writeActualDuplicatesOnly(QTextStream &s, const qint32 &id)
{
    const qint32 size = m_IdList.size();
    const QChar d = ',';

    for(int i = 0; i < size; ++i)
    {
        if(m_DuplicateFlag.testBit(i) == true)
        {
            s << id << d << m_IdList[i] << d
              << Staff::getCommonName(m_IdList[i]) << d << Staff::getFirstName(m_IdList[i]) << d << Staff::getSecondName(m_IdList[i]) << d
              << Staff::getDateOfBirth(m_IdList[i]).toString("dd.MM.yyyy") << d << Staff::getYearOfBirth(m_IdList[i]) << d
              << Staff::getClubName(m_IdList[i]) << d << Staff::getClubCompName(m_IdList[i]) << d
              << Staff::getFullNameUid(m_IdList[i]) << d << Staff::getCommonNameId(m_IdList[i]) << "\n";
        }
    }
}

// --- Write potential and actual duplicates --- //
void DuplicateGroup::writeAll(QTextStream &s, const qint32 &id)
{
    const qint32 size = m_IdList.size();
    const QChar d = ',';

    for(int i = 0; i < size; ++i)
    {
        if(m_DuplicateFlag.testBit(i) == true)
        {
            s << id << d << m_IdList[i] << d
              << Staff::getCommonName(m_IdList[i]) << d << Staff::getFirstName(m_IdList[i]) << d << Staff::getSecondName(m_IdList[i]) << d
              << Staff::getDateOfBirth(m_IdList[i]).toString("dd.MM.yyyy") << d << Staff::getYearOfBirth(m_IdList[i]) << d
              << Staff::getClubName(m_IdList[i]) << d << Staff::getClubCompName(m_IdList[i]) << d
              << Staff::getFullNameUid(m_IdList[i]) << d << Staff::getCommonNameId(m_IdList[i]) << d
              << m_DuplicateFlag.testBit(i) << "\n";
        }
    }
}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Contains actual duplicates? --- //
bool DuplicateGroup::containsDuplicates()
{
    return m_ContainsDuplicates;
}

// --- Get Id list --- //
QVector<qint32> *DuplicateGroup::getIdList()
{
    return  &m_IdList;
}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Flag an item as duplicate --- //
bool DuplicateGroup::flagDuplicate(const qint32 &id)
{
    const bool isNewDuplicate =!m_DuplicateFlag.testBit(id);

    if(id >= 0 && id < m_IdList.size())
    {
        m_DuplicateFlag.setBit(id, true);
        m_ContainsDuplicates = true;
    }

    return isNewDuplicate;
}
