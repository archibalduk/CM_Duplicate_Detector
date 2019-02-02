#ifndef STRING_H
#define STRING_H

// Qt headers
#include <QDataStream>
#include <QString>

// --- QString <--> SI String wrapper/interface class --- //
class String
{
private:
    // Text string
    QString m_String;

public:
    // Constructor
    String();

    // File I/O
    void read(QDataStream &stream, const quint16 &textLength);

    // Get data
    QString get();
    QString getSimpleString();
    bool isEmpty();
    static QString toSimpleString(const QString &text);
};

#endif // STRING_H
