#include "csystemdatetime.h"
#include <sys/time.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

static const char default_rtc[] = "/dev/rtc";

bool CSystemDateTime::isInit = false;
unsigned long long CSystemDateTime::sysTick = 0;
unsigned long long CSystemDateTime::modifyTick = 0;
DateTime CSystemDateTime::modifyTime = DateTime();

CSystemDateTime::CSystemDateTime()
{
}

CSystemDateTime::~CSystemDateTime()
{
}

void CSystemDateTime::initSysTick()
{
    if(!isInit)
    {
        isInit = true;
        sysTick = getCurSysTick();
        modifyTick = 0;
        modifyTime = CSystemDateTime::currentDateTime();
    }
}

unsigned long long CSystemDateTime::getSysTick()
{
    return isInit ? getCurSysTick()-sysTick : 0;
}

DateTime CSystemDateTime::getBootTime()
{
    DateTime bootTime = CSystemDateTime::currentDateTime();
    CSystemDateTime::getDateTime(modifyTick, modifyTime, 0, bootTime);
    return bootTime;
}

unsigned long long CSystemDateTime::getModifyTick()
{
    return modifyTick;
}

DateTime CSystemDateTime::getModifyTime()
{
    return modifyTime;
}

bool CSystemDateTime::getDateTime(const unsigned long long &curSysTik, DateTime &curDateTime)
{
    if(modifyTime.isNull() || !modifyTime.isValid())
    {
        return false;
    }
    curDateTime = modifyTime.addMSecs((int)(curSysTik-modifyTick));
    return true;
}

bool CSystemDateTime::getDateTime(const unsigned long long &compSysTick, const DateTime &compDateTime,\
                                 const unsigned long long &curSysTick, DateTime &curDateTime)
{
    if(compDateTime.isNull() || !compDateTime.isValid())
    {
        return false;
    }
    curDateTime = compDateTime.addMSecs((int)(curSysTick-compSysTick));
    return true;
}

bool CSystemDateTime::setDateTime(const DateTime &dateTime)
{
    if(CSystemDateTime::setRTCDateTime(dateTime) && CSystemDateTime::setSysDateTime(dateTime))
    {
        modifyTick = CSystemDateTime::getSysTick();
        modifyTime = CSystemDateTime::currentDateTime();
        return true;
    }
    return false;
}

bool CSystemDateTime::setSysDateTime(const DateTime &dateTime)
{
    struct tm t;
    struct timespec tp;
    memset(&t, 0, sizeof(struct tm));
    t.tm_sec = dateTime.time().second();
    t.tm_min = dateTime.time().minute();
    t.tm_hour = dateTime.time().hour();
    t.tm_mday = dateTime.date().day();
    t.tm_mon = dateTime.date().month()-1;
    t.tm_year = dateTime.date().year()-1900;
    if(clock_gettime(CLOCK_REALTIME, &tp) == 0\
            && (tp.tv_sec = mktime(&t)) != -1\
            && clock_settime(CLOCK_REALTIME, &tp) == 0)
    {
        return true;
    }
    return false;
}

bool CSystemDateTime::setRTCDateTime(const DateTime &dateTime)
{
    int fd = -1;
    struct rtc_time rtc_tm;
    memset(&rtc_tm, 0, sizeof(struct rtc_time));
    rtc_tm.tm_sec = dateTime.time().second();
    rtc_tm.tm_min = dateTime.time().minute();
    rtc_tm.tm_hour = dateTime.time().hour();
    rtc_tm.tm_mday = dateTime.date().day();
    rtc_tm.tm_mon = dateTime.date().month()-1;
    rtc_tm.tm_year = dateTime.date().year()-1900;
    if((fd = open(default_rtc, O_WRONLY)) > 0)
    {
        int retVal = ioctl(fd, RTC_SET_TIME, &rtc_tm);
        close(fd);
        if(retVal == 0)
        {
            return true;
        }
    }
    return false;
}

Time CSystemDateTime::currentTime()
{
    return Time::currentTime();
}

Date CSystemDateTime::currentDate()
{
    return Date::currentDate();
}

DateTime CSystemDateTime::currentDateTime()
{
    return DateTime::currentDateTime();
}

long long CSystemDateTime::currentMSecsSinceEpoch()
{
    return DateTime::currentDateTime().toMSecsSinceEpoch();
}

unsigned long long CSystemDateTime::getCurSysTick()
{
    struct timespec tp;
    if(clock_gettime(CLOCK_MONOTONIC, &tp) != 0)
    {
        return 0;
    }
    return tp.tv_sec*1000+tp.tv_nsec/1000000.0+0.5;
}
