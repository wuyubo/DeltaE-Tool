#include "DeltaEInterface.h"
#include <QTime>
cRGB_t pat_RGBW[] =
{
    {255, 0, 0},
    {0, 255, 0},
    {0, 0, 255},
    {255, 255, 255},
};

DeltaEInterface::DeltaEInterface(QObject *parent) : QObject(parent)
{
    func_status = FUNC_NONE;
    m_delayTimeMs = DEF_DALEY_MS;
    pCa210 = new Ca210DllCtr(CA210DLL);
    pMstGenGma = new MstGenGmaCtr();
    m_isConnect = false;
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
    if(connectI2C()&& pCa210->caConnect(CA210_CHANNEL))
    {
        m_isConnect = true;
    }
    return m_isConnect;
}
void DeltaEInterface::dteDisConnect()
{
    if(!m_isConnect)
    {
        showMsg("pls connect...\n");
        return;
    }
    if(i2cdevice->gethandle())
    {
        i2cdevice->closeDevice(i2cdevice->gethandle());
    }
    if(pCa210->isConnect())
    {
        pCa210->caDisConnect();
    }
    m_isConnect = false;
}

int DeltaEInterface::dteRun()
{
    if(!m_isConnect)
    {
        showMsg("pls connect...\n");
        return m_isConnect;
    }
    if(dteCheck())
    {
        return true;
    }
    //if check failed, run to adjust.
    setStatus(FUNC_RUN);
    return sRGB_DeltaERun();
}

int DeltaEInterface::dteCheck()
{
    if(!m_isConnect)
    {
        showMsg("pls connect...\n");
        return m_isConnect;
    }
    setStatus(FUNC_CHECK);
    return sRGB_DeltaEVerify();;
}

bool DeltaEInterface::dteAdjust()
{
    if(!m_isConnect)
    {
        showMsg("pls connect...\n");
        return m_isConnect;
    }
    //run to adjust
    setStatus(FUNC_RUN);
    return sRGB_DeltaEAdjust();
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
    sRGB_DeltaEVerifyStep0(); //0. load pattern
    sRGB_DeltaEVerifyStep1(); //1. get 100% wihte Y
    sRGB_DeltaEVerifyStep2(); //2. measure gamma data
    return sRGB_DeltaEVerifyStep3(); //3. check the result
}
bool DeltaEInterface::sRGB_DeltaEVerifyStep0()
{
    QString temp;
    sRGBResult = 0;
    if(isStatus(FUNC_CHECK))
    {
        m_delayTimeMs = CHECK_DALEY_MS;
    }else
    {
        m_delayTimeMs = ADJUST_DALEY_MS;
    }
    m_pdata->update_PatRgb(isStatus(FUNC_CHECK), PATTERN_LEVEL); //load pattern file
    temp.sprintf("Pattern Count: %d\n", m_pdata->pat_RgbCount);
    showMsg(temp);
    return true;
}

bool DeltaEInterface::sRGB_DeltaEVerifyStep1()
{
    //XYZCOLOR color={0,0,0};
    LPCA210DATASTRUCT pca210;
    QString temp;
    sendPattern(pat_RGBW[3]);//SEND 100% White pattern
    delayMs(m_delayTimeMs);//延时ms
   // color = pCa210->caGetAverageMeasureXYZ(3);//获取CA210 的XYZ值
    pca210 = pCa210->caMeasure();
    m_d100W_Raw_Y = pca210->fY;

    temp.sprintf("100 White Y : %lf \n", m_d100W_Raw_Y);
    showMsg(temp);
    return true;
}

bool DeltaEInterface::sRGB_DeltaEVerifyStep2()
{
    //XYZCOLOR color={0,0,0};
    LPCA210DATASTRUCT pca210;
    QString temp;
    int pattern_index=0;
    float result;

    for(pattern_index = 0;pattern_index < m_pdata->getPatternCount(); pattern_index++)
    {
        sendPattern(m_pdata->pat_RgbList[pattern_index]);//发送测试Pattern
        delayMs(m_delayTimeMs);//延时ms
        //color = pCa210->caGetAverageMeasureXYZ(3);//获取CA210 的XYZ值
       // color = testXYZ[pattern_index];
        pca210 = pCa210->caMeasure();
        delayMs(40);//延时ms
        //pca210->fX = color.fX;
        //pca210->fY = color.fY;
        //pca210->fZ = color.fZ;
        if(isStatus(FUNC_CHECK))
        {
            result = GetDeltaE_OnePatCIE94(pattern_index, pca210->fX, pca210->fY, pca210->fZ, m_d100W_Raw_Y);
            sRGBResult += result;
            temp.sprintf("%d : %lf\n", pattern_index+1,result);
            showMsg(temp);
        }
        m_pdata->setPanelNativeData((int)pattern_index/(m_pdata->pat_Level*5), pattern_index%m_pdata->pat_Level,
                                    pca210->fX, pca210->fY, pca210->fZ, pca210->fSx, pca210->fSy);
    }
    m_pdata->savePanelNativeData();
    if(isStatus(FUNC_CHECK))
    {
        m_pdata->saveDeltaEData();
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
    if(sRGBResult < DET94_RESULT)
    {
        return true;
    }
    return false;
}


bool DeltaEInterface::sRGB_DeltaEAdjust()
{
    sRGB_DeltaEAdjustStep0();//load native data
    sRGB_DeltaEAdjustStep1();// generate gamma
    return sRGB_DeltaEAdjustStep2();//send gamma to monitor
}
bool DeltaEInterface::sRGB_DeltaEAdjustStep1()
{
    double GmaIn[4*5*PATTERN_LEVEL];
    BYTE outCompressBuf[64*3];
    BYTE sRgbCM[19];
    const int gammaEntris = 256;
    //nativeDataFmtType=0:9_ptn,1:17_ptn,2:33_ptn,3:65_ptn,4:129_ptn,5:256_ptn,
    //6 : RGB_1ptn_W_9ptn
    const int nativeDataFmtType = m_pdata->pat_Level;
    //gamutType = 0: disable, 1 : sRGB(gma 2.2), 2 : adobeRGB(gma 2.2), 3 : BT709(gma 2.4),
    //4 : BT2020(gma 2.4), 5 : DCI_P3(gma 2.6), 6 : User_define(gma 2.2)
    const int gamutType = 1;
    //Set compression type to none data loss compression method.
    const int compGmaType = 0;// 0 : 64 bytes ,1 : 76 bytes

    m_pdata->getPanelNativeData(GmaIn);
    pMstGenGma->msetGammaEntries(gammaEntris);
    pMstGenGma->msetPanelNativeData(GmaIn, 0);
    pMstGenGma->msetGamutType(gamutType);
    pMstGenGma->msetGammaTrackType(1);
    pMstGenGma->msetGammaPower(2.2);
    pMstGenGma->msetColorTempTrackType(1);
    pMstGenGma->msetColorTemperatureXY(0.313,0.329);
    pMstGenGma->msetDarkModifySettings(1, 1);
    //pMstGenGma->mmstSetMeasPtnNum(nativeDataFmtType);

    //pMstGenGma->mmstSetCompressType(compGmaType);

    //pMstGenGma->msetBrightModifySettings(0, 255);

    //pMstGenGma->mmstSetTargetGamut(0, 0.6354, 0.3422);//R
    //pMstGenGma->mmstSetTargetGamut(1, 0.3031, 0.6170);//G
    //pMstGenGma->mmstSetTargetGamut(2, 0.1475, 0.0590);//B
    //pMstGenGma->mmstSetTargetGamut(3, 0.3124, 0.3284);//W
    //pMstGenGma->mmstGenGamutData(GmaIn, gammaEntris, gamutType, outCompressBuf,sRgbCM);

    //pMstGenGma->mmstGenUserGamutData(GmaIn, outCompressBuf, sRgbCM);
    pMstGenGma->mgenerateGamma();
    pMstGenGma->mgetCompressedGmaData(compGmaType, outCompressBuf);
    pMstGenGma->mgetColorMatrixData(sRgbCM);
    m_pdata->saveCompGma(outCompressBuf, 64*3);
    m_pdata->saveColorMatrix(sRgbCM, 18);
    return true;
}
bool DeltaEInterface::sRGB_DeltaEAdjustStep2()
{
    BYTE *pCompGama;
    BYTE *psRgbCm;
    int i = 0;
    pCompGama = m_pdata->getCompGma();
    psRgbCm = m_pdata->getColorMatrix();
    //先发送MS_WR_BLOCK ，进入调试模式
    cmdSend(&enterDeltaEDebugcmd);
    for(i = 0; i < 3; i++)
    {
        //先发送 GAMMA MODE 0 ，Channel i，前32字节
        POSTGMA_SETMODE(1);
        POSTGMA_SETCHANEL(i);
        POSTGMA_SETSIZE(32);
        POSTGMA_SETOFFSET(0);
        cmdSend(&writeDeltaEPostGammacmd, pCompGama+i*64, 32);
        delayMs(40);
        //cmdSend(&readDeltaEACKcmd);
        //cmdSend(&readDeltaEACKcmd);
        cmdSend(&readDeltaEACKcmd);
        //发送 GAMMA MODE 0 ，Channel i，后32字节s
        POSTGMA_SETSIZE(32);
        POSTGMA_SETOFFSETL(32);
        cmdSend(&writeDeltaEPostGammacmd, pCompGama+i*64+32, 32);
        delayMs(40);
        //cmdSend(&readDeltaEACKcmd);
        //cmdSend(&readDeltaEACKcmd);
        cmdSend(&readDeltaEACKcmd);
    }
    //保存SRGB Gamma
    cmdSend(&saveDeltaEGmacmd);
    //退出调试模式
    cmdSend(&exitDeltaEDebugcmd);
    //写ColorMatrix Mode：SRGB 18Byte
    cmdSend(&enterDeltaEDebugcmd);
    COLORMATRIX_SETMODE(0);
    COLORMATRIX_SETSIZE(18);
    cmdSend(&writeDeltaEColorMatrixcmd, psRgbCm, 18);
    //delayMs(40);
    //cmdSend(&readDeltaEACKcmd);
    //cmdSend(&readDeltaEACKcmd);
    cmdSend(&readDeltaEACKcmd);
    //退出调试模式
    return cmdSend(&exitDeltaEDebugcmd);
}

bool DeltaEInterface::sRGB_DeltaEAdjustStep0()
{
    if(isStatus(FUNC_ADJUST))
    {
        m_pdata->loadPanelNativeData();
    }
    return true;
}
bool DeltaEInterface::cmdSend(QString CmdStr)
{
    if (!i2cdevice->gethandle())
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
    sRGB_DeltaERunstep1(); //useless
    sRGB_DeltaERunstep2(); //measure ca210
    sRGB_DeltaERunstep3(); //monitor exit debug mode
    return sRGB_DeltaERunstep4(); //adjust
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
    return true;
}

bool DeltaEInterface::sRGB_DeltaERunstep2()
{
    return sRGB_DeltaEVerifyStep2();
}

bool DeltaEInterface::sRGB_DeltaERunstep3()
{
    cmdSend(&exitDeltaEDebugcmd);
    cmdSend(&exitDeltaEAutoGammacmd);
    return true;
}

bool DeltaEInterface::sRGB_DeltaERunstep4()
{
    return sRGB_DeltaEAdjust();
}

