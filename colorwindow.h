#ifndef COLORWINDOW_H
#define COLORWINDOW_H

#include "global.h"
#include <QDialog>

namespace Ui {
class ColorWindow;
}


class ColorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ColorWindow(QWidget *parent = 0);
    ~ColorWindow();
public slots:
    void nextColor();
    void actStopTimer();
    void handleTimeout();  //超时处理函数
    void actStartTimer();
public:
    void setRGB(int r, int g, int b);
    void setRGB(cRGB_t rgb);
    void updateColor();

private:
    Ui::ColorWindow *ui;
    cRGB_t m_rgb;
    QTimer *m_pTimer;
};

#endif // COLORWINDOW_H
