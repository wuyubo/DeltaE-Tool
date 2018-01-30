#ifndef MSTGENGMACTR_H
#define MSTGENGMACTR_H

#include "../global.h"
#include "mstGenGma.h"
#include <QObject>
namespace gengma {

class MstGenGmaCtr : public QObject
{
    Q_OBJECT
public:
    explicit MstGenGmaCtr(QObject *parent = 0);

signals:

public slots:
public:
    int  mGetGammaDllVer(); //get version of library
    void mSetMeasPtnNum(int ptnNum); //set pattern number. 9/12/17/33/65/129/256
    void mGenGammaData(double *GmanIn, BYTE *GmaOut, float sRgbCM); //GET DELTA-E gamma table and color sRGB
    void mExGenGammaData(double *GmaIn, BYTE *GmaOut, float GmaPwr);//Get gamma table by specified gamma power and color temperature
    void mExGenGammaData_CT(double *GmaIn, BYTE *GmaOut, float GmaPwr, int colorTemp);
    void mGenGammaData_1024E(double *GmaIn, BYTE*GmaOut, BYTE *sRgbCM, BYTE *sAdobeRgbCM);
    void mSetCompressType(int compType);
    void mSetTargetGamut(int color, double x, double y);
    void mGenUserGamutData(double *GmaIn, BYTE *GmaOut, BYTE *userGamutCM);
    void mGetWBTrackVal(int colorTemp, BYTE *gainR, BYTE *gainG, BYTE* gainB);
    void mSetDarkModifySettings(bool IsEnabled, int darkModifyMode, int darkLevel);
    void mGetUnCompressedGammaData(double *GmaOut); //after call any gamma correction function, Get uncommpressed gamma data
    void mGenGamutData(double *GmaIn, int gammaEntries, int gamutType, BYTE *GmaOut, BYTE *CM);
    void msetGammaEntries(int gmaEntries); //set nmber of gamma entries
    int  mgetGammaEntries();
    void msetPanelNativeData(double * panelNativeData, int formatType);
    void mGetPanelNativeData(double * panelNativeData);
    void msetGamutType(int gamutType);
    void msetUserGamutVertice(int RGBW, double sx, double sy);
    void mgetGamutvertice(int gamutType, int RGBW, double *sx, double *sy);
    void msetGammaTrackType(int gmaTrackType);
    void msetGammaPower(double gmaPwr);
    void msetColorTempTrackType(int ctTrackType);
    void msetColorTemperature(int colorTemp);
    void msetDarkModifySettings(int darkModifyMode, int darkLevel);
    void msetBrightModifySettings(int brightModifyMode, int rightLevel);
    void mgenerateGamma();
    void mgetGammaData(double * gmaOut);
    void mgetCompressedGmaData(int compType, BYTE * gmaOut);
    void mgetColorMatrixData(BYTE * cn);
    void mgetWBTrackValues(int colorTemp, BYTE *gainR, BYTE gainG, BYTE *gainB);
    void mgetWBTrackValuesXY(int colorTemp, BYTE *gainR, BYTE gainG, BYTE *gainB);
    void msetMaxBrightnessRatio(float ratio);
};
}
#endif // MSTGENGMACTR_H
