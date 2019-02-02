#ifndef STAFF_H
#define STAFF_H

// Qt headers
#include <QDataStream>
#include <QVector>

// Application headers
#include "../data_types/date.h"
#include "../duplicate/duplicate_manager.h"

// --- Staff.dat - Staff --- //
class Staff
{   
private:
    // Combined name UID
    qint64 m_FirstAndSecondNameUID;

    // Full name
    QString m_FullName;

    // Database data
    qint32 ID;
    qint32 FirstNameId;
    qint32 SecondNameId;
    qint32 CommonNameId;
    Date DateOfBirth;
    qint16 YearOfBirth;
    qint32 Nation;
    qint32 SecondNation;
    quint8 InternationalApps;
    quint8 InternationalGoals;
    qint32 NationContracted;
    qint8 JobForNation;
    Date DateJoinedNation;
    Date ContractExpiresNation;
    qint32 ClubContracted;
    qint8 JobForClub;
    Date DateJoinedClub;
    Date ContractExpiresClub;
    qint32 EstimatedWage;
    qint32 EstimatedValue;
    qint8 Adaptability;
    qint8 Ambition;
    qint8 Determination;
    qint8 Loyalty;
    qint8 Pressure;
    qint8 Professionalism;
    qint8 Sportsmanship;
    qint8 Temperament;
    qint8 PlayingSquad;
    qint8 Classification;
    qint8 ClubValuation;
    qint32 PlayerData;
    qint32 Preferences; // Version 0x02 - New ptr type
    qint32 NonPlayerData;

    // Runtime data
    qint8 EuroSquadFlag;

    // Duplicates
    static void checkDuplicateStaff();
    static void findDuplicateStaff_Standard_Pass1();
    static void findDuplicateStaff_Standard_Pass2();
    static void findDuplicateStaff_SimilarStrings();
    static bool isSimilar(QString a, QString b, const qint32 &maximumDistance);

    // File I/O
    void read(QDataStream &in);

public:
    // Constructor
    Staff();

    // Container
    static QVector<Staff> db;

    // Duplicates
    static DuplicateManager *s_Duplicates;
    static void findDuplicateStaff();

    // File I/O
    static qint32 readAll(QDataStream &in, const qint32 &count);

    // Get data
    static QString getClubName(const qint32 &id);
    static QString getClubCompName(const qint32 &id);
    static QString getCommonName(const qint32 &id);
    static qint32 getCommonNameId(const qint32 &id);
    static QDate getDateOfBirth(const qint32 id);
    static QString getFirstName(const qint32 &id);
    static qint64 getFullNameUid(const qint32 &id);
    static QString getSecondName(const qint32 &id);
    static qint32 getYearOfBirth(const qint32 &id);

    // Sort data
    static bool sortCommonName(const Staff &a, const Staff &b);
    static bool sortFullName(const Staff &a, const Staff &b);
    static bool sortStandard(const Staff &a, const Staff &b);
};

#endif // STAFF_H
