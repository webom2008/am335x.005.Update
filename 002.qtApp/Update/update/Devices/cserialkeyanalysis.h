#ifndef CSERIALKEYANALYSIS_H
#define CSERIALKEYANALYSIS_H

#include "../Utilities/canalysisdata.h"
#include "../Devices/cserialanalysis.h"

class CModuleBase;

class CSerialKeyAnalysis : public CSerialAnalysis
{
public:
    CSerialKeyAnalysis();
    ~CSerialKeyAnalysis();

public:
    virtual void processCommStatus(const unsigned long long &sysTick);
    virtual void try_Parse(CModuleBase *pModule,
                           const CAnalysisData &rxData,
                           const unsigned long long &sysTick);

private:
    bool try_Parse_Key(CModuleBase *pModule,
                       const CAnalysisData &rxData);

private:
    unsigned long long keyCommSysTick;
};

#endif // CSERIALKEYANALYSIS_H
