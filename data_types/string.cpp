#include "string.h"

// Qt headers
#include <QByteArray>
#include <QRegExp>


/* =============== */
/*      String     */
/* =============== */

// --- Default constructor --- //
String::String() :
    m_String()
{
}

/* ================= */
/*      File I/O     */
/* ================= */

// --- Read raw char to String --- //
void String::read(QDataStream &stream, const quint16 &textLength)
{
    m_String.reserve(textLength);

    QByteArray ba(textLength, 0);
    stream.readRawData(ba.data(), textLength);
    m_String = QString::fromLatin1(ba);
    m_String.replace(0x0099, 0x2122); // Convert from EHM 200x/1 TM (trademark) to UTF symbol
}


/* ================= */
/*      Get Data     */
/* ================= */

// --- Get as QString --- //
QString String::get()
{
    return m_String;
}

// --- Get as a simple/match string --- //
QString String::getSimpleString()
{
    return this->toSimpleString(m_String);
}

// --- Is empty? --- //
bool String::isEmpty()
{
    return m_String.trimmed().isEmpty();
}

// --- Convert a text string to a simple/match string --- //
QString String::toSimpleString(const QString &text)
{
    QString t = text.trimmed().toLower();

    // Importer-specific
    t.replace(",", "_");
    t.replace(";", "_");
    t.replace("-", " "); // Hyphen -> Space

    // Remove characters to be ignored
    t.remove(".");
    t.remove("'");
    t.remove(0x2122);    // Trademark

    // Replace characters not corrected by normalising
    t.replace("ø", "o");

    // Remove accents and convert for Latin-1
    t = text.normalized(QString::NormalizationForm_KD).remove(QRegExp("^a-zA-Z\\s\\d_:")).toLatin1();

    // Remove any question marks left over from conversion to Latin-1
    return t.remove("?");
}
