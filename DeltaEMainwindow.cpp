#include "DeltaEMainwindow.h"
#include "ui_DeltaEMainwindow.h"
#include <QMessageBox>

DeltaEMainWindow::DeltaEMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeltaEMainWindow)
{
    ui->setupUi(this);
    m_bisConnect = false;
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
    connect(pDteInterface, SIGNAL(updateMsgSignal()), this, SLOT(actUpdateMsg()));
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
    pBtnEnable(false);
    if(m_bisConnect == false)
    {
        if(pDteInterface->dteConnect())
        {
            strTips.append("connect success!\n");
            m_bisConnect = true;
            ui->pBtn_Connect->setText("disConnect");
        }
        else
        {
            strTips.append("connect failed!\n");
        }
    }
    else
    {
        pDteInterface->dteDisConnect();
        m_bisConnect = false;
        ui->pBtn_Connect->setText("Connect");
        strTips.append("disConnect!\n");
    }
    showTipsMsg();
    pBtnEnable(true);
}

void DeltaEMainWindow::actRun()
{
    bool bResult = false;
    strTips.append("Running......\n");
    showTipsMsg();
    pBtnEnable(false);
    bResult = pDteInterface->dteRun();
    if(bResult)
    {
       strTips.append("result PASS!!!\n");
    }else
    {
       strTips.append("result FAIL!!!\n");
    }
    showTipsMsg();
    pBtnEnable(true);
}

void DeltaEMainWindow::actCheck()
{
    bool bResult = false;
    strTips.append("Check......\n");
    showTipsMsg();
    pBtnEnable(false);
    bResult = pDteInterface->dteCheck();
    if(bResult)
    {
       strTips.append("result PASS!!!\n");
    }else
    {
       strTips.append("result FAIL!!!\n");
    }
    showTipsMsg();
    pBtnEnable(true);
}

void DeltaEMainWindow::actAdjust()
{
    pBtnEnable(false);
    if(pDteInterface->dteAdjust())
    {
        strTips.append("adjust success!!!\n");
    }
    else
    {
        strTips.append("adjust fail!!!\n");
    }
    showTipsMsg();
    pBtnEnable(true);
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
void DeltaEMainWindow::actUpdateMsg()
{
    strTips.append(pDteInterface->getBackupMsg());
    showTipsMsg();
}

void DeltaEMainWindow::showTipsMsg()
{
    ui->txt_Massage->setText(strTips);
    //移动光标到末尾
    QTextCursor cursor = ui->txt_Massage->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->txt_Massage->setTextCursor(cursor);
}

void DeltaEMainWindow::pBtnEnable(bool bEnable)
{
    ui->pBtn_Run->setEnabled(bEnable);
    ui->pBtn_Adjust->setEnabled(bEnable);
    ui->pBtn_Check->setEnabled(bEnable);
    ui->pBtn_Connect->setEnabled(bEnable);
    ui->pBtn_Color->setEnabled(bEnable);
}
