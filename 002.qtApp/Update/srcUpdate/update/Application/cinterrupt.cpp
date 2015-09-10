#include "cinterrupt.h"
#include "../UI/mainwindow.h"

#define RTC_DRIVER_PATH         "/dev/rtc0"

unsigned long long mTimes = 0;
void thread(union sigval v);

CInterrupt::CInterrupt(MainWindow *pMainWindow)
    :m_pMainWindow(pMainWindow)
{

}

CInterrupt::~CInterrupt()
{

}

void CInterrupt::startTimer(int policy, int priority)
{
    mTimes = 0;
    timer.start(m_pMainWindow,\
                INTERRUPT_TIME_INTERVAL/INIT_BASE_LENGTH,\
                INTERRUPT_TIME_INTERVAL%INIT_BASE_LENGTH*1000000,\
                thread,\
                policy,\
                priority);
}

void CInterrupt::stopTimer()
{
    timer.stop();
}

unsigned long long CInterrupt::runTimes() const
{
    return mTimes;
}

void thread(union sigval v)
{
    MainWindow *pMainWindow = static_cast<MainWindow *>(v.sival_ptr);
    if(NULL != pMainWindow)
    {
        pMainWindow->process();
    }
    mTimes++;
}
