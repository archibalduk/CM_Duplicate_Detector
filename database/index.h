#ifndef INDEX_H
#define INDEX_H

// Qt headers
#include <QDataStream>
#include <QString>
#include <QVector>

// Application headers
#include "data_types/data_types.h"
#include "data_types/string.h"

// --- Index.dat --- //
class Index
{
private:
    String  Filename;
    qint32  FileId;
    qint32  TableSize;
    qint32  Offset;
    qint32  Version;

    // File I/O
    void read(QDataStream &in);

public:  
    // Index header
    enum ENUM_HEADER_SIZE { INDEX_HEADER_SIZE = 8 };
    static qint8 s_Header[INDEX_HEADER_SIZE];

    // Constructor
    Index();

    // Container
    static QVector<Index> db;

    // File I/O
    static qint32 readAll(QDataStream &in);

    // Get data
    QString getFileName();
    qint32 getOffset();
    qint32 getRecordCount();

    // Set data
    void set(const qint32 size, const qint32 offset = 0);

    enum ENUM_INDEX_POSITIONS {
        CLUB_TABLE,
        NATION_CLUBS_TABLE,
        COLOUR_TABLE,
        CONTINENT_TABLE,
        NATION_TABLE,
        OFFICIALS_TABLE,
        STADIUM_TABLE,
        STAFF_TABLE,
        //YOUTH_PLAYER_TABLE = 8, // Not used
        NON_PLAYER_TABLE,
        PLAYER_TABLE,
        STAFF_COMP_TABLE,
        CITY_TABLE,
        CLUB_COMP_TABLE,
        NATION_COMP_TABLE,
        FIRST_NAME_TABLE,
        SECOND_NAME_TABLE,
        COMMON_NAME_TABLE,
        STAFF_HISTORY_TABLE,
        STAFF_COMP_HISTORY_TABLE,
        CLUB_COMP_HISTORY_TABLE,
        NATION_COMP_HISTORY_TABLE,
        STAFF_PREFERENCES_TABLE, // MUX with youth player table
        INDEX_TABLE_COUNT
    };
};

#endif // INDEX_H
