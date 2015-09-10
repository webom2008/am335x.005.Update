#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include <QWSServer>
#include "UI/ui.h"
#include <Utilities/includes.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef CONFIG_TARGET_FOR_ARM
    QWSServer::setCursorVisible(false);
#endif
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QFile qssFile(":/style/style.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        QString qss;
        qss = QLatin1String(qssFile.readAll());
        qApp->setStyleSheet(qss);
        qssFile.close();
    }

    MainWindow w;
    w.initApplication();
    w.show();

    return a.exec();
}
