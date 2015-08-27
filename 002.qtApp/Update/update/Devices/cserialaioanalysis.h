#ifndef CSERIALAIOANALYSIS_H
#define CSERIALAIOANALYSIS_H

#include "../Utilities/canalysisdata.h"
#include "../Devices/cserialanalysis.h"

class CModuleBase;

class CSerialAIOAnalysis : public CSerialAnalysis
{
public:
    CSerialAIOAnalysis();
    ~CSerialAIOAnalysis();

public:
    virtual void processCommStatus(const unsigned long long &sysTick);
    virtual void try_Parse(CModuleBase *pModule,
                           const CAnalysisData &rxData,
                           const unsigned long long &sysTick);

private:
    bool try_Parse_AIO(CModuleBase *pModule,
                       const CAnalysisData &rxData);
    bool try_Parse_SPO2(CModuleBase *pModule,
                        const CAnalysisData &rxData);

private:
    unsigned long long ECGSysTime;
    unsigned long long RESPSysTime;
    unsigned long long SPO2SysTime;
    unsigned char ecgSN;
    unsigned char respSN;
    unsigned char spo2SN;
    unsigned char stSN;

    unsigned long long aioCommSysTick;
    unsigned long long spo2CommSysTick;
};

#endif // CSERIALAIOANALYSIS_H
