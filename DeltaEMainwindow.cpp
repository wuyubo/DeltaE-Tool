#include "DeltaEMainwindow.h"
#include "ui_DeltaEMainwindow.h"
#include <QMessageBox>

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

    pDteInterface = new DeltaEInterface(this);
    if(pDteInterface == NULL)
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
    pDteInterface->dteConnect();
    ui->txt_Massage->setText("connect");
}

void DeltaEMainWindow::actRun()
{
    pDteInterface->dteRun();
    ui->txt_Massage->setText("run");
}

void DeltaEMainWindow::actCheck()
{
    pDteInterface->dteCheck();
    ui->txt_Massage->setText("Check");
}

void DeltaEMainWindow::actAdjust()
{
    pDteInterface->dteAdjust();
    ui->txt_Massage->setText("adjust");
}

void DeltaEMainWindow::actAbout()
{
    QMessageBox::about(this, tr("About"),
                      tr("DeltaE Tool v1.0 \nBase on Qt 5.6.3 MinGW 32bit \n" \
                         "Author:wuyubo @2018.01.29 \n\n" \
                         "This a DeltaE Tool!\n\n" \
                         "Github:https://github.com/wuyubo/DeltaE-Tool\n"\
                         "Copyright © 2018 CVTE.All rights reserved."));
}
