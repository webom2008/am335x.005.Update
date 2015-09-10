#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "cdataframe.h"
#include "../Utilities/sysconfig.h"

CDataFrame::CDataFrame(unsigned char host_addr, unsigned char slave_addr)
    :slaveAddr(slave_addr)
    ,hostAddr(host_addr)
    ,mStatus(WaitDstAddr)
{
    dataLen = 0;
}


CDataFrame::~CDataFrame()
{

}

CAnalysisData& CDataFrame::get_data()
{
    return rxData;
}

unsigned char CDataFrame::get_slave_addr()
{
    return slaveAddr;
}

int CDataFrame::unpack(CAnalysisData &txData, unsigned char *buf, int len)
{
    switch (slaveAddr)
    {
        case AIO_ADDR:
            return unpack_AIO(txData, buf, len);
            break;
        case KEY_ADDR:
        case BCK_ADDR:
        case RECORDER_ADDR:
            return unpack_data(txData, buf, len);
            break;
        default:
//            ErrorLog(<<"SRC_ADDR: "<<((int)slaveAddr));
            break;
    }
    return 0;
}

int CDataFrame::pack(const unsigned char *buf, int len)
{
    switch (slaveAddr)
    {
        case AIO_ADDR:
            return pack_AIO(buf, len);
            break;
        case KEY_ADDR:
        case BCK_ADDR:
        case RECORDER_ADDR:
            return pack_data(buf, len);
            break;
        default:
//            ErrorLog(<<"SRC_ADDR: "<<((int)slaveAddr));
            break;
    }
    return 0;
}

int CDataFrame::unpack_AIO(CAnalysisData &txData, unsigned char *buf, int len)
{
    assert(NULL != buf);

    if(len < txData.m_Len+6)
    {
        return 0;
    }
    txData.m_DstAddr = slaveAddr;
    txData.m_SrcAddr = MPU_ADDR;
    txData.m_SN = 0;
    buf[0] = txData.m_DstAddr;
    buf[1] = txData.m_SrcAddr;
    buf[2] = txData.m_SN;
    buf[3] = txData.m_Type;
    buf[4] = txData.m_Len;
    memcpy(buf+5, txData.m_Data, txData.m_Len);
    buf[txData.m_Len+5] = crc8_AIO(txData);
    //DebugLog(<<(int)buf[0]<<" "<<(int)buf[1]<<" "<<(int)buf[2]<<" "<<(int)buf[3]<<" "<<(int)buf[4]);
    return txData.m_Len+6;
}

int CDataFrame::pack_AIO(const unsigned char *buf, int len)
{
    assert(NULL != buf);

    for (int i=0; i<len;)
    {
        switch(mStatus)
        {
            case WaitDstAddr:
            {
                if (buf[i] == hostAddr)
                {
                    rxData.m_DstAddr = buf[i];
                    mStatus = WaitSrcAddr;
                }
                i++;
            }
                break;
            case WaitSrcAddr:
            {
                if (buf[i] == slaveAddr)
                {
                    rxData.m_SrcAddr = buf[i];
                    mStatus = WaitSN;
                    i++;
                }
                else
                {
                    mStatus = WaitDstAddr;
                }
            }
                break;
            case WaitSN:
            {
                rxData.m_SN = buf[i];
                mStatus = WaitType;
                i++;
            }
                break;
            case WaitType:
            {
                rxData.m_Type = buf[i];
                mStatus = WaitLen;
                i++;
            }
                break;
            case WaitLen:
            {
                if(DATA_BUF_LEN >= buf[i])
                {
                    dataLen = 0;
                    rxData.m_Len = buf[i];
                    if(0 < rxData.m_Len)
                    {
                        mStatus = WaitData;
                    }
                    else
                    {
                        mStatus = WaitCRC;
                    }
                    i++;
                }
                else
                {
                    mStatus = WaitDstAddr;
                }
            }
                break;
            case WaitData:
            {
                if(dataLen < rxData.m_Len)
                {
                    int tempLen = len-i >= rxData.m_Len-dataLen ? rxData.m_Len-dataLen : len-i;
                    memcpy(rxData.m_Data+dataLen, buf+i, tempLen);
                    dataLen += tempLen;
                    i += tempLen;
                }
                if(dataLen >= rxData.m_Len)
                {
                    mStatus = WaitCRC;
                }
            }
                break;
            case WaitCRC:
            {
                rxData.m_CRC = buf[i];
                i++;

                mStatus = WaitDstAddr;
                if(rxData.m_CRC == crc8_AIO(rxData))
                {
                    return i;
                }
            }
                break;
            default:
                mStatus = WaitDstAddr;
                break;
        }
    }
    return 0;
}

unsigned char CDataFrame::crc8_AIO(const CAnalysisData &mData)
{
    unsigned char crc = 0;
    unsigned char buf[DATA_BUF_LEN];
    buf[0] = mData.m_SN;
    buf[1] = mData.m_Type;
    buf[2] = mData.m_Len;
    memcpy(buf+3, mData.m_Data, mData.m_Len);

    for (int i = 0; i < mData.m_Len+3; i++)
    {
        crc ^= buf[i];
        for(int j = 0; j < 8; j++)
        {
            if(crc&0x01)
            {
                crc = (crc >> 1) ^ 0x8C;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}

int CDataFrame::unpack_data(CAnalysisData &txData, unsigned char *buf, int len)
{
    assert(NULL != buf);

    if(len < txData.m_Len+6)
    {
        return 0;
    }
    txData.m_DstAddr = slaveAddr;
    txData.m_SrcAddr = MPU_ADDR;
    txData.m_SN = 0;
    buf[0] = txData.m_DstAddr;
    buf[1] = txData.m_SrcAddr;
    buf[2] = txData.m_Type;
    buf[3] = txData.m_SN;
    buf[4] = txData.m_Len;
    memcpy(buf+5, txData.m_Data, txData.m_Len);
    buf[txData.m_Len+5] = crc8_data(txData);
    //DebugLog(<<(int)buf[0]<<" "<<(int)buf[1]<<" "<<(int)buf[2]<<" "<<(int)buf[3]<<" "<<(int)buf[4]);
    return txData.m_Len+6;
}

int CDataFrame::pack_data(const unsigned char *buf, int len)
{
    assert(NULL != buf);

    for (int i=0; i<len;)
    {
        switch(mStatus)
        {
            case WaitDstAddr:
            {
                if (buf[i] == hostAddr)
                {
                    rxData.m_DstAddr = buf[i];
                    mStatus = WaitSrcAddr;
                }
                i++;
            }
                break;
            case WaitSrcAddr:
            {
                if (buf[i] == slaveAddr)
                {
                    rxData.m_SrcAddr = buf[i];
                    mStatus = WaitType;
                    i++;
                }
                else
                {
                    mStatus = WaitDstAddr;
                }
            }
                break;
            case WaitType:
            {
                rxData.m_Type = buf[i];
                mStatus = WaitSN;
                i++;
            }
                break;
            case WaitSN:
            {
                rxData.m_SN = buf[i];
                mStatus = WaitLen;
                i++;
            }
                break;
            case WaitLen:
            {
                if(DATA_BUF_LEN >= buf[i])
                {
                    dataLen = 0;
                    rxData.m_Len = buf[i];
                    if(0 < rxData.m_Len)
                    {
                        mStatus = WaitData;
                    }
                    else
                    {
                        mStatus = WaitCRC;
                    }
                    i++;
                }
                else
                {
                    mStatus = WaitDstAddr;
                }
            }
                break;
            case WaitData:
            {
                if(dataLen < rxData.m_Len)
                {
                    int tempLen = len-i > rxData.m_Len-dataLen ? rxData.m_Len-dataLen : len-i;
                    memcpy(rxData.m_Data+dataLen, buf+i, tempLen);
                    dataLen += tempLen;
                    i += tempLen;
                }
                if(dataLen >= rxData.m_Len)
                {
                    mStatus = WaitCRC;
                }
            }
                break;
            case WaitCRC:
            {
                rxData.m_CRC = buf[i];
                i++;

                mStatus = WaitDstAddr;
                if(rxData.m_CRC == crc8_data(rxData))
                {
                    return i;
                }
            }
                break;
            default:
                mStatus = WaitDstAddr;
                break;
        }
    }
    return 0;
}

unsigned char CDataFrame::crc8_data(const CAnalysisData &mData)
{
    unsigned char crc = 0;
    unsigned char buf[DATA_BUF_LEN];
    buf[0] = mData.m_Type;
    buf[1] = mData.m_SN;
    buf[2] = mData.m_Len;
    memcpy(buf+3, mData.m_Data, mData.m_Len);

    for (int i = 0; i < mData.m_Len+3; i++)
    {
        crc ^= buf[i];
        for(int j = 0; j < 8; j++)
        {
            if(crc&0x01)
            {
                crc = (crc >> 1) ^ 0x8C;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}
