#include "DeltaEMainwindow.h"
#include "ui_DeltaEMainwindow.h"
#include <QMessageBox>

DeltaEMainWindow::DeltaEMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeltaEMainWindow)
{
    ui->setupUi(this);
    colorUi = new ColorWindow(this);
    connect(ui->pBtn_Connect, SIGNAL(clicked()), this, SLOT(actConnect()));
    connect(ui->pBtn_Run, SIGNAL(clicked()), this, SLOT(actRun()));
    connect(ui->pBtn_Check, SIGNAL(clicked()), this, SLOT(actCheck()));
    connect(ui->pBtn_Adjust, SIGNAL(clicked()), this, SLOT(actAdjust()));
    connect(ui->pBtn_Color, SIGNAL(clicked()), this, SLOT(actOpenColor()));

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(actAbout()));

    pDteInterface = new DeltaEInterface(this);
    if(pDteInterface == NULL)
    {
        this->close();
    }
    connect(pDteInterface, SIGNAL(sendPatSignal(cRGB_t)), this, SLOT(actSendPat(cRGB_t)));
}

DeltaEMainWindow::~DeltaEMainWindow()
{
    if(colorUi != NULL)
    {
        delete colorUi;
    }
    delete ui;
}


void DeltaEMainWindow::actConnect()
{
    if(pDteInterface->dteConnect())
    {
        strTips.append("connect success!\n");
    }
    else
    {
        strTips.append("connect failed!\n");
    }
    showTipsMsg();
}

void DeltaEMainWindow::actRun()
{
    pDteInterface->dteRun();
    strTips.append("Run......\n");
    strTips.append(pDteInterface->getBackupMsg());
    showTipsMsg();
}

void DeltaEMainWindow::actCheck()
{
    pDteInterface->dteCheck();
    strTips.append("Check......\n");
    strTips.append(pDteInterface->getBackupMsg());
    showTipsMsg();
}

void DeltaEMainWindow::actAdjust()
{
    if(pDteInterface->dteAdjust())
    {
        strTips.append("adjust success!!!\n");
    }
    else
    {
        strTips.append("adjust fail!!!\n");
    }
    showTipsMsg();

}
void DeltaEMainWindow::actOpenColor()
{
    colorUi->show();
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

void DeltaEMainWindow::actSendPat(cRGB_t rgb)
{
   colorUi->setRGB(rgb);
   colorUi->updateColor();
}

void DeltaEMainWindow::showTipsMsg()
{
    ui->txt_Massage->setText(strTips);
}
