#ifndef CFOLDERFILE_H
#define CFOLDERFILE_H

#include <QString>
#include <QDir>

#include "../Utilities/typedefs.h"

class CFolderFile
{
public:
    CFolderFile();
    ~CFolderFile();

    static int getDirs(const QString &path, QStringList &dirs);
    static int getDirs(const QString &path,
                        const QString &nameFilters,
                        QStringList &dirs);
    static int getFiles(const QString &path, QStringList &files);
    static int getFiles(const QString &path,
                        const QString &nameFilters,
                        QStringList &files);
    static int getFileLength(const QString &file, UINT32 &len);
    static void addFolderImages(const QString &path);
    static void addSubFolderImages(const QString &path);
};

#endif // CFOLDERFILE_H
