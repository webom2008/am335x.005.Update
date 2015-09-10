#include "cserialcontrolthread.h"
#include "../Devices/cserialcontrol.h"
#include "fdset.h"

CSerialControlThread::CSerialControlThread(CSerialControl *pSerialControl)
    :m_pSerialControl(pSerialControl)
{

}

CSerialControlThread::~CSerialControlThread()
{

}

void CSerialControlThread::thread()
{
    if(NULL != this->m_pSerialControl)
    {
        FdSet fdSet;
        while(!this->isShutdown())
        {
            fdSet.reset();
            m_pSerialControl->build_fdset(fdSet);
            fdSet.selectMilliSeconds(THREAD_SLEEP_TIME);
            m_pSerialControl->process(fdSet);
            mTimes++;
        }
    }
}
