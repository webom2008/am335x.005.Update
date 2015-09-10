#ifndef CSERIALCONTROL_H
#define CSERIALCONTROL_H

#include <map>

class FdSet;
class CAIOModule;
class CKeyModule;
class CBckModule;
class CRecorderModule;
class CSerialDevice;
class CAnalysisData;

class CSerialControl
{
public:
    CSerialControl(
            CAIOModule *pAIOModule,
            CKeyModule *pKeyModule,
            CBckModule *pBckModule,
            CRecorderModule *pRecorderModule);
    ~CSerialControl();

    void build_fdset(FdSet &fdSet);
    void process(FdSet &fdSet);
    void open();
    void close();

    void send_rxData(const CAnalysisData &rxData);
    bool send_txData(const CAnalysisData &txData);

private:
    CAIOModule          *m_pAIOModule;
    CKeyModule          *m_pKeyModule;
    CBckModule          *m_pBckModule;
    CRecorderModule     *m_pRecorderModule;

    std::map<unsigned char, CSerialDevice *> m_SerialDeviceList;
};

#endif // CSERIALCONTROL_H
