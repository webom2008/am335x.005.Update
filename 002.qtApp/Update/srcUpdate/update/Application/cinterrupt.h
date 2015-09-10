#ifndef CINTERRUPT_H
#define CINTERRUPT_H

#include "../Utilities/ctimer.h"

#define INIT_BASE_LENGTH            1000                //1s
#define INTERRUPT_TIME_INTERVAL     25                  //25ms

class MainWindow;

class CInterrupt
{
public:
    CInterrupt(MainWindow *pMainWindow);
    ~CInterrupt();

public:
    void startTimer(int policy = 0, int priority = 0);
    void stopTimer();
    unsigned long long runTimes() const;

private:
    CTimer           timer;
    MainWindow      *m_pMainWindow;
};
#endif // CINTERRUPT_H
