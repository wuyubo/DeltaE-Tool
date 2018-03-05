#include "DeltaEInterface.h"
#include <QTime>
cRGB_t g_PatternList[_MAX_PATTERN_COUT] = {
    {32, 32, 32},
    {64, 64, 64},
    {96, 96, 96},
    {128, 128, 128},
    {160, 160, 160},
    {192, 192, 192},
    {224, 224, 224},
    {255, 255, 255},
    {128, 0, 0},
    {192, 64, 64},
    {255, 0, 0},
    {255, 128, 128},
    {0, 128, 0},
    {64, 192, 64},
    {0, 255, 0},
    {0, 255, 0},
    {0, 0, 128},
    {64, 64, 192},
    {0, 0, 255},
    {128, 128, 255},
    {128, 128, 0},
    {192, 192, 64},
    {255, 255, 0},
    {255, 255, 128},
    {128, 0, 128},
    {192, 64, 192},
    {255, 0, 255},
    {255, 128, 255},
    {0, 128, 128},
    {64, 192, 192},
    {0, 255, 255},
    {128, 255, 255},
};
XYZCOLOR testXYZ[32] =
{
    {1.58,1.65,1.94},
    {7.75,8.15,9.33},
    {17.43,18.51,20.7},
    {31.15,32.96,37.17},
    {49.79,52.54,59.77},
    {73.71,77.55,88.85},
    {102.92,108.38,122.15},
    {135.4,143.96,156.18},
    {13.4,6.56,0.58},
    {36.6,23.41,10.99},
    {60.02,31.41,2.42},
    {77.49,56.92,39.53},
    {11.74,23.61,3.51},
    {32.47,58.01,17.83},
    {51.81,104.66,16.62},
    {70.67,113.37,50.15},
    {6.07,2.06,32.11},
    {21,13.31,79.45},
    {26.49,10.1,139.36},
    {51.36,40.91,143.63},
    {24.84,30.43,4.03},
    {60.58,72.57,19.18},
    {109.74,134.22,19.39},
    {115.74,136.49,52.34},
    {19.6,9.03,33.12},
    {49.8,28.49,80.91},
    {86.45,42.09,141.99},
    {97.45,64.94,145.17},
    {18.04,26.14,36.55},
    {45.58,63.04,87.5},
    {77.84,114.52,154.48},
    {90.45,120.85,154.89},
};

const cRGB_t pat_White = {255, 255, 255};


DeltaEInterface::DeltaEInterface(QObject *parent) : QObject(parent)
{
    func_status = FUNC_NONE;
    pCa210 = new Ca210DllCtr(CA210DLL);
    pMstGenGma = new MstGenGmaCtr();
    if(!pCa210 || !pMstGenGma)
    {
        exit(-1);
    }
    m_pBurnsettings = readI2CSetting();
    i2cdevice = new Isp_I2C();
    m_pDDCprotocol = new DDCProtocol_T(*i2cdevice);
    m_pDDCprotocol->setSlaveAddr(m_pBurnsettings->m_slaveaddr);
    m_transfer = new Transfer_T(*m_pDDCprotocol,m_pBurnsettings->m_perpackretrycnt);
    m_pdata = new Data(this);
    if(m_pdata == NULL)
    {
        exit(-1);
    }
    connect(m_pDDCprotocol,SIGNAL(start_emit(QString)),this,SLOT(recvMsg(QString)));
}

DeltaEInterface::~DeltaEInterface()
{

    delete pCa210;
    delete pMstGenGma;

    if(i2cdevice)
    {
        if(i2cdevice->gethandle())
        {
            i2cdevice->closeDevice(i2cdevice->gethandle());
        }
        delete i2cdevice;
    }
    delete m_pBurnsettings;
    delete m_pDDCprotocol;
    delete m_transfer;
    delete m_pdata;
}

bool DeltaEInterface::dteConnect()
{
    return (connectI2C()&& pCa210->caConnect(CA210_CHANNEL));
}

int DeltaEInterface::dteRun()
{
    setStatus(FUNC_RUN);
    //return sRGB_DeltaERun();
    return sRGB_DeltaEAdjust();
}

int DeltaEInterface::dteCheck()
{
    setStatus(FUNC_CHECK);
    return sRGB_DeltaEVerify();;
}

bool DeltaEInterface::dteAdjust()
{
    setStatus(FUNC_ADJUST);
    QString temp;
    LPCA210DATASTRUCT ca210 = pCa210->caMeasure();
    XYZCOLOR xyz = pCa210->caGetAverageMeasureXYZ(1);
    temp.sprintf("X:%d-%d, Y:%d-%d, Z:%d-%d\n",
                 ca210->fX, xyz.fX, ca210->fY,xyz.fY, ca210->fZ, xyz.fZ);
    showMsg(temp);
    return pCa210->caSetChannel(0);
}
bool DeltaEInterface::connectI2C()
{
    DEBUGMSG("the i2c speed:%d",m_pBurnsettings->getI2cSpeed());
    if (i2cdevice->openDevice(i2cdevice->gethandle(),m_pBurnsettings->getI2cSpeed()*1000.0f) == FTC_SUCCESS)
    {
        return true;
    }
    else
    {
        showMsg("[I2C] connect I2C fail!! \n");
        return false;
    }
}
BurnSetting_T *DeltaEInterface::readI2CSetting()
{
    QSettings Burn_Settings("Cvte","DeltaE Tool");
    BurnSetting_T *pburnsettings;
    Burn_Settings.beginGroup("Burn_setting");
    quint8 tmp_slaveaddr = Burn_Settings.value("Burn_SlaveAddr",0x6E).toInt();
    int tmp_I2cSpeed = Burn_Settings.value("Burn_I2cSpeed", 5).toInt();
    int tmp_WriteDelay = Burn_Settings.value("Burn_writeDelay", 300).toInt();
    int tmp_ReadDelay = Burn_Settings.value("Burn_readDelay", 200).toInt();
    int tmp_RetryCnt = Burn_Settings.value("Burn_RetryCnt", 3).toInt();
    int tmp_PerPackRetryCnt = Burn_Settings.value("Burn_PerPackRetryCnt",3).toInt();
    int tmp_EdidlastDelay = Burn_Settings.value("Burn_EdidlastDelay", 600).toInt();
    int tmp_HdcplastDelay = Burn_Settings.value("Burn_HdcplastDelay", 600).toInt();
    int tmp_EraseHdcp = Burn_Settings.value("Burn_eraseHdcpkeyDelay", 444).toInt();
    int tmp_IsCreatLogs =Burn_Settings.value("Burn_isCreatlogs", 0).toInt();

    pburnsettings = new BurnSetting_T(tmp_slaveaddr,tmp_I2cSpeed,tmp_WriteDelay,tmp_ReadDelay,tmp_RetryCnt,tmp_PerPackRetryCnt,
    tmp_EdidlastDelay,tmp_HdcplastDelay,tmp_EraseHdcp,(bool)tmp_IsCreatLogs);

    Burn_Settings.endGroup();

    QSettings At_cmds("Cvte","DeltaE Tool");
    At_cmds.beginGroup("AT_Cmd");

    At_cmds.endGroup();
    return pburnsettings;
}
QString DeltaEInterface::getBackupMsg()
{
    return backupMsg;
}

void DeltaEInterface::sendPattern(cRGB_t rgb)
{
    quint8 *tempcmd;
    if(isStatus(FUNC_CHECK))
    {
        emit sendPatSignal(rgb);
    }else if(isStatus(FUNC_RUN) || isStatus(FUNC_ADJUST))
    {
        tempcmd = writeDeltaERGBPaternCmd.burndata;
        tempcmd[PAT_R] = rgb.red;
        tempcmd[PAT_G] = rgb.green;
        tempcmd[PAT_B] = rgb.blue;
        cmdSend(&writeDeltaERGBPaternCmd);
    }
}
void DeltaEInterface::showMsg(QString msg)
{
    backupMsg.clear();
    backupMsg.append(msg);
    emit updateMsgSignal();
}

bool DeltaEInterface::isStatus(FUNCSTATUS_t status)
{
    return (func_status == status);
}

void DeltaEInterface::setStatus(FUNCSTATUS_t status)
{
    func_status = status;
}

void DeltaEInterface::delayMs(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

bool DeltaEInterface::sRGB_DeltaEVerify()
{

    if(!pCa210 || !pCa210->isConnect())
    {
       showMsg("please connect Ca210...\n");
       return false;
    }
    sRGB_DeltaEVerifyStep0();
    sRGB_DeltaEVerifyStep1();
    sRGB_DeltaEVerifyStep2();
    return sRGB_DeltaEVerifyStep3();
}
bool DeltaEInterface::sRGB_DeltaEVerifyStep0()
{
    QString temp;
    sRGBResult = 0;
    m_pdata->update_PatRgb(); //load pattern file
    temp.sprintf("Pattern Count: %d\n", m_pdata->pat_RgbCount);
    showMsg(temp);
    return true;
}

bool DeltaEInterface::sRGB_DeltaEVerifyStep1()
{
    XYZCOLOR color={0,0,0};
    QString temp;
    sendPattern(pat_White);//SEND 100% White pattern
    delayMs(300);//延时300ms
    color = pCa210->caGetAverageMeasureXYZ(3);//获取CA210 的XYZ值
    m_d100W_Raw_Y = color.fX;
    temp.sprintf("100 White Y : %lf \n", m_d100W_Raw_Y);
    showMsg(temp);
    return true;
}

bool DeltaEInterface::sRGB_DeltaEVerifyStep2()
{
    XYZCOLOR color={0,0,0};
    QString temp;
    int pattern_index=0;
    float result;
    for(pattern_index = 0;pattern_index < m_pdata->pat_RgbCount; pattern_index++)
    {
        sendPattern(m_pdata->pat_RgbList[pattern_index]);//发送测试Pattern
        delayMs(300);//延时300ms
        color = pCa210->caGetAverageMeasureXYZ(3);//获取CA210 的XYZ值
       // color = testXYZ[pattern_index];
        result = GetDeltaE_OnePatCIE94(pattern_index, color.fX, color.fY, color.fZ, m_d100W_Raw_Y);
        sRGBResult += result;
        temp.sprintf("%d -(%lf, %lf, %lf) : %lf\n", pattern_index+1, color.fX, color.fY, color.fZ, result);
        showMsg(temp);
    }
    return true;
}

bool DeltaEInterface::sRGB_DeltaEVerifyStep3()
{
    QString temp;
    if(sRGBResult > 0)
    {
        sRGBResult = sRGBResult/m_pdata->pat_RgbCount;
    }
    temp.sprintf("result is %lf\n", sRGBResult);
    showMsg(temp);
    if(sRGBResult < 2.0)
    {
        return true;
    }
    return false;
}


bool DeltaEInterface::sRGB_DeltaEAdjust()
{
    sRGB_DeltaEAdjustStep0();// generate gamma
    return sRGB_DeltaEAdjustStep1();//send gamma to monitor
    //return sRGB_DeltaEAdjustStep2();
}
bool DeltaEInterface::sRGB_DeltaEAdjustStep0()
{
    double GmaIn[4*5*9];
    BYTE outCompressBuf[64*3];
    BYTE sRgbCM[19];
    const int gammaEntris = 256;
    const int gamutType = 1;
    pMstGenGma->mmstSetMeasPtnNum(9);
    pMstGenGma->mmstSetCompressType(0);// 0 : 64 bytes ,1 : 76 bytes
    pMstGenGma->mGetPanelNativeData(GmaIn);
    //Generate gamma curve and color matrix
    pMstGenGma->mmstGenGamutData(GmaIn, gammaEntris, gamutType, outCompressBuf,sRgbCM);
    m_pdata->saveCompGma("",outCompressBuf, 64*3);
    m_pdata->saveColorMatrix("",sRgbCM, 18);\
    return true;
}
bool DeltaEInterface::sRGB_DeltaEAdjustStep1()
{
    BYTE *pCompGama;
    BYTE *psRgbCm;
    int i = 0;
   // QString temp;
    pCompGama = m_pdata->getCompGma();
    psRgbCm = m_pdata->getColorMatrix();
    //先发送MS_WR_BLOCK ，进入调试模式
    cmdSend(&enterDeltaEDebugcmd);
    for(i = 0; i < 3; i++)
    {
        //先发送 GAMMA MODE 0 ，Channel i，前40字节
        POSTGMA_SETMODE(0);
        POSTGMA_SETCHANEL(i);
        POSTGMA_SETSIZE(40);
        POSTGMA_SETOFFSET(0);
        cmdSend(&writeDeltaEPostGammacmd, pCompGama+i*64, 40);
        cmdSend(&readDeltaEACKcmd);
        cmdSend(&readDeltaEACKcmd);
        cmdSend(&readDeltaEACKcmd);
        //发送 GAMMA MODE 0 ，Channel i，后24字节s
        POSTGMA_SETSIZE(24);
        POSTGMA_SETOFFSET(40);
        cmdSend(&writeDeltaEPostGammacmd, pCompGama+i*64+40, 24);
        cmdSend(&readDeltaEACKcmd);
        cmdSend(&readDeltaEACKcmd);
        cmdSend(&readDeltaEACKcmd);
    }
    //保存SRGB Gamma
    cmdSend(&saveDeltaEGmacmd);
    //退出调试模式
    cmdSend(&exitDeltaEDebugcmd);
    //写ColorMatrix Mode：SRGB 18Byte
    cmdSend(&enterDeltaEDebugcmd);
    cmdSend(&writeDeltaEColorMatrixcmd, psRgbCm, 18);
    cmdSend(&readDeltaEACKcmd);
    cmdSend(&readDeltaEACKcmd);
    cmdSend(&readDeltaEACKcmd);
    //退出调试模式
    return cmdSend(&exitDeltaEDebugcmd);
}

bool DeltaEInterface::sRGB_DeltaEAdjustStep2()
{
    return false;
}
bool DeltaEInterface::cmdSend(QString CmdStr)
{
    if (NULL==i2cdevice->gethandle())
    {
        showMsg("pleas open device first!!\n");
        return false;
    }

    //first:find all the substring and judge the length. must less than 2
    QStringList cmdlist = CmdStr.split(' ', QString::SkipEmptyParts);

    for(auto x:cmdlist)
    {
        if(x.length()>2)
        {
            showMsg("Commands format error.\n");
            return false;
        }
    }
    if(cmdlist.size()>40)
    {
        showMsg("you may send too much.\n");
        return false;
    }
    if(cmdlist.size()==0)
    {
        showMsg("Just type something,man~\n");
        return false;
    }

    quint8* ins = new quint8[cmdlist.size()];
    bool ok;
    showMsg("\n");
    showMsg("User defined Instrucitons:\n");
    QString _usrstr;
    for (int i = 0; i < cmdlist.size(); ++i)
    {
        ins[i] = cmdlist.at(i).toInt(&ok,16);
        _usrstr.append(cmdlist.at(i));
        _usrstr.append(" ");
    }
    //showMsg(_usrstr.toUpper());
   // showMsg("\n");
    //send
    burnCmd_t c =
    {
        nullptr,
        nullptr,
        nullptr,
        ins,
        (quint8)cmdlist.size(),
        nullptr,
        9,
        nullptr,
        1,
        (quint32)m_pBurnsettings->m_writedelay,
        (quint32)m_pBurnsettings->m_writedelay,
    };

    m_transfer->setburnCmd(&c,nullptr,0,1);

    m_transfer->run();
    m_transfer->wait();//wait for the end of transfer thread.
    delete[] ins;
    return true;
}

bool DeltaEInterface::cmdSend(burnCmd_t *cmd)
{
    if(m_transfer == NULL)
    {
        return false;
    }
    m_transfer->setburnCmd(cmd,nullptr,0,1);

    m_transfer->run();
    m_transfer->wait();//wait for the end of transfer thread.
    return true;
}

bool DeltaEInterface::cmdSend(burnCmd_t *cmd,quint8 *data, quint32 size,quint8 source)
{
    if(m_transfer == NULL)
    {
        return false;
    }
    m_transfer->setburnCmd(cmd,data,size,source);

    m_transfer->run();
    m_transfer->wait();//wait for the end of transfer thread.
    return true;
}

void DeltaEInterface::recvMsg(QString msg)
{
    showMsg(msg);
}

bool DeltaEInterface::sRGB_DeltaERun()
{
    if(!pCa210 || !pCa210->isConnect())
    {
       showMsg("please connect Ca210 or I2c device...\n");
       return false;
    }
    sRGB_DeltaERunstep0(); //monitor enter debug mode and load patern
    sRGB_DeltaERunstep1();
    sRGB_DeltaERunstep2();
    sRGB_DeltaERunstep3();
    return sRGB_DeltaERunstep4();
}

bool DeltaEInterface::sRGB_DeltaERunstep0()
{
    if(cmdSend(&enterDeltaEDebugcmd)
       && cmdSend(&enterDeltaEAutoGammacmd))
    {
        return sRGB_DeltaEVerifyStep0();
    }
    showMsg("DeltaE Debug FAIL！！\n");
    return false;
}

bool DeltaEInterface::sRGB_DeltaERunstep1()
{
    return sRGB_DeltaEVerifyStep1();
}

bool DeltaEInterface::sRGB_DeltaERunstep2()
{
    return sRGB_DeltaEVerifyStep2();
}

bool DeltaEInterface::sRGB_DeltaERunstep3()
{
    cmdSend(&exitDeltaEDebugcmd);
    cmdSend(&exitDeltaEAutoGammacmd);
    return sRGB_DeltaEVerifyStep3();
}

bool DeltaEInterface::sRGB_DeltaERunstep4()
{
    return sRGB_DeltaEAdjust();
}
