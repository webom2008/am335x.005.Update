#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>

#include "cserialcontrol.h"
#include "cserialdevice.h"
#include "caiomodule.h"
#include "ckeymodule.h"
#include "cbckmodule.h"
#include "crecordermodule.h"
#include "../Utilities/sysconfig.h"
#include "../Utilities/canalysisdata.h"

CSerialControl::CSerialControl(
        CAIOModule *pAIOModule,
        CKeyModule *pKeyModule,
        CBckModule *pBckModule,
        CRecorderModule *pRecorderModule)
    :m_pAIOModule(pAIOModule)
    ,m_pKeyModule(pKeyModule)
    ,m_pBckModule(pBckModule)
    ,m_pRecorderModule(pRecorderModule)
{
    m_SerialDeviceList[AIO_ADDR] = (new CSerialDevice(this, AIO_ADDR, AIO_PORT, O_RDWR | O_NOCTTY | O_NDELAY, B230400, 8, 'N', 1, 'O'));
    m_SerialDeviceList[KEY_ADDR] = (new CSerialDevice(this, KEY_ADDR, KEY_PORT, O_RDWR | O_NOCTTY | O_NDELAY, B115200, 8, 'N', 1, 'O'));
    m_SerialDeviceList[BCK_ADDR] = (new CSerialDevice(this, BCK_ADDR, BCK_PORT, O_RDWR | O_NOCTTY | O_NDELAY, B115200, 8, 'N', 1, 'O'));
    m_SerialDeviceList[RECORDER_ADDR] = (new CSerialDevice(this, RECORDER_ADDR, RECORDER_PORT, O_RDWR | O_NOCTTY | O_NDELAY, B19200, 8, 'N', 1, 'O'));
}

CSerialControl::~CSerialControl()
{
    std::map<unsigned char, CSerialDevice *>::iterator it;
    for(it = m_SerialDeviceList.begin(); it != m_SerialDeviceList.end(); it++)
    {
        delete it->second;
        it->second = NULL;
    }
    m_SerialDeviceList.clear();
}

void CSerialControl::build_fdset(FdSet &fdSet)
{
    std::map<unsigned char, CSerialDevice *>::iterator it;
    for(it = m_SerialDeviceList.begin(); it != m_SerialDeviceList.end(); it++)
    {
        CSerialDevice *pDevice = it->second;
        if(NULL != pDevice)
        {
            pDevice->build_fdset(fdSet);
        }
    }
}

void CSerialControl::process(FdSet &fdSet)
{
    std::map<unsigned char, CSerialDevice *>::iterator it;
    for(it = this->m_SerialDeviceList.begin(); it != m_SerialDeviceList.end(); it++)
    {
        CSerialDevice *pDevice = it->second;
        if(NULL != pDevice)
        {
            pDevice->process(fdSet);
        }
    }
}

void CSerialControl::close()
{
    std::map<unsigned char, CSerialDevice *>::iterator it;
    for(it = m_SerialDeviceList.begin(); it != m_SerialDeviceList.end(); it++)
    {
        CSerialDevice *pDevice = it->second;
        if(NULL != pDevice)
        {
            pDevice->close_serial();
        }
    }
}

void CSerialControl::send_rxData(const CAnalysisData &rxData)
{
    switch (rxData.m_SrcAddr) {
    case AIO_ADDR:
        if(NULL != m_pAIOModule)
        {
            m_pAIOModule->write_rxData(rxData);
        }
        break;
    case KEY_ADDR:
        if(NULL != m_pKeyModule)
        {
            m_pKeyModule->write_rxData(rxData);
        }
        break;
    case BCK_ADDR:
        if(NULL != m_pBckModule)
        {
            m_pBckModule->write_rxData(rxData);
        }
        break;
    case RECORDER_ADDR:
        if(NULL != m_pRecorderModule)
        {
            m_pRecorderModule->write_rxData(rxData);
        }
        break;
    default:
        break;
    }
}

bool CSerialControl::send_txData(const CAnalysisData &txData)
{
    CSerialDevice *pDevice = m_SerialDeviceList[txData.m_DstAddr];
    if(NULL != pDevice)
    {
        return pDevice->write_txData(txData);
    }
    return false;
}
