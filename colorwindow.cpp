#include "colorwindow.h"
#include "ui_colorwindow.h"

extern const cRGB_t defaultPatternList[32];
#define TIMER_TIMEOUT   (1*1000)

ColorWindow::ColorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorWindow)
{
    ui->setupUi(this);

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    //actStartTimer();
}

ColorWindow::~ColorWindow()
{
    if(m_pTimer != NULL)
    {
        delete m_pTimer;
    }
    delete ui;
}

void ColorWindow::setRGB(int r, int g, int b)
{
    m_rgb.red = r;
    m_rgb.green = g;
    m_rgb.blue = b;
}

void ColorWindow::setRGB(cRGB_t rgb)
{
    m_rgb.red = rgb.red;
    m_rgb.green = rgb.green;
    m_rgb.blue = rgb.blue;
}

void ColorWindow::nextColor()
{
    static int i = DEF_COLOR+1;

    //setRGB(defaultPatternList[i]);
   // updateColor();

    if(++i == 32) i = 0;
}

void ColorWindow::updateColor()
{
    QString color_style;
    color_style.sprintf("background-color:rgb(%d,%d,%d);", m_rgb.red, m_rgb.green, m_rgb.blue);
    ui->lb_color->setStyleSheet(color_style);
}

void  ColorWindow::handleTimeout()
{
    nextColor();
}

void ColorWindow::actStopTimer()
{
    if(m_pTimer->isActive()){
        m_pTimer->stop();
    }
}

void ColorWindow::actStartTimer()
{
    m_pTimer->start(TIMER_TIMEOUT);
}
