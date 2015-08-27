#ifndef CSERIALBCKANALYSIS_H
#define CSERIALBCKANALYSIS_H

#include "../Utilities/canalysisdata.h"
#include "../Devices/cserialanalysis.h"

class CModuleBase;

class CSerialBckAnalysis : public CSerialAnalysis
{
public:
    CSerialBckAnalysis();
    ~CSerialBckAnalysis();

public:
    virtual void processCommStatus(const unsigned long long &sysTick);
    virtual void try_Parse(CModuleBase *pModule,
                           const CAnalysisData &rxData,
                           const unsigned long long &sysTick);

private:
    bool try_Parse_Bck(CModuleBase *pModule,
                       const CAnalysisData &rxData);

private:
    unsigned long long bckCommSysTick;
};

#endif // CSERIALBCKANALYSIS_H
