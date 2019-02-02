#ifndef NAME_H
#define NAME_H

// Qt headers
#include <QDataStream>
#include <QHash>
#include <QVector>

// Application headers
#include "../data_types/string.h"
#include "../data_types/data_types.h"


// --- Name base class --- //
class Name
{
protected:
    String     NameText; // QString representation of the raw NameText char array
    //qint8  NameText[ STANDARD_TEXT_LENGTH ];
    qint32 ID;
    qint32 UID;
    qint32 Nation;
    qint8  Count;

    // File I/O
    void read(QDataStream &in, QHash<QString, qint32> &names);
    static void readSynonyms(const QString &fileName, QHash<QString, qint32> &names);

    // Flags
    enum ENUM_NAME {
        NONE = -1
    };

public:
    // Constructor
    Name();

    // Get data
    QString get();
    static qint64 getFirstAndSecondNameUID(const qint32 &firstNameId, const qint32 &secondNameId);
};

// --- Common_names.dat --- //
class CommonName : public Name
{
private:
    static qint32 s_UniqueNameCount;

public:
    // Constructor
    CommonName();

    // Containers
    static QVector<CommonName> db;

    // File I/O
    static qint32 readAll(QDataStream &in);

    // Get data
    static QString getName(const qint32 &id);
    static qint32 getUniqueNameCount();
};

// --- First_names.dat --- //
class FirstName : public Name
{
private:
    static qint32 s_UniqueNameCount;

public:
    // Constructor
    FirstName();

    // Containers
    static QVector<FirstName> db;

    // File I/O
    static qint32 readAll(QDataStream &in);

    // Get data
    static QString getName(const qint32 &id);
    static qint32 getUniqueNameCount();
};

// --- Second_names.dat --- //
class SecondName : public Name
{
private:
    static qint32 s_UniqueNameCount;

public:
    // Constructor
    SecondName();

    // Containers
    static QVector<SecondName> db;

    // File I/O
    static qint32 readAll(QDataStream &in);

    // Get data
    static QString getName(const qint32 &id);
    static qint32 getUniqueNameCount();
};

#endif // NAME_H
