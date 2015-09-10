#include "fdset.h"

FdSet::FdSet() :
    size(0)
{
    FD_ZERO(&read);
    FD_ZERO(&write);
    FD_ZERO(&except);
}

FdSet::~FdSet()
{
}

int FdSet::selectMilliSeconds(unsigned long ms)
{
    return select(ms/1000, (ms%1000)*1000);
}

int FdSet::selectNanoSeconds(unsigned long ns)
{
    return select(ns/1000000000, (ns%1000000000)/1000);
}

int FdSet::select(unsigned long s, unsigned long us)
{
    struct timeval tv;
    tv.tv_sec = s;
    tv.tv_usec = us;
    return ::select(size, &read, &write, &except, &tv);
}

bool FdSet::readyToRead(int fd)
{
    return ( FD_ISSET(fd, &read) != 0);
}

bool FdSet::readyToWrite(int fd)
{
    return ( FD_ISSET(fd, &write) != 0);
}

bool FdSet::hasException(int fd)
{
    return (FD_ISSET(fd,&except) != 0);
}

void FdSet::setRead(int fd)
{
    assert( FD_SETSIZE >= 8 );
#ifndef WIN32 // windows fd are not int's and don't start at 0 - this won't work in windows
    assert( fd < (int)FD_SETSIZE ); // redefineing FD_SETSIZE will not work
#endif
    FD_SET(fd, &read);
    size = ( int(fd+1) > size) ? int(fd+1) : size;
}

void FdSet::setWrite(int fd)
{
#ifndef WIN32 // windows fd are not int's and don't start at 0 - this won't work in windows
    assert( fd < (int)FD_SETSIZE ); // redefinitn FD_SETSIZE will not work
#endif
    FD_SET(fd, &write);
    size = ( int(fd+1) > size) ? int(fd+1) : size;
}

void FdSet::setExcept(int fd)
{
#ifndef WIN32
    assert( fd < (int)FD_SETSIZE );
#endif
    FD_SET(fd,&except);
    size = ( int(fd+1) > size) ? int(fd+1) : size;
}

void FdSet::clear(int fd)
{
    FD_CLR(fd, &read);
    FD_CLR(fd, &write);
    FD_CLR(fd, &except);
}

void FdSet::reset()
{
    size = 0;
    FD_ZERO(&read);
    FD_ZERO(&write);
    FD_ZERO(&except);
}
