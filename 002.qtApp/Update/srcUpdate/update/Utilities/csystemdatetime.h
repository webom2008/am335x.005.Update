#ifndef CSYSTEMDATETIME_H
#define CSYSTEMDATETIME_H


#include <time.h>
#include <QDateTime>

typedef QTime       Time;
typedef QDate       Date;
typedef QDateTime   DateTime;

class CSystemDateTime
{
public:
    CSystemDateTime();
    ~CSystemDateTime();

public:
    static void initSysTick();

    static unsigned long long getSysTick();

    static DateTime getBootTime();

    static unsigned long long getModifyTick();

    static DateTime getModifyTime();

    static bool getDateTime(const unsigned long long &curSysTik, DateTime &curDateTime);

    static bool getDateTime(const unsigned long long &compSysTick, const DateTime &compDateTime,\
                            const unsigned long long &curSysTick, DateTime &curDateTime);

    static bool setDateTime(const DateTime &dateTime);

    static Time currentTime();
    static Date currentDate();
    static DateTime currentDateTime();

    static long long currentMSecsSinceEpoch();

private:
    static unsigned long long getCurSysTick();
    static bool setSysDateTime(const DateTime &dateTime);
    static bool setRTCDateTime(const DateTime &dateTime);

private:
    static bool isInit;
    static unsigned long long sysTick;
    static unsigned long long modifyTick;
    static DateTime modifyTime;
};

#endif // CSYSTEMDATETIME_H
