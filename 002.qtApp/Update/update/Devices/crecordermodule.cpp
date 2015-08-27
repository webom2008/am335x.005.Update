#include "crecordermodule.h"
#include "../Utilities/sysconfig.h"
#include "cserialcontrol.h"
#include "cupdatemodule.h"

CRecorderModule::CRecorderModule(CSerialControl *pSerialControl)
    :CModuleBase(pSerialControl, &m_SerialRecorderAnalysis)
    ,m_pUpdateModule(new CUpdateModule(this))
{
}

CRecorderModule::~CRecorderModule()
{
    if (NULL != m_pUpdateModule)
    {
        delete m_pUpdateModule;
        m_pUpdateModule = NULL;
    }
}


void CRecorderModule::process_ms(const unsigned long long &sysTick)
{
    this->processRxDataList(sysTick);
    this->processTxDataList(sysTick);
}

void CRecorderModule::process_s(const unsigned long long &sysTick)
{
    this->processCommStatus(sysTick);
}


void CRecorderModule::processTxDataList(const unsigned long long &sysTick)
{
//    QString print;
    MutexLocker mLocker(txMutex); (void)mLocker;
    if(NULL != this->m_pSerialControl)
    {
        std::map<unsigned char, std::pair<CAnalysisData, unsigned long long> >::iterator it;
        for(it = this->txDataList.begin(); it != this->txDataList.end();)
        {
//            print.sprintf(">>======Recoder Send:0X%02X",it->second.first.m_Type);
//            CMyConsole::addStr2Console(print);
            if(sysTick-it->second.second >= RECORDER_RESPONSE_TIMEOUT)
            {
                if(this->m_pSerialControl->send_txData(it->second.first) && !it->second.first.m_IsNeedACK)
                {
                    this->txDataList.erase(it++);
                    continue;
                }
                it->second.second = sysTick;
            }
            it++;
        }
    }
}

void CRecorderModule::processRxDataList(const unsigned long long &sysTick)
{
    MutexLocker mLocker(rxMutex);(void)mLocker;
    while(this->rxDataList.size() > 0)
    {
        if(NULL != m_pAnalysis)
        {
            m_pAnalysis->try_Parse(this, rxDataList.front(), sysTick);
        }
        this->rxDataList.pop_front();
    }
}

void CRecorderModule::processCommStatus(const unsigned long long &sysTick)
{
    if(NULL != this->m_pAnalysis)
    {
        this->m_pAnalysis->processCommStatus(sysTick);
    }
}

void CRecorderModule::delete_txData(const CAnalysisData &rxData)
{
    MutexLocker mLocker(txMutex);(void)mLocker;
    std::map<unsigned char, std::pair<CAnalysisData, unsigned long long> >::iterator it;
    while((it = this->txDataList.find(rxData.m_Type)) != this->txDataList.end())
    {
        this->txDataList.erase(it);
    }
}

void CRecorderModule::write_rxData(const CAnalysisData &rxData)
{
    MutexLocker mLocker(rxMutex);(void)mLocker;
    this->rxDataList.push_back(rxData);
    while(rxDataList.size() > RX_DATA_MAX_SIZE)
    {
        rxDataList.pop_front();
    }
}

void CRecorderModule::write_txData(const CAnalysisData &txData)
{
    MutexLocker mLocker(txMutex);(void)mLocker;
    txDataList[txData.m_Type] = std::pair<CAnalysisData, unsigned long long>(txData, 0);
    while(txDataList.size() > TX_DATA_MAX_SIZE)
    {
        txDataList.erase(txDataList.begin());
    }
}


void CRecorderModule::doUpdate(const CAnalysisData &data)
{
    if (NULL != this->m_pUpdateModule)
    {
        this->m_pUpdateModule->doUpdateProcess(data);
    }
}

int CRecorderModule::initUpdate(const UINT8 &target, const QString &str)
{
    int ret = -1;
    if (NULL != this->m_pUpdateModule)
    {
        ret = this->m_pUpdateModule->initApplication(target, str);
        if (0 == ret)
        {
            this->m_pUpdateModule->sendUpdateTag();
        }
    }
    return ret;
}
