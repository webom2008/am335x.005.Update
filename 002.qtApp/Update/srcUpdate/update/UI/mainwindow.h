#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../Utilities/includes.h"
#include "../Utilities/autolock.h"

class CInterrupt;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initApplication(void);

public:

    void addDisplay(const QString &str);
    void clearDisplay(void);
    void setProcessBar(int percent);
    void setProcessBar(int total, int count2do);
    void process(void);

private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();
    void addStr2TextBrowser(const QString &str);
    void refreshProcessBarVal(const int val);

signals:
    void sig_addStr2Borwser(const QString &str);
    void sig_refreshProcessBarVal(const int val);

private:
    void startThread(void);
    void stopThread(void);
    void start2update(void);

    Ui::MainWindow      *ui;
    Mutex               mMutex;
    CDataController     *m_pDataController;
    CInterrupt          *m_pInterrupt;
    int                 m_ProcessBarTotal;
    int                 m_ProcessBarLeft;
    bool                m_bConfirmEnable;
};

#endif // MAINWINDOW_H
