#include "autolock.h"
#include <iostream>

Mutex::Mutex()
{
}

Mutex::~Mutex()
{
}

void Mutex::Lock()
{
    this->lock();
}

void Mutex::unLock()
{
    this->unlock();
}

MutexLocker::MutexLocker(Mutex &lock):
    QMutexLocker(&lock)
{
}

MutexLocker::MutexLocker(Mutex *lock):
    QMutexLocker(lock)
{
}

MutexLocker::~MutexLocker()
{
}
