#include "../Utilities/includes.h"
#include "cserialkeyanalysis.h"

CSerialKeyAnalysis::CSerialKeyAnalysis()
    :CSerialAnalysis()
    ,keyCommSysTick(0)
{
}

CSerialKeyAnalysis::~CSerialKeyAnalysis()
{

}


void CSerialKeyAnalysis::processCommStatus(const unsigned long long &sysTick)
{
    unsigned long long tmp = sysTick;
    tmp = tmp;
//    if(sysTick >= keyCommSysTick+COMM_FAULT_INTERVAL && NULL != pDataModel)
//    {
//        pDataModel->write_raw_alarm(AlarmBase::TCH_KEY_COMM_ERROR_ID);
//    }
}

void CSerialKeyAnalysis::try_Parse(CModuleBase *pModule,
                                   const CAnalysisData &rxData,
                                   const unsigned long long &sysTick)
{
    QString info;

    if(this->try_Parse_Key(pModule, rxData))
    {
        keyCommSysTick = sysTick;
    }
    else
    {
        info.sprintf("Key:ID=0X%02X",rxData.m_Type);
//        CMyConsole::addStr2Console(info);
    }
}

bool CSerialKeyAnalysis::try_Parse_Key(CModuleBase *pModule,
                                       const CAnalysisData &rxData)
{
    if(NULL == pModule)
    {
        return false;
    }

    switch (rxData.m_Type) {
    case KEY_ALARM_LIGHT_ID:
        pModule->delete_txData(rxData);
        break;
    default:
        return false;
        break;
    }
    return true;
}
