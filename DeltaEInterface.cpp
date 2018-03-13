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
    i2cdevice = new Isp_I2C();
    m_pdata = new Data(this);
    pMstGenGma = new MstGenGmaCtr();
    m_pDDCprotocol = new DDCProtocol_T(*i2cdevice);
    m_isConnect = false;
    if(!pCa210 || !pMstGenGma || !i2cdevice ||!m_pdata || !m_pDDCprotocol)
    {
        exit(-1);
    }

    m_pBurnsettings = m_pdata->getBurnSetting();
    m_pDDCprotocol->setSlaveAddr(m_pBurnsettings->m_slaveaddr);
    m_transfer = new Transfer_T(*m_pDDCprotocol,m_pBurnsettings->m_perpackretrycnt);

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
    if(connectI2C()&& connectCa210())
    {
        m_isConnect = true;
    }
    return m_isConnect;
}
bool DeltaEInterface::connectCa210()
{
    QString temp;

    if(pCa210->caConnect(CA210_CHANNEL))
    {
        pCa210->caSetSyncMode(0); //ntsc
        pCa210->caSetSpeed(1); //fast

        BYTE b1CAType[20] = {0};
        BYTE b1CAVersion[20] = {0};

        pCa210->caGetCATypeName(b1CAType);
        pCa210->caGetCAVersionName(b1CAVersion);
        temp.sprintf("Connect %s, Version: %s", b1CAType, b1CAVersion);
        showMsg(temp, LOG_PASS);

        return true;
    }
    showMsg("ca210 connect Fail !!!", LOG_ERROR);
    return false;
}

void DeltaEInterface::dteDisConnect()
{
    if(!m_isConnect)
    {
        showMsg("pls connect...", LOG_ERROR);
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
        showMsg("pls connect...", LOG_ERROR);
        return m_isConnect;
    }
    //step1. Verify
    if(dteCheck())
    {
        return true;
    }
    delayMs(300);
    //step2. if Verify failed, run to adjust.
    dteAdjust();
    delayMs(300);
    //step3. Verify again
    return dteCheck();
}

int DeltaEInterface::dteCheck()
{
    if(!m_isConnect)
    {
        showMsg("pls connect...", LOG_ERROR);
        return m_isConnect;
    }
    setStatus(FUNC_CHECK);
    pCa210->caSetChannel(0);
    pCa210->caSetSyncMode(0); //ntsc
    pCa210->caSetSpeed(2); //slow
    return sRGB_DeltaEVerify();;
}

bool DeltaEInterface::dteAdjust()
{
    if(!m_isConnect)
    {
        showMsg("pls connect...", LOG_ERROR);
        return m_isConnect;
    }
    //run to adjust
    setStatus(FUNC_ADJUST);
    pCa210->caSetChannel(2);
    pCa210->caSetSyncMode(0); //ntsc
    pCa210->caSetSpeed(2); //fast
    return sRGB_DeltaERun();
}
void DeltaEInterface::dteTest(int r, int g, int b)
{
    LPCA210DATASTRUCT pca210;
    QString temp;
    cRGB_t testPat = {r, g, b};
    if(!m_isConnect)
    {
        showMsg("pls connect...", LOG_ERROR);
        return;
    }
    setStatus(FUNC_RUN);
    pCa210->caSetChannel(1);
    pCa210->caSetSyncMode(0); //ntsc
    pCa210->caSetSpeed(2); //fast
    cmdSend(&enterDeltaEDebugcmd);
    cmdSend(&enterDeltaEAutoGammacmd);
    sendPattern(testPat);//SEND 100% White pattern
    delayMs(300);//延时ms
    pca210 = pCa210->caMeasure();

    temp.sprintf("Test : %.16lf, %.16lf, %.16lf, %.16lf, %.16lf",
                 pca210->fX, pca210->fY, pca210->fZ, pca210->fSx, pca210->fSy);
    showMsg(temp);
    cmdSend(&exitDeltaEDebugcmd);
    cmdSend(&exitDeltaEAutoGammacmd);
}

bool DeltaEInterface::connectI2C()
{
    if(i2cdevice->gethandle())
    {
        i2cdevice->closeDevice(i2cdevice->gethandle());
    }

    DEBUGMSG("the i2c speed:%d",m_pBurnsettings->getI2cSpeed());
    if (i2cdevice->openDevice(i2cdevice->gethandle(),m_pBurnsettings->getI2cSpeed()*1000.0f) == FTC_SUCCESS)
    {
        return true;
    }
    else
    {
        showMsg("[I2C] connect I2C fail!! ", LOG_ERROR);
        return false;
    }
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
void DeltaEInterface::showMsg(QString msg, LOGTEXTTYPE_t logType)
{
    backupMsg.clear();
    backupMsg.append(msg);
    emit updateMsgSignal(logType);
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
       showMsg("please connect Ca210...", LOG_ERROR);
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
    temp.sprintf("Pattern Count: %d", m_pdata->pat_RgbCount);
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

    temp.sprintf("100 White Y : %lf", m_d100W_Raw_Y);
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
            temp.sprintf("%d : %lf", pattern_index+1,result);
            showMsg(temp);
        }
        else
        {
            m_pdata->setPanelNativeData((int)pattern_index/(m_pdata->pat_Level), pattern_index%m_pdata->pat_Level,
                                    pca210->fX, pca210->fY, pca210->fZ, pca210->fSx, pca210->fSy);
            temp.sprintf("%d,%d(%d, %d, %d): %.16lf",  pattern_index/(m_pdata->pat_Level),
                          pattern_index%m_pdata->pat_Level,
                         m_pdata->pat_RgbList[pattern_index].red,
                         m_pdata->pat_RgbList[pattern_index].green,
                         m_pdata->pat_RgbList[pattern_index].blue,
                         pca210->fX);
            showMsg(temp);
        }
    }

    if(isStatus(FUNC_CHECK))
    {
        m_pdata->saveDeltaEData();
    }
    else
    {
        m_pdata->savePanelNativeData();
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
    temp.sprintf("result is %lf", sRGBResult);
    if(sRGBResult < CIE94_RESULT)
    {
        showMsg(temp, LOG_PASS);
        return true;
    }
    showMsg(temp, LOG_ERROR);
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
    double nativeGammaTbl[3][gammaEntris];
    double fixGammaTbl[3][gammaEntris];
    //nativeDataFmtType=0:9_ptn,1:17_ptn,2:33_ptn,3:65_ptn,4:129_ptn,5:256_ptn,
    //6 : RGB_1ptn_W_9ptn
    const int nativeDataFmtType = m_pdata->pat_Level;
    //gamutType = 0: disable, 1 : sRGB(gma 2.2), 2 : adobeRGB(gma 2.2), 3 : BT709(gma 2.4),
    //4 : BT2020(gma 2.4), 5 : DCI_P3(gma 2.6), 6 : User_define(gma 2.2)
    int gamutType = 1;
    //Set compression type to none data loss compression method.
    const int compGmaType = 0;// 0 : 64 bytes ,1 : 76 bytes

    const int gammaTrackType = 0; // 0:N/A, 1:User_define, 2:DICOM
    const int ctTrackType = 0; //0:N/A., 1:Panel native, 2:User_define
    const double sx = 0.3127; //sy, sy target of color temperature 6500k
    const double sy = 0.329;
    const double gammaPower = 2.2;//based on gamut type

    m_pdata->loadPanelNativeData();
    m_pdata->getPanelNativeData(GmaIn);
    pMstGenGma->msetGammaEntries(gammaEntris);
    pMstGenGma->msetPanelNativeData(GmaIn, 0);
    pMstGenGma->msetGamutType(gamutType);
    pMstGenGma->msetGammaTrackType(gammaTrackType);
    pMstGenGma->msetGammaPower(gammaPower);
    pMstGenGma->msetColorTempTrackType(ctTrackType);
    pMstGenGma->msetColorTemperature(6500);
    pMstGenGma->msetColorTemperatureXY(sx,sy);
    pMstGenGma->mmstSetDarkModifySettings(0, 1, 1);
    pMstGenGma->msetBrightModifySettings(0, 255);
    //pMstGenGma->msetDarkModifySettings(4, 0);
    //pMstGenGma->mmstSetMeasPtnNum(nativeDataFmtType);

    //pMstGenGma->mmstSetCompressType(compGmaType);

    //pMstGenGma->msetBrightModifySettings(0, 255);

    pMstGenGma->mmstSetTargetGamut(0, 0.64, 0.33);//R
    pMstGenGma->mmstSetTargetGamut(1, 0.3, 0.6);//G
    pMstGenGma->mmstSetTargetGamut(2, 0.15, 0.06);//B
    pMstGenGma->mmstSetTargetGamut(3, 0.3127, 0.329);//W
    pMstGenGma->mmstGenGamutData(GmaIn, gammaEntris, gamutType, outCompressBuf,sRgbCM);

    //pMstGenGma->mmstGenUserGamutData(GmaIn, outCompressBuf, sRgbCM);
    pMstGenGma->mgenerateGamma();
    pMstGenGma->mgetGammaData(&nativeGammaTbl[0][0]);

    //gamutType = 0;
    //pMstGenGma->msetGamutType(gamutType);
    //pMstGenGma->msetGammaTrackType(gammaTrackType);
    //pMstGenGma->msetGammaPower(gammaPower);
    //pMstGenGma->msetColorTempTrackType(ctTrackType);
   // pMstGenGma->msetColorTemperatureXY(sx,sy);
    //pMstGenGma->mgenerateGamma();

    pMstGenGma->mgetCompressedGmaData(compGmaType, outCompressBuf);
    //pMstGenGma->mgetGammaData(&fixGammaTbl[0][0]);
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
        POSTGMA_SETOFFSET(32);
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
    if(isStatus(FUNC_ADJUST) || isStatus(FUNC_RUN))
    {
        m_pdata->loadPanelNativeData();
    }
    return true;
}
bool DeltaEInterface::cmdSend(QString CmdStr)
{
    if (!i2cdevice->gethandle())
    {
        showMsg("pleas open device first!!", LOG_ERROR);
        return false;
    }

    //first:find all the substring and judge the length. must less than 2
    QStringList cmdlist = CmdStr.split(' ', QString::SkipEmptyParts);

    for(auto x:cmdlist)
    {
        if(x.length()>2)
        {
            showMsg("Commands format error.", LOG_ERROR);
            return false;
        }
    }
    if(cmdlist.size()>40)
    {
        showMsg("you may send too much.", LOG_ERROR);
        return false;
    }
    if(cmdlist.size()==0)
    {
        showMsg("Just type something,man~", LOG_ERROR);
        return false;
    }

    quint8* ins = new quint8[cmdlist.size()];
    bool ok;
    showMsg("\n");
    showMsg("User defined Instrucitons:");
    QString _usrstr;
    for (int i = 0; i < cmdlist.size(); ++i)
    {
        ins[i] = cmdlist.at(i).toInt(&ok,16);
        _usrstr.append(cmdlist.at(i));
        _usrstr.append(" ");
    }

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
    showMsg(msg, LOG_NONE);
}

bool DeltaEInterface::sRGB_DeltaERun()
{
    if(!pCa210 || !pCa210->isConnect())
    {
       showMsg("please connect Ca210 or I2c device...", LOG_ERROR);
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
        delayMs(300);
        return sRGB_DeltaEVerifyStep0();
    }
    showMsg("DeltaE Debug FAIL！！", LOG_ERROR);
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

