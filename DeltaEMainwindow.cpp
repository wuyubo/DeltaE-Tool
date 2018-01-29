#include "DeltaEMainwindow.h"
#include "ui_DeltaEMainwindow.h"

DeltaEMainWindow::DeltaEMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeltaEMainWindow)
{
    ui->setupUi(this);
    connect(ui->pBtn_Connect, SIGNAL(clicked()), this, SLOT(actConnect()));
    connect(ui->pBtn_Run, SIGNAL(clicked()), this, SLOT(actRun()));
    connect(ui->pBtn_Check, SIGNAL(clicked()), this, SLOT(actCheck()));
    connect(ui->pBtn_Adjust, SIGNAL(clicked()), this, SLOT(actAdjust()));

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(actAbout()));

    pDlEInterface = new DeltaECommonInterface(this);
    if(pDlEInterface == NULL)
    {
        this->close();
    }
}

DeltaEMainWindow::~DeltaEMainWindow()
{
    delete ui;
}


void DeltaEMainWindow::actConnect()
{
    int version = pDlEInterface->dteConnect();
    ui->txt_Massage->setText(QString::number(version, 16));
}

void DeltaEMainWindow::actRun()
{
    pDlEInterface->dteRun();
    ui->txt_Massage->setText("run");
}

void DeltaEMainWindow::actCheck()
{
    pDlEInterface->dteCheck();
    ui->txt_Massage->setText("Check");
}

void DeltaEMainWindow::actAdjust()
{
    pDlEInterface->dteAdjust();
    ui->txt_Massage->setText("adjust");
}

void DeltaEMainWindow::actAbout()
{
    ui->txt_Massage->setText("DeltaE v1.0 Base on Qt 5.6.3 MinGW 32bit");
}
