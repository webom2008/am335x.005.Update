#include "templateclass.h"
#include "canalysisdata.h"

template <class T>
TemplateArray<T>::TemplateArray(int length)
    :dataLen(length)
    ,startIndex(0)
    ,endIndex(0)
    ,buffer(NULL)
{
    MutexLocker mLocker(&mMutex);(void)mLocker;
    buffer = new T[dataLen];
}

template <class T>
TemplateArray<T>::~TemplateArray()
{
    MutexLocker mLocker(&mMutex);(void)mLocker;
    if(NULL != buffer)
    {
        delete[] buffer;
        buffer = NULL;
    }
}

template <class T>
bool TemplateArray<T>::write(const T *t)
{
    MutexLocker mLocker(&mMutex);(void)mLocker;
    if(NULL == t || NULL == buffer)
    {
        return false;
    }
    buffer[endIndex%dataLen] = *t;
    endIndex++;
    while(startIndex+dataLen < endIndex)
    {
        startIndex++;
    }
    return true;
}

template <class T>
bool TemplateArray<T>::write(const T &t)
{
    return this->write(&t);
}

template <class T>
bool TemplateArray<T>::read(T *t)
{
    MutexLocker mLocker(&mMutex);(void)mLocker;
    if(NULL == t || startIndex == endIndex || NULL == buffer)
    {
        return false;
    }
    *t = buffer[startIndex%dataLen];
    startIndex++;
    return true;
}

template <class T>
bool TemplateArray<T>::read(T &t)
{
    return this->read(&t);
}

template <class T>
int TemplateArray<T>::size()
{
    MutexLocker mLocker(&mMutex);(void)mLocker;
    return endIndex-startIndex;
}

template <class T>
bool TemplateArray<T>::isFull()
{
    MutexLocker mLocker(&mMutex);(void)mLocker;
    return endIndex == (startIndex+dataLen);
}

template <class T>
void TemplateArray<T>::clear()
{
    MutexLocker mLocker(&mMutex);(void)mLocker;
    startIndex = 0;
    endIndex = 0;
}

template class TemplateArray<CAnalysisData>;
