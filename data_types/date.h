#ifndef DATE_H
#define DATE_H

// Qt headers
#include <QDataStream>
#include <QDate>
#include <QString>
#include <QVariant>

// --- CM_DATE --- //
class Date
{
private:
    QDate m_Date;
    /*
    qint16   m_Day; // days from Jan 1st.
    qint16   m_Year;
    qint32   m_LeapYear; // =1 if year is a leapyear, 0 otherwise.
    */

public:
    // Constructor
    Date();

    // Get data
    QDate get();
    void toSIDate(qint16 &day, qint16 &year, qint32 &isLeapYear) const;
    qint16 year();

    // Operator overloading
    friend QDataStream & operator >> (QDataStream &in, Date &data);
    friend QDataStream & operator << (QDataStream &out, const Date &data);

    // Set data
    void reset();
    void set(qint16 &day, qint16 &year);
    void set(const QDate &value);
};

// Operator overloading
QDataStream & operator >> (QDataStream &in, Date &data);
QDataStream & operator << (QDataStream &out, const Date &data);

#endif // DATE_H
