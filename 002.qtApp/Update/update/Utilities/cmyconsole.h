#ifndef CMYCONSOLE_H
#define CMYCONSOLE_H

#include <QString>

class MainWindow;
class CMyConsole
{
public:
    CMyConsole();
    ~CMyConsole();

    static void initConsole(MainWindow *pWin);
    static void clearConsole(void);
    static void addStr2Console(const QString &str);
    static void refreshPrecessBar(int percent);
};

#endif // CMYCONSOLE_H
