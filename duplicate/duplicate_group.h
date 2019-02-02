#ifndef DUPLICATE_GROUP_H
#define DUPLICATE_GROUP_H

// Qt headers
#include <QBitArray>
#include <QTextStream>
#include <QVector>

// --- Duplicate group --- //
class DuplicateGroup
{
private:
    // Flag
    bool m_ContainsDuplicates;

    // Duplicate items
    QVector<qint32> m_IdList;
    QBitArray m_DuplicateFlag;

public:
    // Constructor
    DuplicateGroup();

    // Add data
    void add(const qint32 &id);

    // Export data
    void writeActualDuplicatesOnly(QTextStream &s, const qint32 &id);
    void writeAll(QTextStream &s, const qint32 &id);

    // Get data
    bool containsDuplicates();
    QVector<qint32> *getIdList();

    // Set data
    bool flagDuplicate(const qint32 &id);
};

#endif // DUPLICATE_GROUP_H
