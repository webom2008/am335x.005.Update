#ifndef CDATACONTROLLER_H
#define CDATACONTROLLER_H

#include "../Devices/cserialcontrol.h"
#include "../Devices/caiomodule.h"
#include "../Devices/ckeymodule.h"
#include "../Devices/cbckmodule.h"
#include "../Devices/crecordermodule.h"

class MainWindow;
class CSerialControl;
class CSerialControlThread;
class CUpdateFile;
class CDataController
{
public:
    CDataController(MainWindow *pMainWindow);
    ~CDataController();

    void startThread(void);
    void stopThread(void);

    //thread
    void process(const unsigned long long &sysTick,
                 const unsigned long long &runTimes);
    void process_module_ms(const unsigned long long &sysTick);
    void process_module_s(const unsigned long long &sysTick);

    int resetSystem(void);
    void getEmbeddedVersion(void);
    void send_txData(const CAnalysisData &txData);
    void setRecorderPower(bool state);

    int runUdiskUpdate(void);
    int doUpdate(const UPDATE_TARGET_TAG &Tag, const QString &binFileName);

    static void setUpdateWorking(void);
    static void resetUpdateWorking(void);
protected:
    int doUpdateMLO(const QString &binFileName);
    int doUpdateUboot(const QString &binFileName);
    int doUpdateKernel(const QString &binFileName);
    int doUpdateRootfs(const QString &binFileName);
    int doUpdatePatientMonitor(const QString &binFileName);
    int doUpdateApp(const QString &binFileName);

    int setUpdateProcessBar(int count2do);
private:
    MainWindow              *m_pMainWindow;

    CAIOModule              m_AIOModule;
    CKeyModule              m_KeyModule;
    CBckModule              m_BckModule;
    CRecorderModule         m_RecorderModule;

    CSerialControl          m_SerialControl;
    CSerialControlThread    *m_pSerialThread;
    CUpdateFile             *m_pUpdateFile;
    bool                    m_bWaitUpdateEnd;
};

#endif // CDATACONTROLLER_H
