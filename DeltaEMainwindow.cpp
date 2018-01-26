#include "DeltaEMainwindow.h"
#include "ui_DeltaEMainwindow.h"

DeltaEMainWindow::DeltaEMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeltaEMainWindow)
{
    ui->setupUi(this);
    connect(ui->pBtn_Connect, SIGNAL(clicked()), this, SLOT(BtnConnect()));
    connect(ui->pBtn_Run, SIGNAL(clicked()), this, SLOT(BtnRun()));
    connect(ui->pBtn_Check, SIGNAL(clicked()), this, SLOT(BtnCheck()));
    connect(ui->pBtn_Adjust, SIGNAL(clicked()), this, SLOT(BtnAdjust()));

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


void DeltaEMainWindow::BtnConnect()
{
    pDlEInterface->DtEConnect();
    ui->txt_Massage->setText("connect");
}

void DeltaEMainWindow::BtnRun()
{
    pDlEInterface->DtERun();
    ui->txt_Massage->setText("run");
}

void DeltaEMainWindow::BtnCheck()
{
    pDlEInterface->DtECheck();
    ui->txt_Massage->setText("Check");
}

void DeltaEMainWindow::BtnAdjust()
{
    pDlEInterface->DtEAdjust();
    ui->txt_Massage->setText("adjust");
}
