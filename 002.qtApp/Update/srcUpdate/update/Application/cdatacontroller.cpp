#include "../Utilities/includes.h"
#include "../UI/mainwindow.h"
#include "cdatacontroller.h"
#include "cserialcontrolthread.h"
#include "cupdatefile.h"

static bool bUpdateWorkingFlag = false;

CDataController::CDataController(MainWindow *pMainWindow)
    :m_pMainWindow(pMainWindow)
    ,m_AIOModule(&m_SerialControl)
    ,m_KeyModule(&m_SerialControl)
    ,m_BckModule(&m_SerialControl)
    ,m_RecorderModule(&m_SerialControl)
    ,m_SerialControl(&m_AIOModule, &m_KeyModule, &m_BckModule, &m_RecorderModule)
    ,m_bWaitUpdateEnd(false)
{
    m_pSerialThread = new CSerialControlThread(&m_SerialControl);
    m_pUpdateFile = new CUpdateFile();

    assert(m_pSerialThread);
    assert(m_pUpdateFile);
}

CDataController::~CDataController()
{
    if (NULL != m_pSerialThread)
    {
        delete m_pSerialThread;
        m_pSerialThread = NULL;
    }
    if (NULL != m_pUpdateFile)
    {
        delete m_pUpdateFile;
        m_pUpdateFile = NULL;
    }
}


void CDataController::startThread(void)
{
    if(m_pSerialThread)m_pSerialThread->run(SCHED_RR, 2);
}

void CDataController::stopThread(void)
{
    if(m_pSerialThread)m_pSerialThread->shutdown();
}

void CDataController::process(const unsigned long long &sysTick,
                              const unsigned long long &runTimes)
{
    static unsigned int dataIndex = 0;
    unsigned long long count = sysTick/1000;
    unsigned long long tmp = runTimes;
    tmp = tmp;

    this->process_module_ms(sysTick);

    for(; dataIndex < count; dataIndex++)
    {
        this->process_module_s(sysTick);
    }
}

void CDataController::process_module_ms(const unsigned long long &sysTick)
{
    m_AIOModule.process_ms(sysTick);
    m_KeyModule.process_ms(sysTick);
    m_BckModule.process_ms(sysTick);
    m_RecorderModule.process_ms(sysTick);
}

void CDataController::process_module_s(const unsigned long long &sysTick)
{
    UPDATE_TARGET_TAG tag;
    QString str;
    QString print;
    QByteArray tmp;
    int count = 0;

    m_AIOModule.process_s(sysTick);
    m_KeyModule.process_s(sysTick);
    m_BckModule.process_s(sysTick);
    m_RecorderModule.process_s(sysTick);

    if ((NULL != m_pUpdateFile) \
            && (false == bUpdateWorkingFlag) \
            && m_pUpdateFile->isNeed2Hanle())
    {
        count = m_pUpdateFile->getOneMission(tag, str);
        if (count > 0)
        {
            print.sprintf(">>====================================");
            CMyConsole::addStr2Console(print);
            print.sprintf(">>Mission :%s",CUtilities::QString2Char(str, tmp));
            CMyConsole::addStr2Console(print);
            setUpdateProcessBar(count);
            this->doUpdate(tag, str);
            m_pUpdateFile->delOneMission(tag);
            if (0 == count-1) // last bin file
            {
                m_bWaitUpdateEnd = true;
            }
        }
    }

    if ((true == m_bWaitUpdateEnd) && (false == bUpdateWorkingFlag))
    {
        m_bWaitUpdateEnd = false;
        print.sprintf(">>====================================");
        CMyConsole::addStr2Console(print);
        print.sprintf(">>Mission :End! Ready to reset!!!");
        CMyConsole::addStr2Console(print);
        this->setRecorderPower(false);
        this->resetSystem();
    }
}

void CDataController::getEmbeddedVersion(void)
{
    CAnalysisData data;
    data.m_IsNeedACK = true;
    data.m_DstAddr = AIO_ADDR;
    data.m_Type = CSerialAnalysis::AIO_VERSION_ID;
    data.m_Len = 0x00;
    send_txData(data);
}


void CDataController::setRecorderPower(bool state)
{
    CAnalysisData data;
    data.m_IsNeedACK = true;
    data.m_DstAddr = BCK_ADDR;
    data.m_Type = CSerialAnalysis::BCK_ENABLED_RECORDER_ID;
    data.m_Len = 0x01;

    if (state)
    {
        data.m_Data[0] = 0x01;
    }
    else
    {
        data.m_Data[0] = 0x00;
    }
    send_txData(data);
}

void CDataController::send_txData(const CAnalysisData &txData)
{
    switch (txData.m_DstAddr)
    {
        case AIO_ADDR:
            m_AIOModule.write_txData(txData);
            break;
        case KEY_ADDR:
            m_KeyModule.write_txData(txData);
            break;
        case BCK_ADDR:
            m_BckModule.write_txData(txData);
            break;
        case RECORDER_ADDR:
            m_RecorderModule.write_txData(txData);
            break;
        default:
            break;
    }
}

int CDataController::runUdiskUpdate(void)
{
    this->setRecorderPower(true);
    if (NULL != m_pUpdateFile)
    {
        return m_pUpdateFile->initApplication();
    }
    return -1;
}

int CDataController::doUpdate(const UPDATE_TARGET_TAG &Tag,
                              const QString &binFileName)
{
    int ret = -1;
    switch(Tag)
    {
    case UPDATE_TARGET_TAG_SPO2:
    {
        ret = m_AIOModule.initUpdate(CSerialAnalysis::SF_SPO2_UPDATE, binFileName);
        if (0 == ret)
        {
            setUpdateWorking();
        }
    }
        break;
    case UPDATE_TARGET_TAG_AIOSTM:
    {
        ret = m_AIOModule.initUpdate(CSerialAnalysis::SF_AIO_STM_UPDATE, binFileName);
        if (0 == ret)
        {
            setUpdateWorking();
        }
    }
        break;
    case UPDATE_TARGET_TAG_AIODSP:
    {
        ret = m_AIOModule.initUpdate(CSerialAnalysis::SF_AIO_DSP_UPDATE, binFileName);
        if (0 == ret)
        {
            setUpdateWorking();
        }
    }
        break;
    case UPDATE_TARGET_TAG_RECODER:
    {
        ret = m_RecorderModule.initUpdate(CSerialAnalysis::SF_RECORD_UPDATE, binFileName);
        if (0 == ret)
        {
            setUpdateWorking();
        }
    }
        break;
    case UPDATE_TARGET_TAG_BACK:
    {
        ret = m_BckModule.initUpdate(CSerialAnalysis::SF_BACK_UPDATE, binFileName);
        if (0 == ret)
        {
            setUpdateWorking();
        }
    }
        break;
    case UPDATE_TARGET_TAG_MLO:
    {
        setUpdateWorking();
        this->doUpdateMLO(binFileName);
        resetUpdateWorking();
    }
        break;
    case UPDATE_TARGET_TAG_UBOOT:
    {
        setUpdateWorking();
        this->doUpdateUboot(binFileName);
        resetUpdateWorking();
    }
        break;
    case UPDATE_TARGET_TAG_PATIENTMONITOR:
    {
        setUpdateWorking();
        this->doUpdatePatientMonitor(binFileName);
        resetUpdateWorking();
    }
        break;
    case UPDATE_TARGET_TAG_UPDATE_APP:
    {
        setUpdateWorking();
        this->doUpdateApp(binFileName);
        resetUpdateWorking();
    }
        break;
    case UPDATE_TARGET_TAG_KERNEL:
    {
        setUpdateWorking();
        this->doUpdateKernel(binFileName);
        resetUpdateWorking();
    }
        break;
    case UPDATE_TARGET_TAG_ROOTFS:
    {
        setUpdateWorking();
        this->doUpdateRootfs(binFileName);
        resetUpdateWorking();
    }
        break;
    default:
        break;
    }
    return 1;
}


void CDataController::setUpdateWorking(void)
{
    bUpdateWorkingFlag = true;
}

void CDataController::resetUpdateWorking(void)
{
    bUpdateWorkingFlag = false;
}

int CDataController::doUpdateMLO(const QString &binFileName)
{
    QString print;
    int ret = -1;
    UINT32 fileLength = 0;
    QByteArray tmp;
    char erase_cmd[50];
    char write_cmd[100];

    memset(erase_cmd, 0x00, sizeof(erase_cmd));
    memset(write_cmd, 0x00, sizeof(write_cmd));
    ret = CFolderFile::getFileLength(binFileName, fileLength);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateMLO getFileLength ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    CMyConsole::refreshPrecessBar(20);

    if (fileLength > AM335X_MLO_LENGTH_MAX)
    {
        print.sprintf(">>doUpdateMLO Length ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    CMyConsole::refreshPrecessBar(40);
    sprintf(erase_cmd, "mtd_debug erase /dev/mtd0 0 %d",AM335X_MLO_LENGTH_MAX);
    ret = system(erase_cmd);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateMLO erase ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }
    else
    {
        print.sprintf(">>doUpdateMLO erase OK");
        CMyConsole::addStr2Console(print);
    }

    CMyConsole::refreshPrecessBar(80);
    sprintf(write_cmd,
            "mtd_debug write /dev/mtd0 0 %d %s",
            fileLength,
            CUtilities::QString2Char(binFileName, tmp));
    ret = system(write_cmd);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateMLO write ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    print.sprintf(">>doUpdateMLO write OK");
    CMyConsole::addStr2Console(print);
    CMyConsole::refreshPrecessBar(100);
    return 0;
}

int CDataController::doUpdateUboot(const QString &binFileName)
{
    QString print;
    int ret = -1;
    UINT32 fileLength = 0;
    QByteArray tmp;
    char erase_cmd[50];
    char write_cmd[100];
    char erase_env_cmd[] = "flash_erase /dev/mtd2 0 0";

    memset(erase_cmd, 0x00, sizeof(erase_cmd));
    memset(write_cmd, 0x00, sizeof(write_cmd));

    ret = CFolderFile::getFileLength(binFileName, fileLength);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateUboot getFileLength ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    CMyConsole::refreshPrecessBar(20);

    if (fileLength > AM335X_UBOOT_LENGTH_MAX)
    {
        print.sprintf(">>doUpdateUboot Length ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    CMyConsole::refreshPrecessBar(40);
    sprintf(erase_cmd, "mtd_debug erase /dev/mtd1 0 %d",AM335X_UBOOT_LENGTH_MAX);
    ret = system(erase_cmd);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateUboot erase ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }
    else
    {
        print.sprintf(">>doUpdateUboot erase OK");
        CMyConsole::addStr2Console(print);
    }

    CMyConsole::refreshPrecessBar(80);
    sprintf(write_cmd,
            "mtd_debug write /dev/mtd1 0 %d %s",
            fileLength,
            CUtilities::QString2Char(binFileName, tmp));
    ret = system(write_cmd);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateUboot write ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    print.sprintf(">>doUpdateUboot write OK");
    CMyConsole::addStr2Console(print);
    CMyConsole::refreshPrecessBar(100);

    ret = system(erase_env_cmd);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateUboot erase_env_cmd ERROR");
    }
    print.sprintf(">>doUpdateUboot erase_env_cmd OK");
    CMyConsole::addStr2Console(print);

    return 0;
}

int CDataController::doUpdateKernel(const QString &binFileName)
{
    QString print;
    int ret = -1;
    UINT32 fileLength = 0;
    QByteArray tmp;
    char erase_cmd[] = "flash_erase /dev/mtd4 0 0";
    char write_cmd[100];

    memset(write_cmd, 0x00, sizeof(write_cmd));

    ret = CFolderFile::getFileLength(binFileName, fileLength);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateKernel getFileLength ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    CMyConsole::refreshPrecessBar(20);

    if (fileLength > AM335X_KERNEL_LENGTH_MAX)
    {
        print.sprintf(">>doUpdateKernel Length ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    CMyConsole::refreshPrecessBar(40);
    ret = system(erase_cmd);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateKernel erase ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }
    else
    {
        print.sprintf(">>doUpdateKernel erase OK");
        CMyConsole::addStr2Console(print);
    }

    CMyConsole::refreshPrecessBar(80);
    sprintf(write_cmd,
            "nandwrite -p /dev/mtd4 %s",
            CUtilities::QString2Char(binFileName, tmp));
    ret = system(write_cmd);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateKernel write ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    print.sprintf(">>doUpdateKernel write OK");
    CMyConsole::addStr2Console(print);
    CMyConsole::refreshPrecessBar(100);
    return 0;
}

int CDataController::doUpdateRootfs(const QString &binFileName)
{
    QString print;
    int ret = -1;
    UINT32 fileLength = 0;
    QByteArray tmp;
    char erase_cmd[] = "/tmp/updatefile/util/flash_erase /dev/mtd5 0 0";
    char write_cmd[150];

    memset(write_cmd, 0x00, sizeof(write_cmd));

    ret = CFolderFile::getFileLength(binFileName, fileLength);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateRootfs getFileLength ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    CMyConsole::refreshPrecessBar(20);

    if (fileLength > AM335X_ROOTFS_LENGTH_MAX)
    {
        print.sprintf(">>doUpdateRootfs Length ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    CMyConsole::refreshPrecessBar(40);
    ret = system(erase_cmd);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateRootfs erase ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }
    else
    {
        print.sprintf(">>doUpdateRootfs erase OK");
        CMyConsole::addStr2Console(print);
    }

    CMyConsole::refreshPrecessBar(80);
    sprintf(write_cmd,
            "/tmp/updatefile/util/nandwrite -p /dev/mtd5 %s",
            CUtilities::QString2Char(binFileName, tmp));
    ret = system(write_cmd);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateRootfs write ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    print.sprintf(">>doUpdateRootfs write OK");
    CMyConsole::addStr2Console(print);
    CMyConsole::refreshPrecessBar(100);
    return 0;
}

int CDataController::doUpdatePatientMonitor(const QString &binFileName)
{
    QString print;
    int ret = -1;
    QByteArray tmp;

    const char rm_cmd1[] = "rm -fr /mnt/userfs/MonitorConfig";
    const char rm_cmd2[] = "rm -f /mnt/userfs/Monitor";
    const char rm_cmd3[] = "rm -f /mnt/userfs/PatientMonitor";
//    const char rm_cmd4[] = "rm -f /mnt/userfs/updateTime";
    const char sync_cmd[] = "sync";
    const char chmod_cmd[] = "chmod 777 /mnt/userfs/* -R";
    char tar_cmd[150];

    memset(tar_cmd, 0x00, sizeof(tar_cmd));

    CMyConsole::refreshPrecessBar(10);
    system(rm_cmd1);
    system(rm_cmd2);
    system(rm_cmd3);
//    system(rm_cmd4);

    CMyConsole::refreshPrecessBar(20);
    sprintf(tar_cmd,
            "tar xzf %s -C /mnt/userfs/",
            CUtilities::QString2Char(binFileName, tmp));
    ret = system(tar_cmd);
    if (0 != ret)
    {
        print.sprintf(">>doUpdatePatientMonitor tar xzvf ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    CMyConsole::refreshPrecessBar(60);
    print.sprintf(">>doUpdatePatientMonitor tar xzvf OK");
    CMyConsole::addStr2Console(print);

    system(chmod_cmd);
    CMyConsole::refreshPrecessBar(80);

    system(sync_cmd);
    CMyConsole::refreshPrecessBar(100);

    return 0;
}

int CDataController::doUpdateApp(const QString &binFileName)
{
    QString print;
    int ret = -1;
    QByteArray tmp;

    const char rm_cmd[] = "rm -f /mnt/userfs/update";
    const char sync_cmd[] = "sync";
    const char chmod_cmd[] = "chmod 777 /mnt/userfs/update";
    char mv_cmd[150];

    memset(mv_cmd, 0x00, sizeof(mv_cmd));

    system(rm_cmd);
    CMyConsole::refreshPrecessBar(10);

    sprintf(mv_cmd,
            "mv -f %s /mnt/userfs/update",
            CUtilities::QString2Char(binFileName, tmp));
    ret = system(mv_cmd);
    CMyConsole::refreshPrecessBar(20);
    if (0 != ret)
    {
        print.sprintf(">>doUpdateApp mv ERROR");
        CMyConsole::addStr2Console(print);
        return -1;
    }

    CMyConsole::refreshPrecessBar(60);
    print.sprintf(">>doUpdateApp mv OK");
    CMyConsole::addStr2Console(print);

    system(chmod_cmd);
    CMyConsole::refreshPrecessBar(80);

    system(sync_cmd);
    CMyConsole::refreshPrecessBar(100);

    return 0;
}

int CDataController::setUpdateProcessBar(int count2do)
{
    int total;

    assert(m_pMainWindow);

    total = m_pUpdateFile->getTotalMission();
    m_pMainWindow->setProcessBar(total, count2do);
    return 0;
}

int CDataController::resetSystem(void)
{
    CAnalysisData txData;
    txData.m_IsNeedACK = true;
    txData.m_SrcAddr = MPU_ADDR;
    txData.m_DstAddr = BCK_ADDR;
    txData.m_SN = 0;
    txData.m_Type = CSerialAnalysis::BCK_SYSTEM_RESET_ID;
    txData.m_Len = 1;
    txData.m_Data[0] = 0x07;

    sleep(2);
    m_BckModule.write_txData(txData);
    return 0;
}
