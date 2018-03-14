#ifndef DELTAEMAINWINDOW_H
#define DELTAEMAINWINDOW_H

#include "DeltaEInterface.h"
#include "colorwindow.h"
#include "workerthread.h"
#include <QMainWindow>
namespace Ui {
class DeltaEMainWindow;
}

class DeltaEMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DeltaEMainWindow(QWidget *parent = 0);
    ~DeltaEMainWindow();
private slots:
    void actConnect();
    void actRun();
    void actCheck();
    void actAdjust();
    void actAbout();
    void actOpenColor();
    void actTest();
    void actSendPat(cRGB_t rgb);
    void actUpdateMsg(LOGTEXTTYPE_t logType);
    void actWorkFeeback(FUNCSTATUS_t status, bool result);
private:
    void showTipsMsg(LOGTEXTTYPE_t logType = LOG_NORMAL);
    void pBtnEnable(bool bEnable);

private:
    Ui::DeltaEMainWindow *ui;
    ColorWindow *colorUi;
    DeltaEInterface *pDteInterface;
    QString strTips;
    bool m_bisConnect;
    WorkerThread *workerThread;
};

#endif // DELTAEMAINWINDOW_H
