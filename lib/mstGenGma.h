#ifndef _MSTGENGMA_H_
#define _MSTGENGMA_H_
#ifdef __cplusplus
extern "C" {
#endif
typedef  unsigned char BYTE;

int mstGetGammaDllVer(); //get version of library
void mstSetMeasPtnNum(int ptnNum); //set pattern number. 9/12/17/33/65/129/256
void mstGenGammaData(double *GmanIn, BYTE *GmaOut, float sRgbCM); //GET DELTA-E gamma table and color sRGB
void mstExGenGammaData(double *GmaIn, BYTE *GmaOut, float GmaPwr);//Get gamma table by specified gamma power and color temperature
// Get delta-E gamma table by specified gamma power adn color temperature
void mstGenGammaData_CT(double *GmaIn, BYTE *GmaOut, float GmaPwr, int colorTemp);
// Get delta-E gamma table, sRGB and Adobe RGB color matrix for 1024 engine of gamma resolution
void mstGenGammaData_1024E(double *GmaIn, BYTE*GmaOut, BYTE *sRgbCM, BYTE *sAdobeRgbCM);
//set data format type for compressing gamma table.
//For TSUM series:
//0: 64 bytes commpres format
//1: 76 bytes commpress format
//for MST9U series:
//0: 320 bytes compress format A(old)
//1: 320 bytes compress format B
//(new, recommend setting)
void mstSetCommpressType(int compType);
void mstSetTargetGamut(int color, double x, double y);
void mstGenUserGamutData(double *GmaIn, BYTE *GmaOut, BYTE *userGamutCM);
 //after call any gamma correction function, Get R/G/B gain for targeting color temperature
void mstGetWBTrackVal(int colorTemp, BYTE *gainR, BYTE *gainG, BYTE* gainB);
void mstSetDarkModifySettings(bool IsEnabled, int darkModifyMode, int darkLevel);
void mstGetUnCompressedGammaData(double *GmaOut); //after call any gamma correction function, Get uncommpressed gamma data
void mstGenGamutData(double *GmaIn, int gammaEntries, int gamutType, BYTE *GmaOut, BYTE *CM);

void setGammaEntries(int gmaEntries); //set nmber of gamma entries
int getGammaEntries();
void setPanelNativeData(double * panelNativeData, int formatType);
void setGamutType(int gamutType);
void setUserGamutVertice(int RGBW, double sx, double sy);
void getGamnuvertice(int gamutType, int RGBW, double *sx, double *sy);
void setGammaTrackType(int gmaTrackType);
void setGammaPower(double gmaPwr);
void setColorTempTrackType(int ctTrackType);

void setColorTemperature(int colorTemp);
void setDarkModifySettings(int darkModifyMode, int darkLevel);
void setBrightModifySetting(int brightModifyMode, int rightLevel);

void generateGamma();
void getGammaData(double *gmaOut);
void getCompressedGmaData(int compType, BYTE* gmaOut);
void getColorMatrixData(BYTE* cn);
void getWBTrackValues(int colorTemp, BYTE *gainR, BYTE gainG, BYTE *gainB);
void getWBTrackValuesXY(int colorTemp, BYTE *gainR, BYTE gainG, BYTE *gainB);
void setMaxBrightnessRatio(float ratio);

#ifdef __cplusplus
}//extern "C" {
#endif

#endif //_MSTGENGMA_H_
