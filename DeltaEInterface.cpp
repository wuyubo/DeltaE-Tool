#include "DeltaEInterface.h"

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
        if(i2cdevice->gethandle() != NULL)
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
        return pCa210->caConnect(0);
    }
    return false;
}

int DeltaEInterface::dteRun()
{
    const int gammaEntries = 256;
    BYTE commpressedGammaTbl[3][76] = {0};
    double gammaTbl[3][gammaEntries] = {0};
    const int nativeDataFmtType = 0;
    const int gamutType = 0;
    const int gammaTrackType = 2;
    const int ctTrackType = 0;
    const int compGmaType = 1;
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
    quint8 buf[10] = "asdfaddf";
    i2cdevice->write(i2cdevice->gethandle(), burnsettings->getSlaveaddr(), buf, 5);
    return 0;
}

int DeltaEInterface::dteAdjust()
{
    return 0;
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
