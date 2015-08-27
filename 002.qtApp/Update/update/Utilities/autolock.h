#ifndef AUTOLOCK_H
#define AUTOLOCK_H

#include <QMutex>

class Mutex : public QMutex{
public:
    Mutex();
    ~Mutex();
    void Lock();
    void unLock();
};

class MutexLocker : public QMutexLocker{
public:
    MutexLocker(Mutex &lock);
    MutexLocker(Mutex *lock);
    ~MutexLocker();
};

#endif // AUTOLOCK_H
