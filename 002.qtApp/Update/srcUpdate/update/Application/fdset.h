#ifndef FDSET_H
#define FDSET_H

#include <sys/select.h>
#include <assert.h>

class FdSet
{
public:
    FdSet();
    ~FdSet();

    int selectMilliSeconds(unsigned long ms);
    int selectNanoSeconds(unsigned long ns);
    bool readyToRead(int fd);
    bool readyToWrite(int fd);
    bool hasException(int fd);
    void setRead(int fd);
    void setWrite(int fd);
    void setExcept(int fd);
    void clear(int fd);
    void reset();

private:
    int select(unsigned long s, unsigned long us);

private:
    fd_set read;
    fd_set write;
    fd_set except;
    int size;
};

#endif // FDSET_H
