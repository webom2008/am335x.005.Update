#include "cutilities.h"

CUtilities::CUtilities()
{
}

char* CUtilities::QString2Char(const QString &str, QByteArray &tmp)
{
    tmp = str.toLatin1();
    return tmp.data();
}

bool CUtilities::Char2QString(QString &str, const char *pChar)
{
    if(NULL == pChar)
    {
        return false;
    }

    str = QString(QLatin1String(pChar));
    return true;
}
