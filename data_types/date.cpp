#include "date.h"


/* ==================== */
/*      CM/SI Date      */
/* ==================== */

// --- Default Constructor --- //
Date::Date() :
    m_Date(QDate(1900,2,1))
{
}


/* ================== */
/*      Get Data      */
/* ================== */

// --- Get QDate --- //
QDate Date::get()
{
    return m_Date;
}

// --- Get as SI Date --- //
void Date::toSIDate(qint16 &day, qint16 &year, qint32 &isLeapYear) const
{
    day = (m_Date.dayOfYear() > 0) ? static_cast<qint16>(m_Date.dayOfYear() - 1) : 0;
    year = static_cast<qint16>(m_Date.year());
    isLeapYear = m_Date.isLeapYear(year) ? -1 : 0;
}

// --- Get year --- //
qint16 Date::year()
{
    return static_cast<qint16>(m_Date.year());
}


/* ============================== */
/*      Operator Overloading      */
/* ============================== */

// --- Read data from a data stream --- //
QDataStream & operator >>(QDataStream &in, Date &data)
{
    // Buffer
    qint16 day;  // days from Jan 1st.
    qint16 year;
    qint32 isLeapYear;  // = 1 if year is a leapyear, 0 otherwise.

    // Read SI data
    in >> day >> year >> isLeapYear;

    // Convert to QDate
    data.set(day, year);

    return in;
}

// --- Write data to a data stream --- //
QDataStream & operator <<(QDataStream &out, const Date &data)
{
    // Buffer
    qint16 day;  // days from Jan 1st.
    qint16 year;
    qint32 isLeapYear;  // = 1 if year is a leapyear, 0 otherwise.

    // Convert to QDate
    data.toSIDate(day, year, isLeapYear);

    // Write SI data
    out << day << year << isLeapYear;

    return out;
}


/* ================= */
/*      Set Data     */
/* ================= */

// --- Reset date --- //
void Date::reset()
{
    m_Date.setDate(1900, 2, 1);
}

// --- Set from SI Date --- //
void Date::set(qint16 &day, qint16 &year)
{
    // Correct any random or invalid years
    if(year <= 1900) {
        m_Date.setDate(1900,2,1);
        return;
    }

    // Reset the date to zero
    m_Date.setDate(year,1,1);

    // Add days
    m_Date = m_Date.addDays(day);
}

// --- Set from QDate --- //
void Date::set(const QDate &value)
{
    if(value.isValid())
        m_Date = value;
}
