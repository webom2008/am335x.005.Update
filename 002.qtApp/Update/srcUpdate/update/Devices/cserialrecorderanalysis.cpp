#include "../Utilities/includes.h"
#include "cserialrecorderanalysis.h"
#include "crecordermodule.h"

CSerialRecorderAnalysis::CSerialRecorderAnalysis()
    :CSerialAnalysis()
    ,recorderCommSysTick(0)
{
}

CSerialRecorderAnalysis::~CSerialRecorderAnalysis()
{
}

void CSerialRecorderAnalysis::processCommStatus(const unsigned long long &sysTick)
{
    unsigned long long tmp = sysTick;
    tmp = tmp;
//    if(sysTick >= recorderCommSysTick+COMM_FAULT_INTERVAL && NULL != pDataModel)
//    {
//        pDataModel->write_raw_alarm(AlarmBase::TCH_RECORDER_COMM_ERROR_ID);
//    }
}

void CSerialRecorderAnalysis::try_Parse(CModuleBase *pModule,
                                        const CAnalysisData &rxData,
                                        const unsigned long long &sysTick)
{
    QString info;

    if(this->try_Parse_Recorder(pModule, rxData))
    {
        recorderCommSysTick = sysTick;
    }
    else
    {
        info.sprintf("Recorder:ID=0X%02X",rxData.m_Type);
//        CMyConsole::addStr2Console(info);
    }
}

bool CSerialRecorderAnalysis::try_Parse_Recorder(CModuleBase *pModule,
                                                 const CAnalysisData &rxData)
{
    CRecorderModule *pRecorderModule = dynamic_cast<CRecorderModule *>(pModule);
    if(NULL == pRecorderModule)
    {
        return false;
    }
    switch (rxData.m_Type)
    {
    case SF_RECORD_UPDATE:
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
