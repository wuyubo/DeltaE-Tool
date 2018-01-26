#ifndef DELTAEMAINWINDOW_H
#define DELTAEMAINWINDOW_H

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

private:
    Ui::DeltaEMainWindow *ui;
};

#endif // DELTAEMAINWINDOW_H
