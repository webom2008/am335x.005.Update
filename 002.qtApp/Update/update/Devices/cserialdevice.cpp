#include "cserialdevice.h"
#include "../Utilities/sysconfig.h"
#include "../Devices/cserialcontrol.h"

CSerialDevice::CSerialDevice(CSerialControl *pSerialControl,\
                             unsigned char slaveAddr,\
                             const char *file,\
                             int mode,\
                             int speed,\
                             unsigned char data_bits,\
                             unsigned char parity,\
                             unsigned char stop_bits,\
                             unsigned char flow_ctrl)
    :m_pSerialControl(pSerialControl)
    ,m_Frame(MPU_ADDR, slaveAddr)
    ,mTxDataArray(DATA_LIST_MAX_SIZE)
    ,slaveAddr(slaveAddr)
    ,File(file)
    ,Mode(mode)
    ,Speed(speed)
    ,Data_Bits(data_bits)
    ,Stop_Bits(stop_bits)
    ,Parity(parity)
    ,Flow_Ctrl(flow_ctrl)
{

}

CSerialDevice::~CSerialDevice()
{

}

void CSerialDevice::open_serial()
{
    m_Serial.open_port(File, Mode, Speed, Data_Bits, Parity, Stop_Bits, Flow_Ctrl);
}

void CSerialDevice::close_serial()
{
    m_Serial.close_port();
}

void CSerialDevice::build_fdset(FdSet &fdSet)
{
    int fd = m_Serial.get_fd();
    if(-1 == fd)
    {
        this->open_serial();
        return;
    }
    fdSet.setExcept(fd);
    fdSet.setRead(fd);
    if(mTxDataArray.size() > 0)
    {
        fdSet.setWrite(fd);
    }
}

void CSerialDevice::process(FdSet &fdSet)
{
    int fd = m_Serial.get_fd();
    if(-1 == fd)
    {
        return;
    }
    if(fdSet.hasException(fd))
    {
//        ErrorLog(<<(int)frame.get_slave_addr()<<": "<<strerror(errno));
        this->close_serial();
        return;
    }
    if(fdSet.readyToWrite(fd))
    {
        if(!this->write_serial())
        {
//            ErrorLog(<<(int)frame.get_slave_addr()<<": "<<strerror(errno));
            this->close_serial();
            return;
        }
    }
    if(fdSet.readyToRead(fd))
    {
        if(!this->read_serial())
        {
//            ErrorLog(<<(int)frame.get_slave_addr()<<": "<<strerror(errno));
            this->close_serial();
            return;
        }
    }
}

bool CSerialDevice::write_txData(const CAnalysisData &txData)
{
    if(!mTxDataArray.isFull())
    {
        mTxDataArray.write(txData);
        return true;
    }
    return false;
}

bool CSerialDevice::read_txData(CAnalysisData &txData)
{
    if(mTxDataArray.read(txData))
    {
        return true;
    }
    return false;
}

bool CSerialDevice::write_serial()
{
    CAnalysisData txData;
    unsigned char buf[MAX_BUF_SIZE];
    while(this->read_txData(txData))
    {
        int len = m_Frame.unpack(txData, buf, MAX_BUF_SIZE);
        if(m_Serial.write_port(buf, len) != len)
        {
            return false;
        }
    }
    return true;
}

bool CSerialDevice::read_serial()
{
    int len;
    unsigned char buf[MAX_BUF_SIZE];
    if((len = m_Serial.read_port(buf, MAX_BUF_SIZE)) >= 0)
    {
        int tempLen = len;
        unsigned char *tempBuf = buf;
        while((len = m_Frame.pack(tempBuf, tempLen)) > 0)
        {
            tempLen -= len;
            tempBuf += len;
            if(NULL != this->m_pSerialControl)
            {
                this->m_pSerialControl->send_rxData(m_Frame.get_data());
            }
        }
        return true;
    }
    return false;
}
