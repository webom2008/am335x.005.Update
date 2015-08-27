#ifndef CMODULEBASE_H
#define CMODULEBASE_H

#include <QString>
#include "../Utilities/canalysisdata.h"

#define RX_DATA_MAX_SIZE        200
#define TX_DATA_MAX_SIZE        200

class CSerialControl;
class CSerialAnalysis;
class CUpdateModule;

class CModuleBase
{
public:
    CModuleBase(CSerialControl *pSerialControl,
                CSerialAnalysis *pAnalysis);
    ~CModuleBase();

public:
    virtual void process_ms(const unsigned long long &sysTick) = 0;
    virtual void process_s(const unsigned long long &sysTick) = 0;
    virtual void write_rxData(const CAnalysisData &rxData) = 0;
    virtual void write_txData(const CAnalysisData &txData) = 0;
    virtual void delete_txData(const CAnalysisData &rxData) = 0;
    virtual void doUpdate(const CAnalysisData &data) = 0;
    virtual int initUpdate(const UINT8 &target, const QString &str) = 0;

protected:
    virtual void processTxDataList(const unsigned long long &sysTick) = 0;
    virtual void processRxDataList(const unsigned long long &sysTick) = 0;
    virtual void processCommStatus(const unsigned long long &sysTick) = 0;

protected:
    CSerialControl  *m_pSerialControl;
    CSerialAnalysis *m_pAnalysis;
};

#endif // CMODULEBASE_H
