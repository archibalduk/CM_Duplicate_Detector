#ifndef DUPLICATE_MANAGER_H
#define DUPLICATE_MANAGER_H

// Qt headers
#include <QVector>

// Application headers
#include "duplicate_group.h"

// --- Duplicate tracker/manager --- //
class DuplicateManager
{
private:
    // Counter
    qint32 m_ActualDuplicateCount;

    // Data
    QVector<DuplicateGroup> m_Groups;

public:
    // Constructor
    DuplicateManager();

    // Export data
    bool writeData(const bool &exportAll = false);

    // Groups
    qint32 m_CurrentGroup;
    void addNewGroup();
    void addToGroup(const qint32 &id);

    // Get data
    qint32 getActualDuplicateCount();
    QVector<qint32> *getGroupIdList(const qint32 &group);
    qint32 size();

    // Set data
    void flagDuplicates(const qint32 &group, const qint32 &idFirst, const qint32 &idSecond);
};

#endif // DUPLICATE_MANAGER_H
