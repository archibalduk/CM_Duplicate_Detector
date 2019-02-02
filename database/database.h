#ifndef DATABASE_H
#define DATABASE_H

// Qt headers
#include <QDataStream>
#include <QString>

// --- Database wrapper --- //
class Database
{
public:
    // Constructor
    Database();
    ~Database();

    // File I/O
    bool openDatabase(const QString &path);

private:
    // File I/O
    bool read(const QString &path, const QString &fileName, qint32 (*fnRead)(QDataStream &), const qint32 &offset = 0);
    bool read(const QString &path, const QString &fileName, qint32 (*fnRead)(QDataStream &, const qint32 &), const qint32 &offset, const qint32 &count);
};

#endif // DATABASE_H
