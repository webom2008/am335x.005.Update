#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../Utilities/includes.h"
#include "../Application/cdatacontroller.h"
#include "cupdatemodule.h"

static const UINT8 UPDATE_PACKET_DATA_LEN = 128;

const unsigned char SOFTWARE_UPDATE_ASK[] = \
{0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00};
const unsigned char SOFTWARE_UPDATE_ANSWER[] = \
{0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA};

const int PROCESS_PERCENT_RECEIVE_TAG       = 2;
const int PROCESS_PERCENT_RECEIVE_LEN       = 5;
const int PROCESS_PERCENT_DATA              = 95;
const int PROCESS_PERCENT_RECEIVE_END       = 97;
const int PROCESS_PERCENT_RECEIVE_DONE      = 100;

#define OPEN_DEBUG
#ifdef OPEN_DEBUG
#define UPDATE_DBG(fmt, arg...)         printf("\r\n[Update]DEB:"fmt, ##arg)
#define UPDATE_ERR(fmt, arg...)         printf("\r\n[Update]ERR:"fmt, ##arg)
#define UPDATE_INFO(fmt, arg...)        printf(fmt, ##arg)
#else
#define UPDATE_DBG(fmt, arg...)         do{}while(0)
#define UPDATE_ERR(fmt, arg...)         do{}while(0)
#define UPDATE_INFO(fmt, arg...)        do{}while(0)
#endif

CUpdateModule::CUpdateModule(CModuleBase *pParent)
    :m_pParent(pParent)
    ,m_Status(SF_UPDATE_IDEL)
    ,m_BinFileLen(0)
    ,m_BinFileOffset(0)
    ,m_PacketCount(0)
    ,m_pBinFile(NULL)
{
}

CUpdateModule::~CUpdateModule()
{
    if (NULL != m_pBinFile)
    {
        delete m_pBinFile;
        m_pBinFile = NULL;
    }
}

int CUpdateModule::initApplication(const UINT8 &target,
                                    const QString &strBinFilePath)
{
    switch(target)
    {
    case CSerialAnalysis::SF_SPO2_UPDATE:
    {
        m_SendData.m_DstAddr = AIO_ADDR;
        m_SendData.m_Type = CSerialAnalysis::SF_SPO2_UPDATE;
    }
        break;
    case CSerialAnalysis::SF_AIO_STM_UPDATE:
    {
        m_SendData.m_DstAddr = AIO_ADDR;
        m_SendData.m_Type = CSerialAnalysis::SF_AIO_STM_UPDATE;
    }
        break;
    case CSerialAnalysis::SF_AIO_DSP_UPDATE:
    {
        m_SendData.m_DstAddr = AIO_ADDR;
        m_SendData.m_Type = CSerialAnalysis::SF_AIO_DSP_UPDATE;
    }
        break;
    case CSerialAnalysis::SF_BACK_UPDATE:
    {
        m_SendData.m_DstAddr = BCK_ADDR;
        m_SendData.m_Type = CSerialAnalysis::SF_BACK_UPDATE;
    }
        break;
    case CSerialAnalysis::SF_RECORD_UPDATE:
    {
        m_SendData.m_DstAddr = RECORDER_ADDR;
        m_SendData.m_Type = CSerialAnalysis::SF_RECORD_UPDATE;
    }
        break;
    default:
        break;
    }
    m_SendData.m_IsNeedACK = true;

    return readFile2RAM(strBinFilePath);
}

void CUpdateModule::sendUpdateTag(void)
{
    if(NULL != this->m_pParent)
    {
        m_SendData.m_Len = sizeof(SOFTWARE_UPDATE_ASK);
        memcpy(m_SendData.m_Data, SOFTWARE_UPDATE_ASK, sizeof(SOFTWARE_UPDATE_ASK));
        this->m_pParent->write_txData(m_SendData);
        this->m_Status = SF_UPDATE_RECEIVE_TAG;
        m_PacketCount = 0;
    }
}

int CUpdateModule::readFile2RAM(const QString &strBinFilePath)
{
    unsigned int readLen;
    long ret = -1;
    FILE *fp = NULL;
    QByteArray tmp;
    char *pFilePath = NULL;
    QString print;

    pFilePath = CUtilities::QString2Char(strBinFilePath, tmp);

    fp = fopen(pFilePath, "rb");
    if (NULL == fp)
    {
        print.sprintf(">>readFile2RAM fopen %s ERROR",pFilePath);
        CMyConsole::addStr2Console(print);
        return -1;
    }

    if (fseek(fp, 0, SEEK_END) < 0)
    {
        print.sprintf(">>fseek %s ERROR",pFilePath);
        CMyConsole::addStr2Console(print);
        fclose(fp);
        return -1;
    }

    ret = ftell(fp);
    if (ret < 0)
    {
        print.sprintf("ftell %s ERROR",pFilePath);
        CMyConsole::addStr2Console(print);
        fclose(fp);
        return -1;
    }
    m_BinFileLen = ret;

    if (NULL != m_pBinFile)
    {
        delete m_pBinFile;
        m_pBinFile = NULL;
    }
//    m_pBinFile = (UINT8 *)malloc(m_BinFileLen);
    m_pBinFile = new UINT8[m_BinFileLen];
    if (NULL == m_pBinFile)
    {
        print.sprintf("malloc for m_pBinFile ERROR");
        CMyConsole::addStr2Console(print);
        fclose(fp);
        return -1;
    }

    if (fseek(fp, 0, SEEK_SET) < 0)
    {
        print.sprintf("fseek %s ERROR",pFilePath);
        CMyConsole::addStr2Console(print);
        fclose(fp);
        return -1;
    }

    readLen = fread(m_pBinFile, 1, m_BinFileLen, fp);
    fclose(fp);

    if (readLen != m_BinFileLen)
    {
        print.sprintf("fread %s ERROR",pFilePath);
        CMyConsole::addStr2Console(print);
        return -1;
    }
    return 0;
}


void CUpdateModule::doUpdateProcess(const CAnalysisData &data)
{
    QString print;
    int index = 0;
    int nWaitToWriteLen;

    if((NULL == this->m_pParent) || (data.m_Type != m_SendData.m_Type))
    {
        return;
    }

    switch (this->m_Status) //receive handle
    {
    case SF_UPDATE_RECEIVE_TAG:
    {
        if ((sizeof(SOFTWARE_UPDATE_ANSWER) == data.m_Len) \
            && (0 == memcmp(data.m_Data, SOFTWARE_UPDATE_ANSWER, data.m_Len)))
        {
            this->m_Status = SF_UPDATE_SEND_LEN;
            CMyConsole::refreshPrecessBar(PROCESS_PERCENT_RECEIVE_TAG);
            print.sprintf(">>SF_UPDATE_RECEIVE_TAG OK");
            CMyConsole::addStr2Console(print);
        }
        else
        {
            sendUpdateTag();
            this->m_Status = SF_UPDATE_RECEIVE_TAG;
            print.sprintf(">>SF_UPDATE_RECEIVE_TAG ERROR");
            CMyConsole::addStr2Console(print);
            return;
        }
    }
        break;
    case SF_UPDATE_RECEIVE_LEN:
    {
        if ((unsigned char)UPDATE_ACK == data.m_Data[0]) //success
        {
            m_PacketCount++;
            m_BinFileOffset = 0;
            this->m_Status = SF_UPDATE_SEND_DATA;
            CMyConsole::refreshPrecessBar(PROCESS_PERCENT_RECEIVE_LEN);

            print.sprintf(">>SF_UPDATE_RECEIVE_LEN OK");
        }
        else if ((unsigned char)UPDATE_SOL == data.m_Data[0]) //resend again
        {
            this->m_Status = SF_UPDATE_SEND_LEN;
            print.sprintf(">>SF_UPDATE_RECEIVE_LEN RESEND");
        }
        else if ((unsigned char)UPDATE_CA == data.m_Data[0])  // len out of flash
        {
            this->m_Status = SF_UPDATE_ERROR;
            print.sprintf(">>SF_UPDATE_RECEIVE_LEN ERROR");
        }
        else
        {
            //this->m_Status = SF_UPDATE_ERROR;
            this->m_Status = SF_UPDATE_SEND_LEN;
            print.sprintf(">>SF_UPDATE_RECEIVE_LEN UNKNOW");
        }
        CMyConsole::addStr2Console(print);
    }
        break;
    case SF_UPDATE_RECEIVE_DATA:
    {
        if ((unsigned char)UPDATE_ACK == data.m_Data[0]) //success
        {
            m_BinFileOffset = m_PacketCount * UPDATE_PACKET_DATA_LEN;
            m_PacketCount++;
            if (m_BinFileOffset < m_BinFileLen)
            {
                this->m_Status = SF_UPDATE_SEND_DATA;
            }
            else
            {
                this->m_Status = SF_UPDATE_SEND_END;
            }
            CMyConsole::refreshPrecessBar(
                        PROCESS_PERCENT_RECEIVE_LEN \
                        + (PROCESS_PERCENT_DATA - PROCESS_PERCENT_RECEIVE_LEN) \
                        * m_BinFileOffset / m_BinFileLen);

//            print.sprintf(">>SF_UPDATE_RECEIVE_DATA OK");
//            CMyConsole::addStr2Console(print);
        }
        else if ((unsigned char)UPDATE_SOD == data.m_Data[0]) //resend again
        {
            m_PacketCount = ((m_PacketCount>>8)<<8) + data.m_Data[1];
            m_BinFileOffset = (m_PacketCount -1) * UPDATE_PACKET_DATA_LEN;
            this->m_Status = SF_UPDATE_SEND_DATA;
            print.sprintf(">>SF_UPDATE_RECEIVE_DATA RESEND");
            UPDATE_ERR("");
            CMyConsole::addStr2Console(print);
        }
        else
        {
            //this->m_Status = SF_UPDATE_ERROR;

            m_PacketCount = ((m_PacketCount>>8)<<8) + data.m_Data[1];
            m_BinFileOffset = (m_PacketCount -1) * UPDATE_PACKET_DATA_LEN;
            this->m_Status = SF_UPDATE_SEND_DATA;
            print.sprintf(">>SF_UPDATE_RECEIVE_DATA UNKNOW");
            CMyConsole::addStr2Console(print);
        }
    }
        break;
    case SF_UPDATE_RECEIVE_END:
    {
        if ((unsigned char)UPDATE_NAK == data.m_Data[0]) //success
        {
//            if ((UINT8)CSerialAnalysis::SF_AIO_DSP_UPDATE == data.m_Type)
            {
                this->m_Status = SF_UPDATE_RECEIVE_DONE;
            }
//            else
            {

            }
            CMyConsole::refreshPrecessBar(PROCESS_PERCENT_RECEIVE_END);
            print.sprintf(">>SF_UPDATE_RECEIVE_END OK");
        }
        else //resend again
        {
            this->m_Status = SF_UPDATE_SEND_END;
            print.sprintf(">>SF_UPDATE_RECEIVE_END RESEND");
        }
        CMyConsole::addStr2Console(print);
    }
        break;
    case SF_UPDATE_RECEIVE_DONE:
    {
        if ((unsigned char)UPDATE_W_F_DONE == data.m_Data[0]) //success
        {
            this->m_Status = SF_UPDATE_IDEL;
            CMyConsole::refreshPrecessBar(PROCESS_PERCENT_RECEIVE_DONE);
            print.sprintf(">>SF_UPDATE_RECEIVE_DONE OK");
            if (NULL != m_pBinFile)
            {
                delete m_pBinFile;
                m_pBinFile = NULL;
            }
            CDataController::resetUpdateWorking();
        }
        else if ((unsigned char)UPDATE_W_F_ERR == data.m_Data[0]) //error
        {
            this->m_Status = SF_UPDATE_ERROR;
            print.sprintf(">>SF_UPDATE_RECEIVE_DONE ERROR");
        }
        else
        {
            print.sprintf(">>SF_UPDATE_RECEIVE_DONE UNKNOW");
        }
        CMyConsole::addStr2Console(print);
    }
        break;
    default:
        break;
    }


    switch (this->m_Status) //send handle
    {
    case SF_UPDATE_SEND_LEN:
    {
        m_PacketCount = 0;
        index = 0;
        m_SendData.m_Data[index++] = (unsigned char)UPDATE_SOL;
        m_SendData.m_Data[index++] = (unsigned char)(m_PacketCount & 0xFF);
        m_SendData.m_Data[index++] = (unsigned char)((m_BinFileLen >> 24) & 0xFF);
        m_SendData.m_Data[index++] = (unsigned char)((m_BinFileLen >> 16) & 0xFF);
        m_SendData.m_Data[index++] = (unsigned char)((m_BinFileLen >> 8) & 0xFF);
        m_SendData.m_Data[index++] = (unsigned char)((m_BinFileLen >> 0) & 0xFF);
        m_SendData.m_Len = index;
        this->m_pParent->write_txData(m_SendData);
        this->m_Status = SF_UPDATE_RECEIVE_LEN;
        print.sprintf(">>SF_UPDATE_SEND_LEN...");
        CMyConsole::addStr2Console(print);
    }
        break;
    case SF_UPDATE_SEND_DATA:
    {
        nWaitToWriteLen = m_BinFileLen - m_BinFileOffset;
        if (nWaitToWriteLen >= UPDATE_PACKET_DATA_LEN)
        {
            m_SendData.m_Len = UPDATE_PACKET_DATA_LEN + 2;
            memcpy(&m_SendData.m_Data[2],
                    (unsigned char *)(m_pBinFile + m_BinFileOffset),
                    UPDATE_PACKET_DATA_LEN);
        }
        else
        {
            m_SendData.m_Len = nWaitToWriteLen + 2;
            memcpy(&m_SendData.m_Data[2],
                    (unsigned char *)(m_pBinFile + m_BinFileOffset),
                    nWaitToWriteLen);
        }
        m_SendData.m_Data[0] = (unsigned char)UPDATE_SOD;
        m_SendData.m_Data[1] = (unsigned char)(m_PacketCount & 0xFF);

        this->m_pParent->write_txData(m_SendData);
        this->m_Status = SF_UPDATE_RECEIVE_DATA;
//        print.sprintf(">>SF_UPDATE_SEND_DATA...");
//        CMyConsole::addStr2Console(print);
    }
        break;
    case SF_UPDATE_SEND_END:
    {
        index = 0;
        m_SendData.m_Data[index++] = (unsigned char)UPDATE_EOT;
        m_SendData.m_Len = index;
        this->m_pParent->write_txData(m_SendData);
        this->m_Status = SF_UPDATE_RECEIVE_END;
        print.sprintf(">>SF_UPDATE_SEND_END...");
        CMyConsole::addStr2Console(print);
    }
        break;
    case SF_UPDATE_ERROR:
    {
        this->m_Status = SF_UPDATE_IDEL;
    }
        break;
    case SF_UPDATE_SEND_DONE:
    case SF_UPDATE_IDEL:
    default:
        break;
    }
}
