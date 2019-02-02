#include "database.h"

// Qt headers
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QLocale>

// Database headers
#include "index.h"
#include "name.h"
#include "staff.h"


/* ================== */
/*      Database      */
/* ================== */

// --- Default constructor --- //
Database::Database()
{

}

// --- Default destructor --- //
Database::~Database()
{
    // Clear the tables
    Index::db.clear();
    CommonName::db.clear();
    FirstName::db.clear();
    SecondName::db.clear();
    Staff::db.clear();
}


/* ================== */
/*      File I/O      */
/* ================== */

// --- Open database --- //
bool Database::openDatabase(const QString &path)
{
    // Read the index first
    this->read(path, "index.dat", &Index::readAll, Index::INDEX_HEADER_SIZE);

    // Read the rest of the database
    this->read(path, "common_names.dat", &CommonName::readAll);
    this->read(path, "first_names.dat", &FirstName::readAll);
    this->read(path, "second_names.dat", &SecondName::readAll);
    this->read(path, "staff.dat", &Staff::readAll, Index::db[Index::STAFF_TABLE].getOffset(), Index::db[Index::STAFF_TABLE].getRecordCount());

    // Output the number of unique names
    QLocale locale;
    qDebug() << "\n";
    qDebug() << QString("Unique common names: %1").arg(locale.toString(CommonName::getUniqueNameCount()));
    qDebug() << QString("Unique first names: %1").arg(locale.toString(FirstName::getUniqueNameCount()));
    qDebug() << QString("Unique second names: %1").arg(locale.toString(SecondName::getUniqueNameCount()));
    QCoreApplication::processEvents();

    // Find and export duplicates
    Staff::findDuplicateStaff();

    return true;
}

// --- Read .dat file/table --- //
bool Database::read(const QString &path, const QString &fileName, qint32 (*fnRead)(QDataStream &), const qint32 &offset)
{
    // Open the file (read only)
    QFile file(QString("%1/%2").arg(path).arg(fileName));
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "ERROR: Unable to open" << file.fileName();
        return false;
    }

    // Data stream
    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);
    in.skipRawData(offset);

    // Read the data
    const qint32 result = fnRead(in);

    QLocale locale;
    qDebug() << QString("%1: %2 records / %3 bytes").arg(fileName).arg(locale.toString(result)).arg(locale.toString(in.device()->pos()));
    QCoreApplication::processEvents();

    file.close();
    return true;
}

// --- Read .dat file/table (defined number of records) --- //
bool Database::read(const QString &path, const QString &fileName, qint32 (*fnRead)(QDataStream &, const qint32 &),
                    const qint32 &offset, const qint32 &count)
{
    // Open the file (read only)
    QFile file(QString("%1/%2").arg(path).arg(fileName));
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "ERROR: Unable to open" << file.fileName();
        return false;
    }

    // Data stream
    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);
    in.skipRawData(offset);

    // Read the data
    const qint32 result = fnRead(in, count);

    QLocale locale;
    qDebug() << QString("%1: %2 records / %3 bytes").arg(fileName).arg(locale.toString(result)).arg(locale.toString(in.device()->pos()));
    QCoreApplication::processEvents();

    file.close();
    return true;
}
