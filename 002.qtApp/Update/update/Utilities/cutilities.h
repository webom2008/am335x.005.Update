#ifndef CUTILITIES_H
#define CUTILITIES_H

#include <QString>

class CUtilities
{
public:
    CUtilities();
    static char* QString2Char(const QString &str, QByteArray &tmp);
    static bool Char2QString(QString &str, const char *pChar);
};

#endif // CUTILITIES_H
