#include "DeltaEInterface.h"
#include <QTime>
cRGB_t g_PatternList[RGB_COUT] = {
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
    //return connectI2C();
    if(pCa210)
    {
        return pCa210->caConnect(CA210_CHANNEL);
    }
    return false;
}

int DeltaEInterface::dteRun()
{
    return sRGB_DeltaEVerify();

    const int gammaEntries = 256;
    //BYTE commpressedGammaTbl[3][76] = {0};
    //double gammaTbl[3][gammaEntries] = {0};
    const int nativeDataFmtType = 0;
    const int gamutType = 0;
    const int gammaTrackType = 2;
    const int ctTrackType = 0;
   // const int compGmaType = 1;
    double Native_RGB_XYZxy[4*5*9] = {0};
    if(pMstGenGma)
    {
       // pMstGenGma->mGetPanelNativeData(Native_RGB_XYZxy);
        pMstGenGma->msetGammaEntries(gammaEntries);
        pMstGenGma->msetPanelNativeData(Native_RGB_XYZxy, nativeDataFmtType);
        pMstGenGma->msetGamutType(gamutType);
        pMstGenGma->msetGammaTrackType(gammaTrackType);
        pMstGenGma->msetColorTempTrackType(ctTrackType);
    //    pMstGenGma->mgenerateGamma();
   //     pMstGenGma->mgetCompressedGmaData(compGmaType, &commpressedGammaTbl[0][0]);
   //     pMstGenGma->mgetGammaData(&gammaTbl[0][0]);
        return 1;

    }
    return 0;
}

int DeltaEInterface::dteCheck()
{
    //quint8 buf[10] = "asdfaddf";
   // i2cdevice->write(i2cdevice->gethandle(), burnsettings->getSlaveaddr(), buf, 5);
    return 0;
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
    int pattern_index=0;
    float f100W_Raw_Y = 0;
    QString temp;
    //LPCA210DATASTRUCT pca210Data;
    XYZCOLOR color={0,0,0};
    if(!pCa210 || !pCa210->isConnect())
    {
       backupMsg.clear();
       backupMsg.sprintf("please connect Ca210...\n");
       return false;
    }
    sRGBResult = 0;
    sendPattern(pat_White);//SEND 100% White pattern
    delayMs(300);//延时300ms
    //pca210Data = pCa210->caMeasure();
    color = pCa210->caGetAverageMeasureXYZ(3);//获取CA210 的XYZ值
    f100W_Raw_Y = color.fX;

    temp.sprintf("get white pattern for Ca210 XYZ(%f, %f, %f) \n",
                      color.fX, color.fY, color.fZ);
    showMsg(temp);

    for(pattern_index = 0;pattern_index<32;pattern_index++)
    {
        sendPattern(g_PatternList[pattern_index]);//发送测试Pattern
        delayMs(300);//延时300ms
        color = pCa210->caGetAverageMeasureXYZ(3);//获取CA210 的XYZ值
       // color = testXYZ[pattern_index];
        sRGBResult += GetDeltaE_OnePatCIE94(pattern_index, color.fX, color.fY, color.fZ, f100W_Raw_Y);
        temp.clear();
        temp.sprintf("%d -(%lf, %lf, %lf) : %lf\n",
                     pattern_index+1, color.fX, color.fY, color.fZ, GetDeltaE_OnePatCIE94(pattern_index, color.fX, color.fY, color.fZ, f100W_Raw_Y));
        showMsg(temp);
    }
    if(sRGBResult > 0)
    {
        sRGBResult = sRGBResult/32;
    }
    temp.clear();
    temp.sprintf("result is %lf\n", sRGBResult);
    backupMsg.clear();
    backupMsg.append(temp);
    if(sRGBResult < 2.0)
    {
        return true;
    }
    return false;
}
