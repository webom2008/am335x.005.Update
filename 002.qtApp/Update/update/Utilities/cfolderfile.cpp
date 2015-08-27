#include <QDirIterator>
#include <QDebug>

#include "includes.h"

CFolderFile::CFolderFile()
{
}

CFolderFile::~CFolderFile()
{
}


int CFolderFile::getDirs(const QString &path, QStringList &dirs)
{
    QString info;
    QDir dir(path);
    QFileInfoList list;
    int i = 0;

    if(!dir.exists())
    {
        return 0;
    }

    dir.setFilter(QDir::Dirs | QDir::NoSymLinks);
    list = dir.entryInfoList();
    for (i = 0; i < list.size(); i++)
    {
        info = list.at(i).absoluteFilePath();
        dirs.append(info);
        CMyConsole::addStr2Console(info);
    }
    return list.size();
}

int CFolderFile::getDirs(  const QString &path,
                            const QString &nameFilters,
                            QStringList &dirs)
{
    QString info;
    QDir directory = QDir(path);
    int i = 0;

    if(!directory.exists())
    {
        return 0;
    }

    dirs = directory.entryList(QStringList(nameFilters),
                               QDir::Dirs | QDir::NoSymLinks);
    for (i = 0; i < dirs.size(); i++)
    {
        info = dirs.at(i);
        info = path+info;
        dirs.replace(i,info);
//        CMyConsole::addStr2Console(info);
    }
    return dirs.size();
}

int CFolderFile::getFiles(const QString &path, QStringList &files)
{
    QString info;
    QDir dir(path);
    QFileInfoList list;
    int i = 0;

    if(!dir.exists())
    {
        return 0;
    }

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    list = dir.entryInfoList();
    for (i = 0; i < list.size(); i++)
    {
        info = list.at(i).absoluteFilePath();
        files.append(info);
        CMyConsole::addStr2Console(info);
    }
    return list.size();
}

int CFolderFile::getFiles(const QString &path,
                    const QString &nameFilters,
                    QStringList &files)
{
    QString info;
    QDir directory = QDir(path);
    QChar separator = QChar('/');
    int i = 0;

    if(!directory.exists())
    {
        return 0;
    }

    files = directory.entryList(QStringList(nameFilters),
                               QDir::Files | QDir::NoSymLinks);
    for (i = 0; i < files.size(); i++)
    {
        info = files.at(i);
        info = path + separator + info;
        files.replace(i,info);
    }
    return files.size();
}

int CFolderFile::getFileLength(const QString &file, UINT32 &len)
{
    FILE *fp = NULL;
    long read_len = 0;
    QByteArray tmp;
    char *pFilePath = NULL;
    QString print;

    pFilePath = CUtilities::QString2Char(file, tmp);

    fp = fopen(pFilePath, "rb");
    if (NULL == fp)
    {
        print.sprintf(">>getFileLength fopen %s ERROR",pFilePath);
        CMyConsole::addStr2Console(print);
        return -1;
    }

    if (fseek(fp, 0, SEEK_END) < 0)
    {
        print.sprintf(">>getFileLength fseek %s ERROR",pFilePath);
        CMyConsole::addStr2Console(print);
        fclose(fp);
        return -1;
    }

    read_len = ftell(fp);
    fclose(fp);
    if (read_len < 0)
    {
        print.sprintf(">>getFileLength ftell %s ERROR",pFilePath);
        CMyConsole::addStr2Console(print);
        return -1;
    }

    len = (UINT32)read_len;
    return 0;
}

void CFolderFile::addFolderImages(const QString &path)
{
    QString info;
    QDir dir(path);
    QFileInfoList list;

    if(!dir.exists())
    {
        return;
    }

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    list = dir.entryInfoList();

    int file_count = list.count();
    if(file_count <= 0)
    {
        return;
    }

    QStringList string_list;
    for(int i=0; i < list.size(); i++)
    {
        QFileInfo file_info = list.at(i);
        QString suffix = file_info.suffix();
        if(QString::compare(suffix, QString("png"), Qt::CaseInsensitive) == 0)
        {
            QString absolute_file_path = file_info.absoluteFilePath();
            string_list.append(absolute_file_path);
        }
    }
}

void CFolderFile::addSubFolderImages(const QString &path)
{
    //判断路径是否存在
    QDir dir(path);
    if(!dir.exists())
    {
        return;
    }

    //获取所选文件类型过滤器
    QStringList filters;
    filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.png")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");

    //定义迭代器并设置过滤器
    QDirIterator dir_iterator(path,
        filters,
        QDir::Files | QDir::NoSymLinks,
        QDirIterator::Subdirectories);
    QStringList string_list;
    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        QString absolute_file_path = file_info.absoluteFilePath();
        string_list.append(absolute_file_path);
    }
}
