#ifndef CSERIALCONTROLTHREAD_H
#define CSERIALCONTROLTHREAD_H

#include "thread.h"

class CSerialControl;

class CSerialControlThread : public Thread
{
public:
    CSerialControlThread(CSerialControl *pSerialControl);
    ~CSerialControlThread();

protected:
    virtual void thread();

private:
    CSerialControl *m_pSerialControl;
};

#endif // CSERIALCONTROLTHREAD_H
