#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Application/cinterrupt.h"
#include "../Utilities/csystemdatetime.h"

const int PROCESS_BAR_PERCENT_MAX = 1000;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pDataController(NULL),
    m_pInterrupt(NULL),
    m_ProcessBarTotal(1),
    m_ProcessBarLeft(0),
    m_bConfirmEnable(false)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::WindowTitleHint);

    CMyConsole::initConsole(this);

    m_pDataController   = new CDataController(this);
    m_pInterrupt        = new CInterrupt(this);


    ui->updateProgressBar->setMaximum(PROCESS_BAR_PERCENT_MAX);
    ui->updateProgressBar->setMinimum(0);
    ui->updateProgressBar->setValue(0);

    ui->displayBrowser->setMouseTracking(false);
    ui->displayBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->displayBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->displayBrowser->document()->setMaximumBlockCount(30);

    ui->InfoLabel->setText(tr("Don't Poweroff Until update process finish!"));

    connect(this, SIGNAL(sig_addStr2Borwser(QString)),
            this, SLOT(addStr2TextBrowser(QString)));
    connect(this, SIGNAL(sig_refreshProcessBarVal(int)),
            this, SLOT(refreshProcessBarVal(int)));

    this->startThread();
}

MainWindow::~MainWindow()
{
    this->stopThread();

    if (NULL != m_pDataController)
    {
        delete m_pDataController;
        m_pDataController = NULL;
    }

    if (NULL != m_pInterrupt)
    {
        delete m_pInterrupt;
        m_pInterrupt = NULL;
    }

    delete ui;
}

void MainWindow::startThread(void)
{
    CSystemDateTime::initSysTick();
    if(NULL != m_pInterrupt)
    {
        m_pInterrupt->startTimer(SCHED_RR, 2);
    }
    if(NULL != m_pDataController)
    {
        m_pDataController->startThread();
    }
//    CMyConsole::addStr2Console("startThread");
}

void MainWindow::stopThread(void)
{
    if(NULL != m_pDataController)
    {
        m_pDataController->stopThread();
    }
    if(NULL != m_pInterrupt)
    {
        m_pInterrupt->stopTimer();
    }
    CMyConsole::addStr2Console("stopThread");
}

void MainWindow::process(void)
{
    MutexLocker mLocker(mMutex);(void)mLocker;
    if(NULL != m_pDataController && NULL != m_pInterrupt)
    {
        unsigned long long runTimes = m_pInterrupt->runTimes();
        unsigned long long sysTick = CSystemDateTime::getSysTick();
        m_pDataController->process(sysTick, runTimes);
    }
}

void MainWindow::initApplication(void)
{
//    this->start2update();
}

void MainWindow::addDisplay(const QString &str)
{
    emit(sig_addStr2Borwser(str));
}

void MainWindow::addStr2TextBrowser(const QString &str)
{
    ui->displayBrowser->append(str);
    ui->displayBrowser->moveCursor(QTextCursor::End);
}

void MainWindow::clearDisplay(void)
{
    ui->displayBrowser->clear();
}

void MainWindow::setProcessBar(int percent)
{
    int percentForOneMission;
    int displayPrecent;

    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;

    if (1 == m_ProcessBarLeft && 100 == percent)
    {
        displayPrecent = PROCESS_BAR_PERCENT_MAX;
    }
    else
    {
        percentForOneMission = PROCESS_BAR_PERCENT_MAX / m_ProcessBarTotal;
        displayPrecent = percentForOneMission * (m_ProcessBarTotal - m_ProcessBarLeft) \
                + percent * percentForOneMission / 100;
    }

    emit(sig_refreshProcessBarVal(displayPrecent));
}

void MainWindow::setProcessBar(int total, int count2do)
{
    m_ProcessBarLeft = count2do;
    m_ProcessBarTotal = total;
}

void MainWindow::refreshProcessBarVal(const int val)
{
    ui->updateProgressBar->setValue(val);
}

void MainWindow::start2update(void)
{
    int ret = -1;
    if ((NULL != m_pDataController) && (false == m_bConfirmEnable))
    {
        m_bConfirmEnable = true;
        ret = m_pDataController->runUdiskUpdate();
        if (0 == ret)
        {
            ui->confirmButton->setDisabled(true);
            ui->cancelButton->setDisabled(true);
        }
        else
        {
            m_bConfirmEnable = false;
        }
    }
}

void MainWindow::on_confirmButton_clicked()
{
    start2update();
}

void MainWindow::on_cancelButton_clicked()
{
    if (NULL != m_pDataController)
    {
        m_pDataController->resetSystem();
    }
}

