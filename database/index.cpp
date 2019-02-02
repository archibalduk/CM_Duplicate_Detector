#include "index.h"

// --- Static data --- //
QVector<Index> Index::db;
qint8 Index::s_Header[INDEX_HEADER_SIZE];


/* ==================== */
/*      Index Data      */
/* ==================== */

// --- Default constructor --- //
Index::Index() :
    FileId(0),
    TableSize(0),
    Offset(0),
    Version(0)
{

}


/* ================= */
/*      File I/O     */
/* ================= */

// --- Read row of data --- //
void Index::read(QDataStream &in)
{
    Filename.read(in, MAX_INDEX_NAME + 1);
    in >> FileId >> TableSize >> Offset >> Version;
}

// --- Read all data --- //
qint32 Index::readAll(QDataStream &in)
{
    while(!in.atEnd())
    {
        Index tmp;
        tmp.read(in);
        db.push_back(tmp);
    }

    return db.size();
}


/* ================= */
/*      Get Data     */
/* ================= */

// --- File Name --- //
QString Index::getFileName()
{
    return Filename.get();
}

// --- Offset --- //
qint32 Index::getOffset()
{
    return Offset;
}

// --- Record count --- //
qint32 Index::getRecordCount()
{
    return TableSize;
}


/* ================= */
/*      Set Data     */
/* ================= */

// --- Count/size and offset --- //
void Index::set(const qint32 size, const qint32 offset)
{
    TableSize = size;
    Offset = offset;
}
