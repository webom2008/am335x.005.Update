#include "../Utilities/includes.h"
#include "cserialbckanalysis.h"


CSerialBckAnalysis::CSerialBckAnalysis()
    :CSerialAnalysis()
    ,bckCommSysTick(0)
{
}

CSerialBckAnalysis::~CSerialBckAnalysis()
{
}

void CSerialBckAnalysis::processCommStatus(const unsigned long long &sysTick)
{
    unsigned long long tmp = sysTick;
    tmp = tmp;
//    if(sysTick >= bckCommSysTick+COMM_FAULT_INTERVAL && NULL != pDataModel)
//    {
//        pDataModel->write_raw_alarm(AlarmBase::TCH_BCK_COMM_ERROR_ID);
//    }
}

void CSerialBckAnalysis::try_Parse(CModuleBase *pModule,
                                   const CAnalysisData &rxData,
                                   const unsigned long long &sysTick)
{
    QString info;

    if(this->try_Parse_Bck(pModule, rxData))
    {
        bckCommSysTick = sysTick;
    }
    else
    {
        info.sprintf("Back:ID=0X%02X",rxData.m_Type);
//        CMyConsole::addStr2Console(info);
    }
}

bool CSerialBckAnalysis::try_Parse_Bck(CModuleBase *pModule,
                                       const CAnalysisData &rxData)
{
    if(NULL == pModule)
    {
        return false;
    }

    switch (rxData.m_Type) {
    case BCK_ENABLED_RECORDER_ID:
    {
        pModule->delete_txData(rxData);
    }
        break;
    case SF_BACK_UPDATE:
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
