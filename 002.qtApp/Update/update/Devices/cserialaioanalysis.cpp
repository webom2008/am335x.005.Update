#include "../Utilities/includes.h"

#include "cserialaioanalysis.h"

CSerialAIOAnalysis::CSerialAIOAnalysis()
    :CSerialAnalysis()
    ,ECGSysTime(0)
    ,RESPSysTime(0)
    ,SPO2SysTime(0)
    ,aioCommSysTick(0)
    ,spo2CommSysTick(0)
{

}

CSerialAIOAnalysis::~CSerialAIOAnalysis()
{

}

void CSerialAIOAnalysis::processCommStatus(const unsigned long long &sysTick)
{
    if(sysTick >= spo2CommSysTick+COMM_FAULT_INTERVAL)
    {
//        CMyConsole::addStr2Console("No SPO2 detect!");
    }
    if(sysTick >= aioCommSysTick+COMM_FAULT_INTERVAL)
    {
//        CMyConsole::addStr2Console("No AIO detect!");
    }
}

void CSerialAIOAnalysis::try_Parse(CModuleBase *pModule,
                                   const CAnalysisData &rxData,
                                   const unsigned long long &sysTick)
{
    QString info;

    if(this->try_Parse_AIO(pModule,rxData))
    {
        aioCommSysTick = sysTick;
    }
    else if(this->try_Parse_SPO2(pModule,rxData))
    {
        spo2CommSysTick = sysTick;
    }
    else
    {
        info.sprintf("AIO:ID=0X%02X",rxData.m_Type);
//        CMyConsole::addStr2Console(info);
    }
}

bool CSerialAIOAnalysis::try_Parse_AIO(CModuleBase *pModule,
                                       const CAnalysisData &rxData)
{
    char *pChar;
    if(NULL == pModule)
    {
        return false;
    }
    switch (rxData.m_Type) {
    case AIO_VERSION_ID:
    {
        pChar = (char *)rxData.m_Data;
        CMyConsole::addStr2Console(pChar);
        pModule->delete_txData(rxData);
    }
        break;
    case SF_AIO_STM_UPDATE:
    case SF_AIO_DSP_UPDATE:
    {
        pModule->delete_txData(rxData);
        pModule->doUpdate(rxData);
    }
        break;
    default:
        return false;
        break;
    }
    return true;
}

bool CSerialAIOAnalysis::try_Parse_SPO2(CModuleBase *pModule,
                                        const CAnalysisData &rxData)
{
    if(NULL == pModule)
    {
        return false;
    }

    switch (rxData.m_Type) {
    case SPO2_VERSION_ID:
    {
        pModule->delete_txData(rxData);
    }
        break;
    case SF_SPO2_UPDATE:
    {
        pModule->delete_txData(rxData);
        pModule->doUpdate(rxData);
    }
        break;
    default:
        return false;
        break;
    }
    return true;
}
