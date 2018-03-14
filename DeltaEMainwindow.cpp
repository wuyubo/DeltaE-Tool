#include "DeltaEMainwindow.h"
#include "ui_DeltaEMainwindow.h"
#include <QMessageBox>
#include <QMetaType>

DeltaEMainWindow::DeltaEMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeltaEMainWindow)
{
    ui->setupUi(this);
    m_bisConnect = false;
    colorUi = NULL;
    workerThread = new WorkerThread(this);
    qRegisterMetaType<LOGTEXTTYPE_t>("LOGTEXTTYPE_t");//注册LOGTEXTTYPE_t类型
    qRegisterMetaType<cRGB_t>("cRGB_t");//注册cRGB_t类型
    qRegisterMetaType<FUNCSTATUS_t>("FUNCSTATUS_t");//注册FUNCSTATUS_t类型

    connect(ui->pBtn_Connect, SIGNAL(clicked()), this, SLOT(actConnect()));
    connect(ui->pBtn_Run, SIGNAL(clicked()), this, SLOT(actRun()));
    connect(ui->pBtn_Check, SIGNAL(clicked()), this, SLOT(actCheck()));
    connect(ui->pBtn_Adjust, SIGNAL(clicked()), this, SLOT(actAdjust()));
    connect(ui->pBtn_Color, SIGNAL(clicked()), this, SLOT(actOpenColor()));
    connect(ui->pBtn_Test, SIGNAL(clicked()), this, SLOT(actTest()));

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(actAbout()));

    pDteInterface = new DeltaEInterface(this);
    if(pDteInterface == NULL)
    {
        this->close();
    }
    workerThread->setInterface(pDteInterface);
    connect(pDteInterface, SIGNAL(sendPatSignal(cRGB_t)), this, SLOT(actSendPat(cRGB_t)));
    connect(pDteInterface, SIGNAL(updateMsgSignal(LOGTEXTTYPE_t)), this, SLOT(actUpdateMsg(LOGTEXTTYPE_t)));
    connect(workerThread, SIGNAL(signalSendFeeback(FUNCSTATUS_t,bool)), this, SLOT(actWorkFeeback(FUNCSTATUS_t,bool)));
}

DeltaEMainWindow::~DeltaEMainWindow()
{
    if(colorUi)
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
        strTips.append("connect....");
        showTipsMsg();
        if(pDteInterface->dteConnect())
        {
            m_bisConnect = true;
            ui->pBtn_Connect->setText("DisConnect");
            strTips.append("connect seccess !!\n");
            showTipsMsg(LOG_PASS);
        }
    }
    else
    {
        pDteInterface->dteDisConnect();
        m_bisConnect = false;
        ui->pBtn_Connect->setText("Connect");
        strTips.append("disConnect!\n");
        showTipsMsg(LOG_PASS);
    }
    pBtnEnable(true);
}

void DeltaEMainWindow::actRun()
{
    if(colorUi == NULL)
    {
        strTips.append("please open color window......\n");
        showTipsMsg(LOG_ERROR);
        return;
    }
    pBtnEnable(false);
    workerThread->setStatus(FUNC_RUN);
    workerThread->start();
    strTips.append("Running......\n");
    showTipsMsg();
}

void DeltaEMainWindow::actCheck()
{
    if(colorUi == NULL)
    {
        strTips.append("please open color window......");
        showTipsMsg(LOG_ERROR);
        return;
    }
    pBtnEnable(false);
    workerThread->setStatus(FUNC_CHECK);
    workerThread->start();
    strTips.append("Check......\n");
    showTipsMsg();
}

void DeltaEMainWindow::actAdjust()
{
    if(ui->rBtn_measure->isChecked())
    {
        pDteInterface->setAdjType(ADJ_MEASURE);
    }
    else if(ui->rBtn_file->isChecked())
    {
        pDteInterface->setAdjType(ADJ_FILE);
    }
    pBtnEnable(false);
    workerThread->setStatus(FUNC_ADJUST);
    workerThread->start();
    strTips.append("adjust......\n");
    showTipsMsg();
}
void DeltaEMainWindow::actOpenColor()
{
    colorUi = new ColorWindow(this);
    if(colorUi)
    {
        colorUi->show();
    }
}
void DeltaEMainWindow::actTest()
{
    pDteInterface->dteTest(ui->txt_R->text().toInt(),
                           ui->txt_G->text().toInt(),
                           ui->txt_B->text().toInt());
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
    if(colorUi)
    {
        colorUi->setRGB(rgb);
        colorUi->updateColor();
    }
}
void DeltaEMainWindow::actUpdateMsg(LOGTEXTTYPE_t logType)
{
    strTips = pDteInterface->getBackupMsg();
    showTipsMsg(logType);
}
void DeltaEMainWindow::actWorkFeeback(FUNCSTATUS_t status, bool result)
{
    workerThread->quit();
    workerThread->wait();
    switch (status) {
    case FUNC_CHECK:
        strTips.append("check ");
        break;
    case FUNC_ADJUST:
        strTips.append("adjust ");
        break;
    case FUNC_RUN:
        strTips.append("run ");
        break;
    default:
        break;
    }
    if(result)
    {
        strTips.append("success !!!\n");
        showTipsMsg(LOG_PASS);
    }
    else
    {
        strTips.append("fail !!!\n");
        showTipsMsg(LOG_ERROR);
    }
    pBtnEnable(true);
}

void DeltaEMainWindow::showTipsMsg(LOGTEXTTYPE_t logType)
{
    QColor logColor = Qt::black;
    switch(logType)
    {
        case LOG_NONE:
            return;
        case LOG_NORMAL:
            logColor = Qt::black;
            break;
        case LOG_PASS:
            logColor = Qt::green;
            break;
        case LOG_ERROR:
            logColor = Qt::red;
            break;
        case LOG_WARNING:
            logColor = Qt::yellow;
            break;
        case LOG_TEXTBOX:
            break;
        default:
            break;
    }

    ui->logtextBrowser->setTextColor(logColor);
    ui->logtextBrowser->append(strTips);
    strTips.clear();
}

void DeltaEMainWindow::pBtnEnable(bool bEnable)
{
    ui->pBtn_Run->setEnabled(bEnable);
    ui->pBtn_Adjust->setEnabled(bEnable);
    ui->pBtn_Check->setEnabled(bEnable);
    ui->pBtn_Connect->setEnabled(bEnable);
    ui->pBtn_Color->setEnabled(bEnable);
}
