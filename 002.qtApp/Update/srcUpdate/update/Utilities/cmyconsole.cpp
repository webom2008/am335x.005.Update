#include "../Utilities/includes.h"
#include "../UI/mainwindow.h"
#include "cmyconsole.h"

static MainWindow *m_pMainWindow = NULL;
static Mutex    mMutex;

CMyConsole::CMyConsole()
{

}

CMyConsole::~CMyConsole()
{

}


void CMyConsole::initConsole(MainWindow *pWin)
{
    assert(pWin);

    m_pMainWindow = pWin;
}

void CMyConsole::clearConsole(void)
{
    MutexLocker mLocker(mMutex);(void)mLocker;
    if (NULL != m_pMainWindow)
    {
        m_pMainWindow->clearDisplay();
    }
}

void CMyConsole::addStr2Console(const QString &str)
{
    MutexLocker mLocker(mMutex);(void)mLocker;
    if (NULL != m_pMainWindow)
    {
        m_pMainWindow->addDisplay(str);
    }
}

void CMyConsole::refreshPrecessBar(int percent)
{
    if (NULL != m_pMainWindow)
    {
        m_pMainWindow->setProcessBar(percent);
    }
}
