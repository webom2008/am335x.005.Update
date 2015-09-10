#ifndef CKEYMODULE_H
#define CKEYMODULE_H

#include <list>
#include <map>

#include "cmodulebase.h"
#include "../Utilities/canalysisdata.h"
#include "../Utilities/autolock.h"
#include "cserialkeyanalysis.h"


class CKeyModule : public CModuleBase
{
public:
    CKeyModule(CSerialControl *pSerialControl);
    ~CKeyModule();

public:
    virtual void process_ms(const unsigned long long &sysTick);
    virtual void process_s(const unsigned long long &sysTick);
    virtual void write_rxData(const CAnalysisData &rxData);
    virtual void write_txData(const CAnalysisData &txData);
    virtual void delete_txData(const CAnalysisData &rxData);
    virtual void doUpdate(const CAnalysisData &data);
    virtual int initUpdate(const UINT8 &target, const QString &str);

protected:
    virtual void processTxDataList(const unsigned long long &sysTick);
    virtual void processRxDataList(const unsigned long long &sysTick);
    virtual void processCommStatus(const unsigned long long &sysTick);

private:
    Mutex                       rxMutex;
    std::list<CAnalysisData>    rxDataList;
    Mutex                       txMutex;
    std::map<unsigned char, std::pair<CAnalysisData, unsigned long long> > txDataList;
    CSerialKeyAnalysis          m_SerialKeyAnalysis;
    CUpdateModule               *m_pUpdateModule;
};

#endif // CKEYMODULE_H
