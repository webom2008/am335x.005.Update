#ifndef CSERIALRECORDERANALYSIS_H
#define CSERIALRECORDERANALYSIS_H

#include "../Utilities/canalysisdata.h"
#include "../Devices/cserialanalysis.h"

class CModuleBase;

class CSerialRecorderAnalysis : public CSerialAnalysis
{
public:
    CSerialRecorderAnalysis();
    ~CSerialRecorderAnalysis();

public:
    virtual void processCommStatus(const unsigned long long &sysTick);
    virtual void try_Parse(CModuleBase *pModule,
                           const CAnalysisData &rxData,
                           const unsigned long long &sysTick);

private:
    bool try_Parse_Recorder(CModuleBase *pModule,
                            const CAnalysisData &rxData);

private:
    unsigned long long recorderCommSysTick;
};

#endif // CSERIALRECORDERANALYSIS_H
