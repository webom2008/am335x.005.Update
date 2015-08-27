#ifndef CTIMER_H
#define CTIMER_H

#include <signal.h>
#include <time.h>
#include <pthread.h>

typedef void (*FUNCTION_TYPE)(union sigval);

class CTimer
{
public:
    CTimer();
    ~CTimer();

    int start(void *v, long long s, long long ns, FUNCTION_TYPE function, int policy = 0, int priority = 0, int times = 0, int identifier = pthread_self());
    void stop();
    int getTime();
    bool isActive();
    void threadRun(union sigval v);

private:
    timer_t timerid;
    void *pParam;
    unsigned long long times;
    unsigned long long runtimes;
    FUNCTION_TYPE function;
};

#endif // CTIMER_H
