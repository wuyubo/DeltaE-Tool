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
}

DeltaEMainWindow::~DeltaEMainWindow()
{
    delete ui;
}


void DeltaEMainWindow::BtnConnect()
{
    ui->txt_Massage->setText("connect");
}

void DeltaEMainWindow::BtnRun()
{
    ui->txt_Massage->setText("run");
}

void DeltaEMainWindow::BtnCheck()
{
    ui->txt_Massage->setText("Check");
}

void DeltaEMainWindow::BtnAdjust()
{
    ui->txt_Massage->setText("adjust");
}
