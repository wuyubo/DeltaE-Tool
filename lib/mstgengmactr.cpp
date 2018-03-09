#include "mstgengmactr.h"

namespace gengma {

double default_Native_RGB_XYZxy[4*5*9] = {
    //RX
     0.107084296643734,
     0.345137089490891,
     1.49937164783478,
     3.6866021156311,
     7.04250001907349,
     11.6406202316284,
     17.6222400665283,
     24.7057495117188,
     33.4616317749023,
     //RY
     0.1120890006423,
     0.243019700050354,
     0.881242513656616,
     2.08757400512695,
     3.93095850944519,
     6.46041202545166,
     9.75764751434326,
     13.6366443634033,
     18.4037284851074,
    //RZ
     0.195870697498322,
     0.214340701699257,
     0.312890201807022,
     0.493769288063049,
     0.748154520988464,
     1.08822524547577,
     1.42198657989502,
     1.70512223243713,
     1.81727421283722,
    //R_x
     0.258007109165192,
     0.430078715085983,
     0.556662023067474,
     0.588167548179626,
     0.600813210010529,
     0.60662168264389,
     0.611843526363373,
     0.616910934448242,
     0.623323202133179,
    //R_y
     0.27006533741951,
     0.30282923579216,
     0.327173233032227,
     0.333055555820465,
     0.335359841585159,
     0.336668163537979,
     0.338785141706467,
     0.340511620044708,
     0.34282460808754,
    //GX
     0.107084296643734,
     0.315345406532288,
     1.36632812023163,
     3.37892985343933,
     6.5303316116333,
     10.8149785995483,
     16.4813404083252,
     24.0750026702881,
     33.8349723815918,
    //GY
     0.1120890006423,
     0.529766321182251,
     2.63785624504089,
     6.69684982299805,
     12.8432950973511,
     21.2975044250488,
     32.3627243041992,
     46.8918876647949,
     64.3945999145508,
    //GZ
     0.195870697498322,
     0.245300605893135,
     0.50388491153717,
     0.987670600414276,
     1.7710794210434,
     2.77366948127747,
     4.0166802406311,
     5.51784133911133,
     6.87772369384766,
    //G_x
     0.258007109165192,
     0.289198309183121,
     0.303084999322891,
     0.305413752794266,
     0.308840036392212,
     0.310007780790329,
     0.311787903308868,
     0.314768731594086,
     0.321908891201019,
     //G_y
     0.27006533741951,
     0.485840350389481,
     0.585141003131866,
     0.605312943458557,
     0.607400000095367,
     0.610485911369324,
     0.612226009368896,
     0.613088190555573,
     0.612655878067017,
     //BX
     0.107084296643734,
     0.1961620002985,
     0.720336079597473,
     1.74482154846191,
     3.32229566574097,
     5.41422557830811,
     8.15475749969482,
     11.5544490814209,
     16.3915195465088,
    //BY
     0.1120890006423,
     0.149847403168678,
     0.377540111541748,
     0.800832688808441,
     1.49423980712891,
     2.41188263893127,
     3.64533162117004,
     5.26720571517944,
     8.68611621856689,
    //BZ
     0.195870697498322,
     0.662986516952515,
     3.42801117897034,
     8.84207630157471,
     17.0297832489014,
     28.001953125,
     42.2718811035156,
     59.8446006774902,
     82.2953338623047,
    // B_x
     0.258007109165192,
     0.194413080811501,
     0.15915909409523,
     0.153219431638718,
     0.15207576751709,
     0.151116892695427,
     0.150813028216362,
     0.150711014866829,
     0.152659639716148,
    //B_y
     0.27006533741951,
     0.148511409759521,
     0.0834179222583771,
     0.0703241676092148,
     0.0683977827429771,
     0.0673182532191277,
     0.067416287958622,
     0.0687030553817749,
     0.0808966755867004,
    //WX
     0.107084296643734,
     0.652173101902008,
     3.4422402381897,
     8.85540390014648,
     17.0583400726318,
     28.2264614105225,
     42.8356590270996,
     60.9521636962891,
     83.5444259643555,
    // WY
     0.1120890006423,
     0.709529399871826,
     3.74653053283691,
     9.57651710510254,
     18.4557781219482,
     30.5781631469727,
     46.4085311889648,
     66.467887878418,
     91.3735733032227,
    //WZ
     0.195870697498322,
     0.742805480957031,
     3.92300033569336,
     10.1739349365234,
     19.5670108795166,
     32.064094543457,
     48.1594047546387,
     67.6458282470703,
     90.6668167114258,
    //W_x
     0.258007109165192,
     0.309893369674683,
     0.309783220291138,
     0.309566110372543,
     0.309694826602936,
     0.3106290102005,
     0.311750650405884,
     0.312469631433487,
     0.314567774534225,
     //W_y
     0.27006533741951,
     0.337147384881973,
     0.337167710065842,
     0.334774702787399,
     0.335065364837646,
     0.336509227752686,
     0.337753385305405,
     0.340745836496353,
     0.34404668211937,

};

MstGenGmaCtr::MstGenGmaCtr(QObject *parent) : QObject(parent)
{

}

int  MstGenGmaCtr::mmstGetGammaDllVer()
{
    return mstGetGammaDllVer();
}

void MstGenGmaCtr::mmstSetMeasPtnNum(int ptnNum)
{
    mstSetMeasPtnNum(ptnNum);
}

void MstGenGmaCtr::mmstGenGammaData(double *GmanIn, BYTE *GmaOut, BYTE* sRgbCM)
{
    mstGenGammaData(GmanIn, GmaOut, sRgbCM);
}

void MstGenGmaCtr::mmstExGenGammaData(double *GmaIn, BYTE *GmaOut, float GmaPwr)
{
    mstExGenGammaData(GmaIn, GmaOut, GmaPwr);
}

void MstGenGmaCtr::mmstExGenGammaData_CT(double *GmaIn, BYTE *GmaOut, float GmaPwr, int colorTemp)
{
    mstExGenGammaData_CT(GmaIn, GmaOut, GmaPwr, colorTemp);
}

void MstGenGmaCtr::mmstGenGammaData_1024E(double *GmaIn, BYTE*GmaOut, BYTE *sRgbCM, BYTE *sAdobeRgbCM)
{
    mstGenGammaData_1024E(GmaIn, GmaOut, sRgbCM, sAdobeRgbCM);
}

void MstGenGmaCtr::mmstSetCompressType(int compType)
{
    mstSetCompressType(compType);
}

void MstGenGmaCtr::mmstSetTargetGamut(int color, double x, double y)
{
    mstSetTargetGamut(color, x, y);
}

void MstGenGmaCtr::mmstGenUserGamutData(double *GmaIn, BYTE *GmaOut, BYTE *userGamutCM)
{
    mstGenUserGamutData(GmaIn, GmaOut, userGamutCM);
}

void MstGenGmaCtr::mmstGetWBTrackVal(int colorTemp, BYTE *gainR, BYTE *gainG, BYTE* gainB)
{
    mstGetWBTrackVal(colorTemp, gainR, gainG, gainB);
}

void MstGenGmaCtr::mmstSetDarkModifySettings(bool IsEnabled, int darkModifyMode, int darkLevel)
{
    mstSetDarkModifySettings(IsEnabled, darkModifyMode, darkLevel);
}
void MstGenGmaCtr::mmstGetUnCompressedGammaData(double *GmaOut)
{
    mstGetUnCompressedGammaData(GmaOut);
}

void MstGenGmaCtr::mmstGenGamutData(double *GmaIn, int gammaEntries, int gamutType, BYTE *GmaOut, BYTE *CM)
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
void MstGenGmaCtr::msetColorTemperatureXY(double sx, double sy)
{
    setColorTemperatureXY(sx, sy);
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
