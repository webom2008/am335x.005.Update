#ifndef CAIOMODULE_H
#define CAIOMODULE_H

#include <list>
#include <map>

#include "../Utilities/sysconfig.h"
#include "cmodulebase.h"
#include "../Utilities/canalysisdata.h"
#include "../Utilities/autolock.h"
#include "cserialaioanalysis.h"

#define RX_DATA_MAX_SIZE        200
#define TX_DATA_MAX_SIZE        200

class CSerialControl;

class CAIOModule : public CModuleBase
{
public:
    CAIOModule(CSerialControl *pSerialControl);
    ~CAIOModule();

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
    Mutex rxMutex;
    std::list<CAnalysisData> rxDataList;
    Mutex txMutex;
    std::map<unsigned char, std::pair<CAnalysisData, unsigned long long> > txDataList;

    CSerialAIOAnalysis      m_SerialAIOAnalysis;
    CUpdateModule           *m_pUpdateModule;
};

#endif // CAIOMODULE_H
