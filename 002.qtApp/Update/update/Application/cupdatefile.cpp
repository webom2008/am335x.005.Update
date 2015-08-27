#include "cupdatefile.h"

const char UDISK_NAME_FILTER[]          = "sd*";
const char AIODSP_NAME_FILTER[]         = "aPM12_AIO_DSPAPP_*.ldr";
const char AIOSTM_NAME_FILTER[]         = "aPM12_AIO_STMAPP_*.bin";
const char SPO2_NAME_FILTER[]           = "aPM12_SpO2_APP_*.bin";
const char REC_NAME_FILTER[]            = "AES_aPM12_REC_APP_*.bin";
const char PWR_NAME_FILTER[]            = "AES_aPM12_PWR_APP_*.bin";
const char MLO_NAME_FILTER[]            = "MLO.byteswap*";
const char UBOOT_NAME_FILTER[]          = "u-boot*.img";
const char KERNEL_NAME_FILTER[]         = "uImage*";
const char ROOTFS_NAME_FILTER[]         = "rootfs*.img";
const char PATIENTMONITOR_NAME_FILTER[] = "aPM12_MPU_3352_*.tar.gz";
const char UPDATE_APP_NAME_FILTER[]     = "update*";

const char UPDATE_FILE_NAME_FILTER[]    = "updatefile*.tar.gz";
const char UPDATE_BIN_PATH[]            = "/tmp/updatefile";
#ifdef CONFIG_TARGET_FOR_PC
const char UDISK_MOUNT_PATH[] = "/home/ubuntu/tmp/";
#else
const char UDISK_MOUNT_PATH[] = "/media/";
#endif

CUpdateFile::CUpdateFile()
    :m_bInitFlag(false)
{
}

CUpdateFile::~CUpdateFile()
{
}


int CUpdateFile::initApplication(void)
{
    int ret = -1;
    QString print;
    bool bFlag = false;

    m_MissionCount = 0;

//    print.sprintf(">>Detect Udisk Mount ==========");
    bFlag = detectUdisk();
    if (bFlag)
    {
        print.sprintf(">>detect Udisk OK");
        CMyConsole::addStr2Console(print);
    }
    else
    {
        print.sprintf(">>detect Udisk ERROR");
        CMyConsole::addStr2Console(print);
        return ret;
    }

//    print.sprintf(">>Detect *.tar.gz file ==========");
    bFlag = detectTarGzFile();
    if (bFlag)
    {
        print = ">>detect " +m_UpdateFilePath + " OK";
        CMyConsole::addStr2Console(print);
    }
    else
    {
        print.sprintf(">>detect %s ERROR",UPDATE_FILE_NAME_FILTER);
        CMyConsole::addStr2Console(print);
        return ret;
    }

//    print.sprintf(">>tar xzvf *.tar.gz file ==========");
    bFlag = do_unTarFile();
    if (bFlag)
    {
        print.sprintf(">>do_unTarFile OK");
        CMyConsole::addStr2Console(print);
    }
    else
    {
        print.sprintf(">>do_unTarFile ERROR");
        CMyConsole::addStr2Console(print);
        return ret;
    }

//    print.sprintf(">>get bin file name ==========");
    m_MissionCount = accessAndAddFilePath();
    if (m_MissionCount > 0)
    {
        print.sprintf(">>MissionCount =%d", m_MissionCount);
        CMyConsole::addStr2Console(print);
        m_bInitFlag = true;
        return 0;
    }
    return ret;
}


int CUpdateFile::getTotalMission(void)
{
    return m_MissionCount;
}

bool CUpdateFile::detectUdisk(void)
{
    QString str;
    QString nameFilters;
    QString info;

    m_strListUdiskPath.clear();
    str = UDISK_MOUNT_PATH;
    nameFilters = UDISK_NAME_FILTER;
    CFolderFile::getDirs(str, nameFilters, m_strListUdiskPath);

    for (int i = 0; i < m_strListUdiskPath.size(); i++)
    {
        info = m_strListUdiskPath.at(i);
//        CMyConsole::addStr2Console(info);
    }

    if (m_strListUdiskPath.size() > 0)
    {
        return true;
    }

    return false;
}

bool CUpdateFile::detectTarGzFile(void)
{
    QString str;
    QString nameFilters;
    QStringList names;
    QString print;
    int i = 0;
    int ret = 0;

    for (i = 0; i < m_strListUdiskPath.size(); i++)
    {
        str = m_strListUdiskPath.at(i);
        nameFilters = UPDATE_FILE_NAME_FILTER;
        ret = CFolderFile::getFiles(    m_strListUdiskPath.at(i),
                                        nameFilters,
                                        names);
        if (ret > 0)
        {
            m_UpdateFilePath = names.at(0); //only detect fisrt one
            print = ">>" + m_UpdateFilePath;
            CMyConsole::addStr2Console(print);
            return true;
        }
    }

    return false;
}

bool CUpdateFile::do_unTarFile(void)
{
    char *name = NULL;
    char command[100];
    QByteArray byteArray;
    QString print;
    int ret = -1;

    name = CUtilities::QString2Char(m_UpdateFilePath, byteArray);
    memset(command, 0x00, sizeof(command));
    strcat(command, "tar xzf ");
    strcat(command, name);
    strcat(command, " -C /tmp/");
    ret = system(command);

    if (0 != ret)
    {
        print.sprintf("%s",command);
        print = ">>" + print + "ERROR";
        CMyConsole::addStr2Console(print);
        return false;
    }
    return true;
}


int  CUpdateFile::accessAndAddFilePath(void)
{
    QString print;
    QString nameFilters;
    QString path;
    QString bin_name;
    QStringList names;
    int file_count = 0;
    int ret = 0;

    this->m_MapTargetFilePath.clear();
    CUtilities::Char2QString(path, UPDATE_BIN_PATH);

    //1.SPO2 bin detect
    CUtilities::Char2QString(nameFilters, SPO2_NAME_FILTER);
    ret = CFolderFile::getFiles(    path,
                                    nameFilters,
                                    names);
    if (ret > 0)
    {
        bin_name = names.at(0); //only detect fisrt one
        this->m_MapTargetFilePath[UPDATE_TARGET_TAG_SPO2] = bin_name;
        print = ">>" + bin_name;
        CMyConsole::addStr2Console(print);
        file_count++;
    }

    //2.AIOSTM bin detect
    CUtilities::Char2QString(nameFilters, AIOSTM_NAME_FILTER);
    ret = CFolderFile::getFiles(    path,
                                    nameFilters,
                                    names);
    if (ret > 0)
    {
        bin_name = names.at(0); //only detect fisrt one
        this->m_MapTargetFilePath[UPDATE_TARGET_TAG_AIOSTM] = bin_name;
        print = ">>" + bin_name;
        CMyConsole::addStr2Console(print);
        file_count++;
    }

    //3.AIODSP bin detect
    CUtilities::Char2QString(nameFilters, AIODSP_NAME_FILTER);
    ret = CFolderFile::getFiles(    path,
                                    nameFilters,
                                    names);
    if (ret > 0)
    {
        bin_name = names.at(0); //only detect fisrt one
        this->m_MapTargetFilePath[UPDATE_TARGET_TAG_AIODSP] = bin_name;
        print = ">>" + bin_name;
        CMyConsole::addStr2Console(print);
        file_count++;
    }

    //4.Recoder bin detect
    CUtilities::Char2QString(nameFilters, REC_NAME_FILTER);
    ret = CFolderFile::getFiles(    path,
                                    nameFilters,
                                    names);
    if (ret > 0)
    {
        bin_name = names.at(0); //only detect fisrt one
        this->m_MapTargetFilePath[UPDATE_TARGET_TAG_RECODER] = bin_name;
        print = ">>" + bin_name;
        CMyConsole::addStr2Console(print);
        file_count++;
    }

    //5.Backboard bin detect
    CUtilities::Char2QString(nameFilters, PWR_NAME_FILTER);
    ret = CFolderFile::getFiles(    path,
                                    nameFilters,
                                    names);
    if (ret > 0)
    {
        bin_name = names.at(0); //only detect fisrt one
        this->m_MapTargetFilePath[UPDATE_TARGET_TAG_BACK] = bin_name;
        print = ">>" + bin_name;
        CMyConsole::addStr2Console(print);
        file_count++;
    }

    //6.MLO bin detect
    CUtilities::Char2QString(nameFilters, MLO_NAME_FILTER);
    ret = CFolderFile::getFiles(    path,
                                    nameFilters,
                                    names);
    if (ret > 0)
    {
        bin_name = names.at(0); //only detect fisrt one
        this->m_MapTargetFilePath[UPDATE_TARGET_TAG_MLO] = bin_name;
        print = ">>" + bin_name;
        CMyConsole::addStr2Console(print);
        file_count++;
    }

    //7.UBoot bin detect
    CUtilities::Char2QString(nameFilters, UBOOT_NAME_FILTER);
    ret = CFolderFile::getFiles(    path,
                                    nameFilters,
                                    names);
    if (ret > 0)
    {
        bin_name = names.at(0); //only detect fisrt one
        this->m_MapTargetFilePath[UPDATE_TARGET_TAG_UBOOT] = bin_name;
        print = ">>" + bin_name;
        CMyConsole::addStr2Console(print);
        file_count++;
    }

    //8.PatentMonitor *.tar.gz detect
    CUtilities::Char2QString(nameFilters, PATIENTMONITOR_NAME_FILTER);
    ret = CFolderFile::getFiles(    path,
                                    nameFilters,
                                    names);
    if (ret > 0)
    {
        bin_name = names.at(0); //only detect fisrt one
        this->m_MapTargetFilePath[UPDATE_TARGET_TAG_PATIENTMONITOR] = bin_name;
        print = ">>" + bin_name;
        CMyConsole::addStr2Console(print);
        file_count++;
    }

    //9.update app detect
    CUtilities::Char2QString(nameFilters, UPDATE_APP_NAME_FILTER);
    ret = CFolderFile::getFiles(    path,
                                    nameFilters,
                                    names);
    if (ret > 0)
    {
        bin_name = names.at(0); //only detect fisrt one
        this->m_MapTargetFilePath[UPDATE_TARGET_TAG_UPDATE_APP] = bin_name;
        print = ">>" + bin_name;
        CMyConsole::addStr2Console(print);
        file_count++;
    }

    //10.Kernel bin detect
    CUtilities::Char2QString(nameFilters, KERNEL_NAME_FILTER);
    ret = CFolderFile::getFiles(    path,
                                    nameFilters,
                                    names);
    if (ret > 0)
    {
        bin_name = names.at(0); //only detect fisrt one
        this->m_MapTargetFilePath[UPDATE_TARGET_TAG_KERNEL] = bin_name;
        print = ">>" + bin_name;
        CMyConsole::addStr2Console(print);
        file_count++;
    }

    //11.Rootfs bin detect
    CUtilities::Char2QString(nameFilters, ROOTFS_NAME_FILTER);
    ret = CFolderFile::getFiles(    path,
                                    nameFilters,
                                    names);
    if (ret > 0)
    {
        bin_name = names.at(0); //only detect fisrt one
        this->m_MapTargetFilePath[UPDATE_TARGET_TAG_ROOTFS] = bin_name;
        print = ">>" + bin_name;
        CMyConsole::addStr2Console(print);
        file_count++;
    }

    return file_count;
}


int CUpdateFile::getOneMission(UPDATE_TARGET_TAG &Tag, QString &binFileName)
{
    int size = 0;
    std::map<UPDATE_TARGET_TAG, QString>::iterator it;

    size = this->m_MapTargetFilePath.size();
    if (size > 0)
    {
        it = this->m_MapTargetFilePath.begin();
        Tag = it->first;
        binFileName = it->second;
        return size;
    }
    return 0;
}

int CUpdateFile::delOneMission(const UPDATE_TARGET_TAG &Tag)
{
    QString str;
    QByteArray tmp;
    char command[100];
    int size = 0;

    size = this->m_MapTargetFilePath.size();
    if (size > 0)
    {
        //1.delete bin file
        str = this->m_MapTargetFilePath[Tag];
        memset(command, 0x00, sizeof(command));
        strcat(command, "rm -f ");
        strcat(command, CUtilities::QString2Char(str, tmp));
        system(command);

        //2.delete from map
        this->m_MapTargetFilePath.erase(Tag);
        return 1;
    }
    return 0;
}

bool CUpdateFile::isNeed2Hanle(void)
{
    if (m_bInitFlag && this->m_MapTargetFilePath.size() > 0)
    {
        return true;
    }
    return false;
}
