#ifndef DELTAEMAINWINDOW_H
#define DELTAEMAINWINDOW_H

#include <QMainWindow>
#include "DeltaEInterface.h"
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
private:
    DeltaEInterface *pDteInterface;
private:
    Ui::DeltaEMainWindow *ui;
};

#endif // DELTAEMAINWINDOW_H
