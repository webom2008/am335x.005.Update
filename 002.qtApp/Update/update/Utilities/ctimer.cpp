#include <string.h>

#include "ctimer.h"
#define CLOCK_ID            CLOCK_REALTIME

/**
 * clock_id
 * CLOCK_REALTIME           :Systemwide realtime clock.
 * CLOCK_MONOTONIC          :Represents monotonic time. Cannot be set.
 * CLOCK_PROCESS_CPUTIME_ID :High resolution per-process timer.
 * CLOCK_THREAD_CPUTIME_ID  :Thread-specific timer.
 * CLOCK_REALTIME_HR        :High resolution version of CLOCK_REALTIME.
 * CLOCK_MONOTONIC_HR       :High resolution version of CLOCK_MONOTONIC.
 */

void timer_thread(union sigval v)
{
    CTimer *pTimer = static_cast<CTimer *>(v.sival_ptr);
    if(NULL != pTimer)
    {
        pTimer->threadRun(v);
    }
}

CTimer::CTimer()
    :timerid(NULL)
    ,pParam(NULL)
    ,times(0)
    ,runtimes(0)
{
}

CTimer::~CTimer()
{
    this->stop();
    this->pParam = NULL;
    this->function = NULL;
    this->times = 0;
    this->runtimes = 0;
}

int CTimer::start(void *v, long long s, long long ns, FUNCTION_TYPE function, int policy, int priority, int times, int identifier)
{
    if(NULL != timerid)
    {
        return -1;
    }
    this->pParam = v;
    this->function = function;
    this->times = times;
    this->runtimes = 0;

    struct sigevent evp;                                        //Specify the asynchronous notification to be generated timer expires
    memset(&evp, 0, sizeof(struct sigevent));
    //init attr
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //set policy
    pthread_attr_setschedpolicy(&attr, policy);
    //set priority
    struct sched_param param;
    param.__sched_priority = priority;
    pthread_attr_setschedparam(&attr, &param);
    //enable pthread attributes
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    //New thread accredited
    evp.sigev_notify = SIGEV_THREAD;                            //Thread notice
    evp.sigev_value.sival_int = identifier;                     //identifier
    evp.sigev_value.sival_ptr = this;
    evp.sigev_notify_attributes = &attr;
    evp.sigev_notify_function = timer_thread;                   //thread function

    timer_create(CLOCK_ID, &evp, &timerid);               //create timer

    struct itimerspec timerspec;
    timerspec.it_interval.tv_sec = s;
    timerspec.it_interval.tv_nsec = ns;
    timerspec.it_value.tv_sec = timerspec.it_interval.tv_sec;
    timerspec.it_value.tv_nsec = timerspec.it_interval.tv_nsec;
    timer_settime(timerid, 0, &timerspec, NULL);                //start timer

    return 0;
}

void CTimer::stop()
{
    if(NULL != timerid)
    {
        timer_delete(timerid);
        timerid = NULL;
    }
}

int CTimer::getTime()
{
    if(NULL != timerid)
    {
        struct itimerspec timerspec;
        if(timer_gettime(timerid, &timerspec) != -1)
        {
            return timerspec.it_value.tv_sec*1000+timerspec.it_value.tv_nsec/1000000;
        }
    }
    return -1;
}

bool CTimer::isActive()
{
    return (NULL != timerid);
}

void CTimer::threadRun(union sigval v)
{
    runtimes++;
    if(0 != times && runtimes >= times)
    {
        this->stop();
    }

    v.sival_ptr = pParam;
    this->function(v);
}
