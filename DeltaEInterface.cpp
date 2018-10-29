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
    setStatus(FUNC_NONE);
    setAdjType(ADJ_MEASURE);
    m_delayTimeMs = DEF_DALEY_MS;
    pCa210 = new Ca210DllCtr(CA210DLL);
    i2cdevice = new Isp_I2C();
    m_pdata = new Data(this);
    pMstGenGma = new MstGenGmaCtr();
    m_pDDCprotocol = new DDCProtocol_T(*i2cdevice);
    m_isConnect = false;
    m_bI2cConnect = false;
    m_bCa210Connect = false;
    m_standard = CIE94_RESULT;
    if(!pCa210 || !pMstGenGma || !i2cdevice ||!m_pdata || !m_pDDCprotocol)
    {
        exit(-1);
    }

    m_pBurnsettings = m_pdata->getBurnSetting();
    m_pDDCprotocol->setSlaveAddr(m_pBurnsettings->m_slaveaddr);
    m_transfer = new Transfer_T(*m_pDDCprotocol,m_pBurnsettings->m_perpackretrycnt);

    connect(m_transfer, SIGNAL(send_debugMsg(QString)), this, SLOT(recvMsg(QString)));
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
void DeltaEInterface::setConnectFlag(bool bcnt)
{
    m_isConnect = bcnt;
}

void DeltaEInterface::setstandardResult(float value)
{
    m_standard = value;
}

bool DeltaEInterface::IsConnectCA210()
{
    return m_bCa210Connect;
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
    m_pdata->loadCA210Setting();
    if(pCa210->caConnect(m_pdata->ca210Setting->checkChannel))
    {
        pCa210->caSetSyncMode(m_pdata->ca210Setting->checkSyncMode); //ntsc
        pCa210->caSetSpeed(m_pdata->ca210Setting->checkSpeed); //fast

        BYTE b1CAType[20] = {0};
        BYTE b1CAVersion[20] = {0};

        pCa210->caGetCATypeName(b1CAType);
        pCa210->caGetCAVersionName(b1CAVersion);
        temp.sprintf("已连接 %s, 版本: %s", b1CAType, b1CAVersion);
        showMsg(temp, LOG_PASS);
        m_bCa210Connect = true;
    }else
    {
        m_bCa210Connect = false;
        showMsg("连接色温仪（ca210）失败!!!", LOG_ERROR);
    }
    return m_bCa210Connect;
}

void DeltaEInterface::dteDisConnect()
{
    if(!m_isConnect)
    {
        showMsg("请连接串口或色温仪...", LOG_ERROR);
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
        showMsg("请连接串口或色温仪...", LOG_ERROR);
        return m_isConnect;
    }
    //step1. Verify
    if(dteCheck())
    {
        return true;
    }
    //delayMs(300);
    //step2. if Verify failed, run to adjust.
    setAdjType(ADJ_MEASURE);
    return dteAdjust();
    //delayMs(300);
    //step3. Verify again
    //return dteCheck();
}

int DeltaEInterface::dteCheck()
{
    if(!m_isConnect)
    {
        showMsg("请连接串口或色温仪...", LOG_ERROR);
        return m_isConnect;
    }
    setStatus(FUNC_CHECK);
    pCa210->caSetChannel(m_pdata->ca210Setting->checkChannel);
    pCa210->caSetSyncMode(m_pdata->ca210Setting->checkSyncMode); //ntsc
    pCa210->caSetSpeed(m_pdata->ca210Setting->checkSpeed); //slow
    return sRGB_DeltaEVerify();;
}

bool DeltaEInterface::dteAdjust()
{
    bool result = false;
    if(!m_isConnect)
    {
        showMsg("请连接串口或色温仪...", LOG_ERROR);
        return m_isConnect;
    }
    //run to adjust
    setStatus(FUNC_ADJUST);
    pCa210->caSetChannel(m_pdata->ca210Setting->adjustChannel);
    pCa210->caSetSyncMode(m_pdata->ca210Setting->adjustSyncMode); //ntsc
    pCa210->caSetSpeed(m_pdata->ca210Setting->adjustSpeed);  //fast
    if(isAdjType(ADJ_MEASURE))
    {
        result = sRGB_DeltaERun();
    }
    else
    {
        result = sRGB_DeltaEAdjust();
    }
    if(result == false)
    {
        showMsg("调节失败...", LOG_ERROR);
    }
    return result;
}
void DeltaEInterface::dteTest(int r, int g, int b)
{
    LPCA210DATASTRUCT pca210;
    QString temp;
    cRGB_t testPat = {r, g, b};
    if(!m_isConnect)
    {
        showMsg("请连接串口或色温仪...", LOG_ERROR);
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
bool DeltaEInterface::IsConnectI2C()
{
    return m_bI2cConnect;
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
        showMsg("连接串口成功!! ", LOG_PASS);
        m_bI2cConnect = true;
    }
    else
    {
        showMsg("连接串口失败!! ", LOG_ERROR);
        m_bI2cConnect = false;
    }
    return m_bI2cConnect;
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
void DeltaEInterface::setAdjType(AdjustType_t adjtype)
{
    m_adjType = adjtype;
}

bool DeltaEInterface::isAdjType(AdjustType_t adjtype)
{
    return (m_adjType == adjtype);
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
       showMsg("请连接色温仪...", LOG_ERROR);
       return false;
    }
    sRGB_DeltaEVerifyStep0(); //0. load pattern
    sRGB_DeltaEVerifyStep1(); //1. get 100% wihte Y
    showMsg("测量中....");
    sRGB_DeltaEVerifyStep2(); //2. measure gamma data
    return sRGB_DeltaEVerifyStep3(); //3. check the result
}
bool DeltaEInterface::sRGB_DeltaEVerifyStep0()
{
    QString temp;
    sRGBResult = 0;
    if(isStatus(FUNC_CHECK))
    {
        m_delayTimeMs = m_pdata->ca210Setting->checkDelayms;
    }else
    {
        m_delayTimeMs = m_pdata->ca210Setting->adjustDelayms;;
    }
    m_pdata->update_PatRgb(isStatus(FUNC_CHECK)); //load pattern file
    temp.sprintf("测试Pattern阶数: %d", m_pdata->pat_RgbCount);
    showMsg(temp);
    return true;
}

bool DeltaEInterface::sRGB_DeltaEVerifyStep1()
{
    //XYZCOLOR color={0,0,0};
    LPCA210DATASTRUCT pca210;
    //QString temp;
    sendPattern(pat_RGBW[3]);//SEND 100% White pattern
    delayMs(m_delayTimeMs);//延时ms
   // color = pCa210->caGetAverageMeasureXYZ(3);//获取CA210 的XYZ值
    pca210 = pCa210->caMeasure();
    m_d100W_Raw_Y = pca210->fY;

    //temp.sprintf("100 White Y : %lf", m_d100W_Raw_Y);
    //showMsg(temp);
    return true;
}

bool DeltaEInterface::sRGB_DeltaEVerifyStep2()
{
    //XYZCOLOR color={0,0,0};
    LPCA210DATASTRUCT pca210;
    //QString temp;
    int pattern_index=0;
    float result;
    bool ret = false;

    for(pattern_index = 0;pattern_index < m_pdata->getPatternCount(); pattern_index++)
    {
        sendPattern(m_pdata->pat_RgbList[pattern_index]);//发送测试Pattern
        delayMs(m_delayTimeMs);//延时ms
        pca210 = pCa210->caMeasure();
        if(pca210 == NULL)
        {
            return false;
        }
        if(isStatus(FUNC_CHECK))
        {
            result = GetDeltaE_OnePatCIE94(pattern_index, pca210->fX, pca210->fY, pca210->fZ, m_d100W_Raw_Y);
            sRGBResult += result;
          //  temp.sprintf("%d : %lf", pattern_index+1,result);
          //  showMsg(temp);
        }
        else
        {
            m_pdata->setPanelNativeData((int)pattern_index/(m_pdata->pat_Level), pattern_index%m_pdata->pat_Level,
                                    pca210->fX, pca210->fY, pca210->fZ, pca210->fSx, pca210->fSy);
            /*if((pca210->fX > 100)|| (pca210->fY > 100) )
            {
                showMsg("检测数据出现异常，请检测CA210是否对准屏幕", LOG_ERROR);
                return false;
            }*/
        }
    }

    if(isStatus(FUNC_CHECK))
    {
        ret = m_pdata->saveDeltaEData();
    }
    else
    {
        ret = m_pdata->savePanelNativeData();
    }
    return ret;
}

bool DeltaEInterface::sRGB_DeltaEVerifyStep3()
{
    QString temp;
    if(sRGBResult > 0)
    {
        sRGBResult = sRGBResult/m_pdata->pat_RgbCount;
    }
    temp.sprintf("结果为 %lf", sRGBResult);
    if(sRGBResult < m_standard)
    {
        showMsg(temp, LOG_PASS);
        return true;
    }
    showMsg(temp, LOG_ERROR);
    return false;
}


bool DeltaEInterface::sRGB_DeltaEAdjust()
{
    bool ret = false;
    ret = sRGB_DeltaEAdjustStep0();//load native data
    if(ret == false)
    {
        showMsg("文件加载失败，请重试", LOG_ERROR);
        return false;
    }
    showMsg("生成gamma数据....");
    sRGB_DeltaEAdjustStep1();// generate gamma
    if(ret == false)
    {
        showMsg("算法运行失败，请重试", LOG_ERROR);
        return false;
    }
    showMsg("写入gamma数据到Monitor....");
    ret = sRGB_DeltaEAdjustStep2();//send gamma to monitor
    if(ret == false)
    {
        cmdSend(&exitDeltaEDebugcmd);
        showMsg("串口通信失败，请检查串口连接重试", LOG_ERROR);
    }
    return ret;
}
bool DeltaEInterface::sRGB_DeltaEAdjustStep1()
{
    bool ret = false;
    double GmaIn[4*5*m_pdata->deltaEStting->patternLevel];
    const int compDataSize = m_pdata->deltaEStting->compressSize;
    const int sRgbCMDataSize = m_pdata->deltaEStting->colorMetrixSize;
    BYTE outCompressBuf[compDataSize*3];
    BYTE sRgbCM[sRgbCMDataSize];
    const int gammaEntris = m_pdata->deltaEStting->gammaEntris;
    //nativeDataFmtType=0:9_ptn,1:17_ptn,2:33_ptn,3:65_ptn,4:129_ptn,5:256_ptn,
    //6 : RGB_1ptn_W_9ptn
    const int nativeDataFmtType = m_pdata->deltaEStting->nativeDataFmtType;
    //gamutType = 0: disable, 1 : sRGB(gma 2.2), 2 : adobeRGB(gma 2.2), 3 : BT709(gma 2.4),
    //4 : BT2020(gma 2.4), 5 : DCI_P3(gma 2.6), 6 : User_define(gma 2.2)
    const int gamutType = m_pdata->deltaEStting->gamutType;
    //Set compression type to none data loss compression method.
    const int compGmaType = m_pdata->deltaEStting->compGmaType;// 0 : 64 bytes ,1 : 76 bytes

    const int gammaTrackType = m_pdata->deltaEStting->gammaTrackType; // 0:N/A, 1:User_define, 2:DICOM
    const int ctTrackType = m_pdata->deltaEStting->colorTempTrackType; //0:N/A., 1:Panel native, 2:User_define
    const double sx = m_pdata->deltaEStting->ctSy; //sy, sy target of color temperature 6500k
    const double sy = m_pdata->deltaEStting->ctSx;
    const int  colorTemp = m_pdata->deltaEStting->colorTemperature;
    const double gammaPower = m_pdata->deltaEStting->gammaPower;//based on gamut type
    const int brightModifyMode = m_pdata->deltaEStting->brightModifyMode;
    const int brightModifyLevel = m_pdata->deltaEStting->brightModifyLevel;
    const int darkModifyMode = m_pdata->deltaEStting->darkModifyMode;
    const int darkModifyLevel = m_pdata->deltaEStting->darkModifyLevel;
    const double targetRx = m_pdata->deltaEStting->targetGamutRx;
    const double targetRy = m_pdata->deltaEStting->targetGamutRy;
    const double targetGx = m_pdata->deltaEStting->targetGamutGx;
    const double targetGy = m_pdata->deltaEStting->targetGamutGy;
    const double targetBx = m_pdata->deltaEStting->targetGamutBx;
    const double targetBy = m_pdata->deltaEStting->targetGamutBy;
    const double targetWx = m_pdata->deltaEStting->targetGamutWx;
    const double targetWy = m_pdata->deltaEStting->targetGamutWy;
    const float  maxBrightnessRatio = m_pdata->deltaEStting->maxBrightnessRatio;
    //QString temp;
    //temp.sprintf("gammaEntris:%d, nativeDataFmtType:%d, gamutType:%d,darkModifyLevel:%d "
     //            , gammaEntris, nativeDataFmtType, gamutType, darkModifyLevel);
    //showMsg(temp);
    //step 0. measured XYZxy NativeData
    ret = m_pdata->loadPanelNativeData();
    if(ret == false)
    {
        return ret;
    }
    m_pdata->getPanelNativeData(GmaIn);
    //step 1. set output gamma entries
    pMstGenGma->msetGammaEntries(gammaEntris);
    //step 2. set panel native data with specified format type
    pMstGenGma->msetPanelNativeData(GmaIn, nativeDataFmtType);
    //step 3. set calibration gamut target
    pMstGenGma->msetGamutType(gamutType);
    pMstGenGma->msetGammaTrackType(gammaTrackType);
    pMstGenGma->msetGammaPower(gammaPower);
    //step 4. set calibration color temperature target
    if(m_pdata->deltaEStting->colorTempTrackTypeSet)
    {
        pMstGenGma->msetColorTempTrackType(ctTrackType);
        pMstGenGma->msetColorTemperature(colorTemp);
        pMstGenGma->msetColorTemperatureXY(sx,sy);
    }

    //step 5. set dark modified method for gamma 1ut
    if(m_pdata->deltaEStting->darkModifySet)
    {
        pMstGenGma->msetDarkModifySettings(darkModifyMode, darkModifyLevel);
        //pMstGenGma->mmstSetDarkModifySettings(0, 1, 1);
    }
    if(m_pdata->deltaEStting->brightModifySet)
    {
        pMstGenGma->msetBrightModifySettings(brightModifyMode, brightModifyLevel);
        pMstGenGma->msetMaxBrightnessRatio(maxBrightnessRatio);
    }

    if(m_pdata->deltaEStting->targetGamutSet)
    {
        pMstGenGma->mmstSetTargetGamut(COLOR_R, targetRx, targetRy);   //R
        pMstGenGma->mmstSetTargetGamut(COLOR_G, targetGx, targetGy);     //G
        pMstGenGma->mmstSetTargetGamut(COLOR_B, targetBx, targetBy);   //B
        pMstGenGma->mmstSetTargetGamut(COLOR_W, targetWx, targetWy);//W
    }

    //pMstGenGma->mmstSetCompressType(compGmaType);
    //pMstGenGma->mmstGenGamutData(GmaIn, gammaEntris, gamutType, outCompressBuf,sRgbCM);
    //pMstGenGma->mmstGenUserGamutData(GmaIn, outCompressBuf, sRgbCM);
    //step 6. generate gamma data and get gamma data
    pMstGenGma->mgenerateGamma();
    pMstGenGma->mgetCompressedGmaData(compGmaType, outCompressBuf);
    pMstGenGma->mgetColorMatrixData(sRgbCM);
    ret = m_pdata->saveCompGma(outCompressBuf, compDataSize*3);
    if(ret == false)
    {
        showMsg("保存CompGma数据失败", LOG_ERROR);
        return ret;
    }
    ret = m_pdata->saveColorMatrix(sRgbCM, sRgbCMDataSize);
    if(ret == false)
    {
        showMsg("保存ColorMatrix数据失败", LOG_ERROR);
        return ret;
    }
    return true;
}
bool DeltaEInterface::sRGB_DeltaEAdjustStep2()
{
    BYTE *pCompGama;
    BYTE *psRgbCm;
    bool ret = false;
    const int compDataSize = m_pdata->deltaEStting->compressSize;
    const int sRgbCMDataSize = m_pdata->deltaEStting->colorMetrixSize;
    const int packet_1 = (int)compDataSize/2;
    const int packet_2 = compDataSize-packet_1;
    int i = 0;
    pCompGama = m_pdata->getCompGma();
    psRgbCm = m_pdata->getColorMatrix();
    //先发送MS_WR_BLOCK ，进入调试模式
    showMsg("1.发送MS_WR_BLOCK ，进入调试模式");
    ret = cmdSend(&enterDeltaEDebugcmd);
    if(ret == false)
    {
        showMsg("进入调试模式失败", LOG_ERROR);
       // return false;
    }
    for(i = 0; i < 3; i++)
    {
        //先发送 GAMMA MODE 0 ，Channel i，前32字节
        POSTGMA_SETMODE(1);
        POSTGMA_SETCHANEL(i);
        POSTGMA_SETSIZE(packet_1);
        POSTGMA_SETOFFSET(0);
        delayMs(100);
        showMsg("2.发送 GAMMA MODE 0 ，Channel i，前32字节");
        ret = cmdSend(&writeDeltaEPostGammacmd, pCompGama+i*compDataSize, packet_1);
        if(ret == false)
        {
            showMsg("发送前32字节失败", LOG_ERROR);
            return false;
        }
         delayMs(300);
         showMsg("3.读取ACK");
        delayMs(40);
        ret = cmdSend(&readDeltaEACKcmd);
        if(ret == false)
        {
            showMsg("发送前32字节失败", LOG_ERROR);
          //  return false;
        }
        //发送 GAMMA MODE 0 ，Channel i，后32字节s
        POSTGMA_SETSIZE(packet_2);
        POSTGMA_SETOFFSET(packet_1);
        delayMs(100);
        showMsg("4.发送 GAMMA MODE 0 ，Channel i，后32字节");
        ret = cmdSend(&writeDeltaEPostGammacmd, pCompGama+i*compDataSize+packet_1, packet_2);
        if(ret == false)
        {
            showMsg("发送失败后32字节失败", LOG_ERROR);
          //  return false;
        }
        delayMs(300);
        showMsg("6.读取ACK");
        ret = cmdSend(&readDeltaEACKcmd);
        if(ret == false)
        {
            showMsg("发送失败后32字节失败", LOG_ERROR);
            return false;
        }
    }
    //保存SRGB Gamma
    delayMs(60);
    showMsg("7.保存gamma");
    ret = cmdSend(&saveDeltaEGmacmd);
    if(ret == false)
    {
        showMsg("发送gamma数据失败", LOG_ERROR);
       // return false;
    }
    //退出调试模式
    delayMs(100);
    showMsg("8.退出调试模式");
    ret = cmdSend(&exitDeltaEDebugcmd);
    if(ret == false)
    {
     //  return false;
    }

    //写ColorMatrix Mode：SRGB 18Byte
     delayMs(300);
    showMsg("9.进入调试模式");
    ret = cmdSend(&enterDeltaEDebugcmd);
    if(ret == false)
    {
     //   return false;
    }
    COLORMATRIX_SETMODE(0);
    COLORMATRIX_SETSIZE(sRgbCMDataSize);
    delayMs(100);
    showMsg("10.发送color Matix");
    ret = cmdSend(&writeDeltaEColorMatrixcmd, psRgbCm, sRgbCMDataSize);
    if(ret == false)
    {
      //  return false;
    }
    delayMs(300);
    ret = cmdSend(&readDeltaEACKcmd);
    if(ret == false)
    {
      //  return false;
    }
    delayMs(100);
    //退出调试模式
    delayMs(800);
    showMsg("12.进入调试模式");
    return cmdSend(&exitDeltaEDebugcmd);
}

bool DeltaEInterface::sRGB_DeltaEAdjustStep0()
{
    bool ret = false;
    if(isStatus(FUNC_ADJUST) || isStatus(FUNC_RUN))
    {
        ret = m_pdata->loadDeltaESetting();
        if(ret == false)
        {
            showMsg("加载setting.ini失败!!", LOG_ERROR);
            return ret;
        }
        m_pdata->loadPanelNativeData();
        if(ret == false)
        {
            showMsg("加载PanelNativeData失败!!", LOG_ERROR);
            return ret;
        }
    }
    return ret;
}
bool DeltaEInterface::cmdSend(QString CmdStr)
{
    if (!i2cdevice->gethandle())
    {
        showMsg("请先连接设备!!", LOG_ERROR);
        return false;
    }

    //first:find all the substring and judge the length. must less than 2
    QStringList cmdlist = CmdStr.split(' ', QString::SkipEmptyParts);

    for(auto x:cmdlist)
    {
        if(x.length()>2)
        {
            showMsg("命令格式错误.", LOG_ERROR);
            return false;
        }
    }
    if(cmdlist.size()>40)
    {
        showMsg("发送内容过长.", LOG_ERROR);
        return false;
    }
    if(cmdlist.size()==0)
    {
        showMsg("发送的内容不能为空", LOG_ERROR);
        return false;
    }

    quint8* ins = new quint8[cmdlist.size()];
    bool ok;
    showMsg("\n");
    showMsg("自定义命令:");
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
    c.retrycnt = m_pdata->m_pBurnsettings->getRetryCnt();
    c.delay = m_pdata->m_pBurnsettings->getwriteDelay();
    m_transfer->setburnCmd(&c,nullptr,0,1);

    m_transfer->run();
    m_transfer->wait();//wait for the end of transfer thread.
    delete[] ins;
    return m_transfer->getResult();
}

bool DeltaEInterface::cmdSend(burnCmd_t *cmd)
{
    if(m_transfer == NULL)
    {
        return false;
    }
    cmd->retrycnt = m_pdata->m_pBurnsettings->getRetryCnt();
    cmd->delay = m_pdata->m_pBurnsettings->getwriteDelay();
    m_transfer->setburnCmd(cmd,nullptr,0,1);

    m_transfer->run();
    m_transfer->wait();//wait for the end of transfer thread.
    return m_transfer->getResult();
}

bool DeltaEInterface::cmdSend(burnCmd_t *cmd,quint8 *data, quint32 size,quint8 source)
{
    if(m_transfer == NULL)
    {
        return false;
    }
    cmd->retrycnt = m_pdata->m_pBurnsettings->getRetryCnt();
    //cmd->delay = m_pdata->m_pBurnsettings->getwriteDelay();
    m_transfer->setburnCmd(cmd,data,size,source);

    m_transfer->run();
    m_transfer->wait();//wait for the end of transfer thread.
    return m_transfer->getResult();
}

void DeltaEInterface::recvMsg(QString msg)
{
    showMsg(msg, LOG_DEBUG);
}

bool DeltaEInterface::sRGB_DeltaERun()
{
    bool ret = false;
    if(!pCa210 || !pCa210->isConnect())
    {
       showMsg("请连接串口或色温仪...", LOG_ERROR);
       return false;
    }
    ret = sRGB_DeltaERunstep0(); //monitor enter debug mode and load patern
    if(ret == false)
    {
        return ret;
    }
    ret = sRGB_DeltaERunstep1(); //useless
    if(ret == false)
    {
        return ret;
    }
    ret = sRGB_DeltaERunstep2(); //measure ca210
    if(ret == false)
    {
        showMsg("色温检测有误，请检连接重试", LOG_ERROR);
        sRGB_DeltaERunstep3();
        return ret;
    }
    ret = sRGB_DeltaERunstep3(); //monitor exit debug mode
    if(ret == false)
    {
        showMsg("串口发失败，请检查串连接重试！！", LOG_ERROR);
        return ret;
    }
    ret = sRGB_DeltaERunstep4(); //adjust
    return ret;
}

bool DeltaEInterface::sRGB_DeltaERunstep0()
{
    if(cmdSend(&enterDeltaEDebugcmd)
       && cmdSend(&enterDeltaEAutoGammacmd))
    {
        delayMs(300);
        return sRGB_DeltaEVerifyStep0();
    }
    showMsg("串口发失败，请检查串连接重试！！", LOG_ERROR);
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
    bool ret = false;
    ret = cmdSend(&exitDeltaEDebugcmd);
    ret = cmdSend(&exitDeltaEAutoGammacmd);
    return ret;
}

bool DeltaEInterface::sRGB_DeltaERunstep4()
{
    return sRGB_DeltaEAdjust();
}

