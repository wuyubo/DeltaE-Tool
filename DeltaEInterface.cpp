#include "DeltaEInterface.h"

DeltaEInterface::DeltaEInterface(QObject *parent) : QObject(parent)
{
    pCa210 = new Ca210DllCtr(CA210DLL);
    pMstGenGma = new MstGenGmaCtr();
    if(!pCa210 || !pMstGenGma)
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
}

bool DeltaEInterface::dteConnect()
{
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
        pMstGenGma->mgenerateGamma();
        pMstGenGma->mgetCompressedGmaData(compGmaType, &commpressedGammaTbl[0][0]);
        pMstGenGma->mgetGammaData(&gammaTbl[0][0]);
        return 1;

    }
    return 0;
}

int DeltaEInterface::dteCheck()
{
    return 0;
}

int DeltaEInterface::dteAdjust()
{
    return 0;
}
