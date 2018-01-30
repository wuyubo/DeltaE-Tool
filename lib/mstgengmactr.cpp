#include "mstgengmactr.h"

namespace gengma {

MstGenGmaCtr::MstGenGmaCtr(QObject *parent) : QObject(parent)
{

}

int  MstGenGmaCtr::mGetGammaDllVer()
{
    return mstGetGammaDllVer();
}

void MstGenGmaCtr::mSetMeasPtnNum(int ptnNum)
{
    mstSetMeasPtnNum(ptnNum);
}

void MstGenGmaCtr::mGenGammaData(double *GmanIn, BYTE *GmaOut, float sRgbCM)
{
    mstGenGammaData(GmanIn, GmaOut, sRgbCM);
}

void MstGenGmaCtr::mExGenGammaData(double *GmaIn, BYTE *GmaOut, float GmaPwr)
{
    mstExGenGammaData(GmaIn, GmaOut, GmaPwr);
}

void MstGenGmaCtr::mExGenGammaData_CT(double *GmaIn, BYTE *GmaOut, float GmaPwr, int colorTemp)
{
    mstExGenGammaData_CT(GmaIn, GmaOut, GmaPwr, colorTemp);
}

void MstGenGmaCtr::mGenGammaData_1024E(double *GmaIn, BYTE*GmaOut, BYTE *sRgbCM, BYTE *sAdobeRgbCM)
{
    mstGenGammaData_1024E(GmaIn, GmaOut, sRgbCM, sAdobeRgbCM);
}

void MstGenGmaCtr::mSetCompressType(int compType)
{
    mstSetCompressType(compType);
}

void MstGenGmaCtr::mSetTargetGamut(int color, double x, double y)
{
    mstSetTargetGamut(color, x, y);
}

void MstGenGmaCtr::mGenUserGamutData(double *GmaIn, BYTE *GmaOut, BYTE *userGamutCM)
{
    mstGenUserGamutData(GmaIn, GmaOut, userGamutCM);
}

void MstGenGmaCtr::mGetWBTrackVal(int colorTemp, BYTE *gainR, BYTE *gainG, BYTE* gainB)
{
    mstGetWBTrackVal(colorTemp, gainR, gainG, gainB);
}

void MstGenGmaCtr::mSetDarkModifySettings(bool IsEnabled, int darkModifyMode, int darkLevel)
{
    mstSetDarkModifySettings(IsEnabled, darkModifyMode, darkLevel);
}
void MstGenGmaCtr::mGetUnCompressedGammaData(double *GmaOut)
{
    mstGetUnCompressedGammaData(GmaOut);
}

void MstGenGmaCtr::mGenGamutData(double *GmaIn, int gammaEntries, int gamutType, BYTE *GmaOut, BYTE *CM)
{
    mstGenGamutData(GmaIn, gammaEntries, gamutType, GmaOut, CM);
}

void MstGenGmaCtr::msetGammaEntries(int gmaEntries)
{
    setGammaEntries(gmaEntries);
}

int MstGenGmaCtr::mgetGammaEntries()
{
    return getGammaEntries();
}

void MstGenGmaCtr::msetPanelNativeData(double * panelNativeData, int formatType)
{
    setPanelNativeData(panelNativeData, formatType);
}

void MstGenGmaCtr::mGetPanelNativeData(double * panelNativeData)
{
    //GetPanelNativeData(panelNativeData);
}

void MstGenGmaCtr::msetGamutType(int gamutType)
{
    setGamutType(gamutType);
}

void MstGenGmaCtr::msetUserGamutVertice(int RGBW, double sx, double sy)
{
    setUserGamutVertice(RGBW, sx, sy);
}

void MstGenGmaCtr::mgetGamutvertice(int gamutType, int RGBW, double *sx, double *sy)
{
    getGamutVertice(gamutType, RGBW, sx, sy);
}

void MstGenGmaCtr::msetGammaTrackType(int gmaTrackType)
{
    setGammaTrackType(gmaTrackType);
}

void MstGenGmaCtr::msetGammaPower(double gmaPwr)
{
    setGammaPower(gmaPwr);
}

void MstGenGmaCtr::msetColorTempTrackType(int ctTrackType)
{
    setColorTempTrackType(ctTrackType);
}

void MstGenGmaCtr::msetColorTemperature(int colorTemp)
{
    setColorTemperature(colorTemp);
}

void MstGenGmaCtr::msetDarkModifySettings(int darkModifyMode, int darkLevel)
{
    setDarkModifySettings(darkModifyMode, darkLevel);
}

void MstGenGmaCtr::msetBrightModifySettings(int brightModifyMode, int rightLevel)
{
    setBrightModifySettings(brightModifyMode, rightLevel);
}

void MstGenGmaCtr::mgenerateGamma()
{
    generateGamma();
}

void MstGenGmaCtr::mgetGammaData(double *gmaOut)
{
    getGammaData(gmaOut);
}

void MstGenGmaCtr::mgetCompressedGmaData(int compType, BYTE* gmaOut)
{
    getCompressedGmaData(compType, gmaOut);
}

void MstGenGmaCtr::mgetColorMatrixData(BYTE* cn)
{
    getColorMatrixData(cn);
}

void MstGenGmaCtr::mgetWBTrackValues(int colorTemp, BYTE *gainR, BYTE gainG, BYTE *gainB)
{
    getWBTrackValues(colorTemp, gainR, gainG, gainB);
}

void MstGenGmaCtr::mgetWBTrackValuesXY(int colorTemp, BYTE *gainR, BYTE gainG, BYTE *gainB)
{
    getWBTrackValuesXY(colorTemp, gainR, gainG, gainB);
}

void MstGenGmaCtr::msetMaxBrightnessRatio(float ratio)
{
    setMaxBrightnessRatio(ratio);
}
} //name space gengma
