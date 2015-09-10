#ifndef THREAD_H
#define THREAD_H
#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

#define THREAD_SLEEP_TIME           25

class Thread{
public:
    Thread();
    virtual ~Thread();
    virtual void run(int policy = 0, int priority = 0);
    virtual void shutdown();
    virtual void thread() = 0;
    bool isShutdown() const;
    unsigned long long runTimes() const;

protected:
    void join();

protected:
#ifdef WIN32
    HANDLE mThread;
    unsigned int mId;
#else
    pthread_t mId;
#endif
    bool mShutdown;
    unsigned long long mTimes;
};
#endif
