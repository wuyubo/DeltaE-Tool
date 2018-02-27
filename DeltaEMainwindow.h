#ifndef DELTAEMAINWINDOW_H
#define DELTAEMAINWINDOW_H

#include "DeltaEInterface.h"
#include "colorwindow.h"
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
    void actSendPat(cRGB_t rgb);
    void actUpdateMsg();
private:
    void showTipsMsg();

private:
    Ui::DeltaEMainWindow *ui;
    ColorWindow *colorUi;
    DeltaEInterface *pDteInterface;
    QString strTips;
};

#endif // DELTAEMAINWINDOW_H
