#ifndef TEMPLATECLASS_H
#define TEMPLATECLASS_H


#include "autolock.h"
template <class T>
class TemplateArray
{
public:
    TemplateArray(int length);
    ~TemplateArray();

    bool write(const T *t);
    bool write(const T &t);
    bool read(T *t);
    bool read(T &t);
    int size();
    bool isFull();
    void clear();

private:
    Mutex mMutex;
    const int dataLen;
    unsigned long long startIndex, endIndex;
    T *buffer;
};


#endif // TEMPLATECLASS_H
