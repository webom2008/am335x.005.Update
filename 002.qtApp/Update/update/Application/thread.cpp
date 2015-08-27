#include "thread.h"
#include <cassert>
#ifdef WIN32
#include <process.h>
typedef unsigned(__stdcall *RESIP_THREAD_START_ROUTINE)(void*);
#endif

extern "C"
{
    static void*
#ifdef WIN32
    __stdcall
#endif
    threadWrapper(void* threadParm )
    {
       assert( threadParm );
       Thread* t = static_cast < Thread* > ( threadParm );
       assert( t );
       t->thread();
#ifdef WIN32
       _endthreadex(0);
#endif
       return 0;
    }
}

Thread::Thread():
#ifdef WIN32
    mThread(0),
#endif
    mId(0),
    mShutdown(true),
    mTimes(0)
{
}

Thread::~Thread()
{
    shutdown();
}

void Thread::run(int policy, int priority)
{
    assert(mId == 0);
    mShutdown = false;
    mTimes = 0;
#ifdef WIN32
    mThread = (HANDLE)_beginthreadex(
        NULL, // LPSECURITY_ATTRIBUTES lpThreadAttributes,  // pointer to security attributes
        0, // DWORD dwStackSize,                         // initial thread stack size
        RESIP_THREAD_START_ROUTINE
        (threadWrapper), // LPTHREAD_START_ROUTINE lpStartAddress,     // pointer to thread function
        this, //LPVOID lpParameter,                        // argument for new thread
        0, //DWORD dwCreationFlags,                     // creation flags
        &mId// LPDWORD lpThreadId                         // pointer to receive thread ID
        );
    assert( mThread != 0 );
#else
    pthread_attr_t attr;
    if(pthread_attr_init(&attr) != 0)
    {
//        ErrorLog(<<::strerror(errno));
        return;
    }
    if(pthread_attr_setschedpolicy(&attr, policy) != 0)
    {
//        ErrorLog(<<::strerror(errno));
        return;
    }
    struct sched_param param;
    param.sched_priority = priority;
    if(pthread_attr_setschedparam(&attr, &param) != 0)
    {
//        ErrorLog(<<::strerror(errno));
        return;
    }
    if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0)
    {
//        ErrorLog(<<::strerror(errno));
        return;
    }
    if (pthread_create( &mId, 0, threadWrapper, (void*)this) != 0)
    {
//        ErrorLog(<<::strerror(errno));
        return;
    }
#endif
}

void Thread::join()
{
    if (mId == 0)
    {
        return;
    }
#ifdef WIN32
    DWORD exitCode;
    while (true)
    {
        if (GetExitCodeThread(mThread,&exitCode) != 0)
        {
            if (exitCode != STILL_ACTIVE)
            {
                break;
            }
            else
            {
                WaitForSingleObject(mThread,INFINITE);
            }
        }
        else
        {
            // log something here
            break;
        }
    }

    CloseHandle(mThread);
    mThread=0;
#else
    void* stat;
    if (mId != pthread_self())
    {
        if ( pthread_join( mId , &stat ) != 0 )
        {
//            ErrorLog(<<strerror(errno));
        }
    }
#endif
    mId = 0;
}

void Thread::shutdown()
{
    if (!mShutdown)
    {
        mShutdown = true;
    }
    join();
}

bool Thread::isShutdown() const
{
    return ( mShutdown );
}

unsigned long long Thread::runTimes() const
{
    return mTimes;
}
