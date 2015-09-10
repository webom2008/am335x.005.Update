#ifndef CUPDATEFILE_H
#define CUPDATEFILE_H

#include <map>
#include <QString>
#include <QStringList>
#include "../Utilities/includes.h"

class CUpdateFile
{
public:
    CUpdateFile();
    ~CUpdateFile();

    int initApplication(void);

    int getTotalMission(void);
    int getOneMission(UPDATE_TARGET_TAG &Tag, QString &binFileName);
    int delOneMission(const UPDATE_TARGET_TAG &Tag);
    bool isNeed2Hanle(void);

private:
    bool detectUdisk(void);
    bool detectTarGzFile(void);
    bool do_unTarFile(void);
    int accessAndAddFilePath(void);

    bool                m_bInitFlag;
    QStringList         m_strListUdiskPath;
    QString             m_UpdateFilePath;
    std::map<UPDATE_TARGET_TAG, QString> m_MapTargetFilePath;
    int                 m_MissionCount;
};

#endif // CUPDATEFILE_H
