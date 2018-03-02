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
    pCa210 = new Ca210DllCtr(CA210DLL);
    pMstGenGma = new MstGenGmaCtr();
    if(!pCa210 || !pMstGenGma)
    {
        exit(-1);
    }
    readI2CSetting();
    i2cdevice = new Isp_I2C();
    m_pdata = new Data(this);
    if(m_pdata == NULL)
    {
        exit(-1);
    }
}

DeltaEInterface::~DeltaEInterface()
{
    if(pCa210)
    {
        delete pCa210;
    }
    if(pMstGenGma)
    {
        delete pMstGenGma;
    }
    if(i2cdevice)
    {
        if(i2cdevice->gethandle())
        {
            i2cdevice->closeDevice(i2cdevice->gethandle());
        }
        delete i2cdevice;
    }
}

bool DeltaEInterface::dteConnect()
{
    return connectI2C();
    if(pCa210)
    {
        return pCa210->caConnect(CA210_CHANNEL);
    }
    return false;
}

int DeltaEInterface::dteRun()
{
    return sRGB_DeltaEAdjust();
}

int DeltaEInterface::dteCheck()
{
    return sRGB_DeltaEVerify();;
}

bool DeltaEInterface::dteAdjust()
{
    return pCa210->caSetChannel(0);
}
bool DeltaEInterface::connectI2C()
{
    DEBUGMSG("the i2c speed:%d",burnsettings->getI2cSpeed());
    if (i2cdevice->openDevice(i2cdevice->gethandle(),burnsettings->getI2cSpeed()*1000.0f) == FTC_SUCCESS)
    {
        return true;
    }
    else
    {
        return true;
    }
}
void DeltaEInterface::readI2CSetting()
{
    QSettings Burn_Settings("Cvte","DeltaE Tool");

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

    burnsettings = new BurnSetting_T(tmp_slaveaddr,tmp_I2cSpeed,tmp_WriteDelay,tmp_ReadDelay,tmp_RetryCnt,tmp_PerPackRetryCnt,
    tmp_EdidlastDelay,tmp_HdcplastDelay,tmp_EraseHdcp,(bool)tmp_IsCreatLogs);

    Burn_Settings.endGroup();

    QSettings At_cmds("Cvte","DeltaE Tool");
    At_cmds.beginGroup("AT_Cmd");

    At_cmds.endGroup();
}
QString DeltaEInterface::getBackupMsg()
{
    return backupMsg;
}

void DeltaEInterface::sendPattern(cRGB_t rgb)
{
    emit sendPatSignal(rgb);
}
void DeltaEInterface::showMsg(QString msg)
{
    backupMsg.clear();
    backupMsg.append(msg);
    emit updateMsgSignal();
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
    temp.sprintf("100\% White Y : %lf \n", m_d100W_Raw_Y);
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
    sRGB_DeltaEAdjustStep1();// generate gamma
    return sRGB_DeltaEAdjustStep2();
}
bool DeltaEInterface::sRGB_DeltaEAdjustStep0()
{
    int i, j;
    QString temp, showtemp;
#if 0
    const int gammaEntries = 256;
    BYTE outCompressBuf[64*3];
    double nativeGammaTbl[3][gammaEntries] = {0};
    double fixedGammaTbl[3][gammaEntries] = {0};
    //nativeDataFmtType = 0 : 9_ptn, 1 : 17_ptn, 2 : 33_ptn, 3 :
    // 65_ptn, 4 : 129_ptn, 5 : 256_ptn, 6 : 12_ptn (LG used)
    const int nativeDataFmtType = 0;
    //gamutType = 0: N/A, 1 : sRGB(gma 2.2), 2 : adobeRGB(gma 2.2), 3 : BT709(gma
    // 2.4), 4 : BT2020(gma 2.4), 5 : DCI_P3(gma 2.6), 6 : User_define(gma 2.2)
    int gamutType = 1;
    const int gammaTrackType = 1; //gammaTrackType = 0: N/A, 1 : User_define, 2 : DICOM
    // based on gamut type, ex: if gamut is sRGB , then gammaPower set 2.2 ,
    // if gamut is BT709, then gammaPower set 2.4 and so on.
    const double gammaPower = 2.2;
    //ctTrackType=0:N/A,1:Panelnative,2:User_define(FixedTarget)
    const int ctTrackType=2;
    const double sx = 0.3127; //sx, sy target of color temperature 6500K
    const double sy = 0.329;
    const int compGmaType = 2; //for normal gamma set to 0, for dicom gamma set to 1

    //Step 0. Get measrued RGBW XYZxy data. Its formats is as follow.
    //Please reference Native_L9.txt
    //X_R0/X_R32/.. ~ X_R255
    //Y_R0/Y_R32/.. ~ Y_R255
    //Z_R0/Z_R32/.. ~ Z_R255
    //sx_R0/sx_R32/.. ~ sx_R255
    //sy_R0/sy_R32/.. ~ sy_R255
    //....
    double Native_RGBW_XYZxy[9 * 4 * 5] = {0};
    pMstGenGma->mGetPanelNativeData(Native_RGBW_XYZxy);
    pMstGenGma->msetGammaEntries(gammaEntries);
    pMstGenGma->msetPanelNativeData(Native_RGBW_XYZxy, nativeDataFmtType);
    pMstGenGma->msetGamutType(gamutType);
    pMstGenGma->msetDarkModifySettings(4, 0);
    //Get native gamma
    pMstGenGma->mgenerateGamma();
    //pMstGenGma->mgetCompressedGmaData(compGmaType, &outCompressBuf[0][0]);
    pMstGenGma->mgetGammaData(&nativeGammaTbl[0][0]);
    for(i = 0; i < 3*64; i++)
    {
        temp.clear();
        temp.sprintf("%d,",outCompressBuf[i]);
        if(i % 8 == 0)
        {
            temp.append("\n");
        }

        showMsg(temp);
    }
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < gammaEntries; j++)
        {
            temp.clear();
            temp.sprintf("%lf,",nativeGammaTbl[i][j]);

            if(j % 8 == 0)
            {
                temp.append("\n");
            }
            showtemp.append(temp);
        }
        showtemp.append("\n\n");
        showMsg(showtemp);
    }
 #else
    double GmaIn[4*5*9];
    double gmaOut[3][256];
    BYTE outCompressBuf[64*3];
    BYTE sRgbCM[19];
    const int gammaEntris = 256;
    const int gamutType = 1;
    pMstGenGma->mmstSetMeasPtnNum(9);
    pMstGenGma->mmstSetCompressType(0);// 0 : 64 bytes ,1 : 76 bytes
    pMstGenGma->mGetPanelNativeData(GmaIn);
    //Generate gamma curve and color matrix
    pMstGenGma->mmstGenGammaData(GmaIn, outCompressBuf,sRgbCM);
    //pMstGenGma->mmstGenGamutData(GmaIn, gammaEntris, gamutType, outCompressBuf,sRgbCM);

    pMstGenGma->mmstGetUnCompressedGammaData(&gmaOut[0][0]);
    for(i = 0; i < 3*64; i++)
    {
        temp.clear();
        temp.sprintf("%d,",outCompressBuf[i]);
        if(i !=0 && i % 8 == 0)
        {
            temp.append("\n");
        }

        showMsg(temp);
    }
    showMsg("\n\n");
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 256; j++)
        {
            temp.clear();
            temp.sprintf("%lf,",gmaOut[i][j]);

            if(j !=0 && j % 8 == 0)
            {
                temp.append("\n");
            }
            showtemp.append(temp);
        }
        showtemp.append("\n\n");
        showMsg(showtemp);
    }
   #endif
    return true;
}
bool DeltaEInterface::sRGB_DeltaEAdjustStep1()
{
    QString temp, showtemp;
    int i, j;
    const int gammaEntries = 256;
    BYTE commpressedGammaTbl[3][64] = {0};
    double gammaTbl[3][gammaEntries] = {0};
    BYTE SRGB_CM[18] = {0};
    //nativeDataFmtType=0:9_ptn,1:17_ptn,2:33_ptn,3:65_ptn,4:129_ptn,5:256_ptn,
    //6 : RGB_1ptn_W_9ptn
    const int nativeDataFmtType = 0;
    //gamutType = 0: disable, 1 : sRGB(gma 2.2), 2 : adobeRGB(gma 2.2), 3 : BT709(gma 2.4),
    //4 : BT2020(gma 2.4), 5 : DCI_P3(gma 2.6), 6 : User_define(gma 2.2)
    const int gamutType = 1;
    //gammaTrackType = 0: disable, 1 : User_define, 2 : DICOM
    const int gammaTrackType = 1;
    //ctTrackType = 0: disable, 1 : Panel native, 2 : User_define(Fixed Target)
    const int ctTrackType = 2;
    //for normal gamma set to 0, for dicom gamma set to 1
    const int compGmaType = 0;
    double Native_RGB_XYZxy[4*5*9] = {0};

    if(pMstGenGma)
    {
        //Assign measured XYZxy data to Native_RGBW_XYZxy
        pMstGenGma->mGetPanelNativeData(Native_RGB_XYZxy);
        //step 1. set output gamma entries
        pMstGenGma->msetGammaEntries(gammaEntries);
        //step 2. set panel native data (measured XYZxy data) with specified format type
        pMstGenGma->msetPanelNativeData(Native_RGB_XYZxy, nativeDataFmtType);
        //step 3. set calibration gamut target
        pMstGenGma->msetGamutType(gamutType);
        //step 4. set calibration gamma target
        pMstGenGma->msetGammaTrackType(gammaTrackType);
        //step 5. set calibration color temperature target
        pMstGenGma->msetColorTempTrackType(ctTrackType);
        //step 4. set dark modified method for gamma lut. (depend on real case)
        pMstGenGma->msetDarkModifySettings(1, 16);//Linear
        //step 6. generate gamma data & get gamma data
        pMstGenGma->mgenerateGamma();
        pMstGenGma->mgetCompressedGmaData(compGmaType, &commpressedGammaTbl[0][0]);
        pMstGenGma->mgetGammaData(&gammaTbl[0][0]);
        pMstGenGma->mgetColorMatrixData(SRGB_CM);
        for(i = 0; i < 3; i++)
        {
            if(i == 0) showtemp.append("R\n");
            else if(i==1)showtemp.append("G\n");
            else showtemp.append("B\n");
            for(j = 0; j < 64; j++)
            {
                temp.clear();
                temp.sprintf("0x%x, ",commpressedGammaTbl[i][j]);

                if((j+1) % 4 == 0)
                {
                    temp.append("\n");
                }
                showtemp.append(temp);
            }
            showtemp.append("\n\n");
            showMsg(showtemp);
            showtemp.clear();
        }
        showMsg("\n\n\n");
        for(i = 0; i < 3; i++)
        {
            for(j = 0; j < gammaEntries; j++)
            {
                temp.clear();
                temp.sprintf("%lf,",gammaTbl[i][j]);

                if((j+1) % 8 == 0)
                {
                    temp.append("\n");
                }
                showtemp.append(temp);
            }
            showtemp.append("\n\n");
            showMsg(showtemp);
            showtemp.clear();
        }
        showMsg("\n\n\n");
        for(i = 0; i < 18; i++)
        {
            temp.clear();
            temp.sprintf("0x%x,",SRGB_CM[i]);

            if((i+1) % 6 == 0)
            {
                temp.append("\n");
            }
            showMsg(temp);
        }
        showMsg("\n");
        return true;

    }
    return false;
}

bool DeltaEInterface::sRGB_DeltaEAdjustStep2()
{
    return false;
}


