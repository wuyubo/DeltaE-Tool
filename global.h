#ifndef GLOBAL_DEFINE_H
#define GLOBAL_DEFINE_H

//global include
#include <QtCore>
#include <qstring.h>
#include <qfile.h>
#include <qglobal.h>
#include <QDataStream>
#include <QMessageBox>
/**********************************
*** mglobal define here
**********************************/
#define VERSION  "DeltaE V1.0 @CVTE"

#define CA210DLL "CA210Ctrl.dll"  // put dll file to folder where is exe

#define SRGB_PATTERN_PATN "sRGB_patternL"
#define SRGB_PATTERN_SUFFIX ".cvt"
#define GAMMA_TABLE_PATH  "./GammaTable.txt"
#define COLOR_MATRIX_PATH "./ColorMatrix.txt"
#define NATIVE_PATH   "./Native_PL"
#define NATIVE_SUFFIX   ".txt"
#define DELTAE_DATA_PATH  "./DeltaE_data.txt"
#define SETTING_PATH  "./setting.txt"

#define I2C_SETTING  "I2C"
#define CA210_SETTING  "CA210"
#define DELTAE_SETTING  "DeltaE"

#define EN_DEBUG 1
#if EN_DEBUG
#define DEBUGMSG(fmt, args...) qDebug(fmt, ##args)
#else
#define DEBUGMSG(fmt, args...)
#endif
/**********************************
*** global define end
**********************************/
/**********************************
*** global data type here
**********************************/
typedef  unsigned char BYTE;
/**********************************
*** global data type end
**********************************/
/**********************************
*** global data here
**********************************/
typedef struct
{
    quint8* data;
    quint8 size;
}burndata_t;

typedef struct
{
    int red;
    int green;
    int blue;
}cRGB_t;

typedef enum FUNCSTATUS
{
    FUNC_NONE,
    FUNC_CHECK,
    FUNC_ADJUST,
    FUNC_RUN,
    FUNC_END,
}FUNCSTATUS_t;
enum COLOR
{
    COLOR_R,
    COLOR_G,
    COLOR_B,
    COLOR_W,
    COLOR_END,
};

typedef enum LOGTEXTTYPE
{
    LOG_NONE,
    LOG_NORMAL,
    LOG_PASS,
    LOG_ERROR,
    LOG_WARNING,
    LOG_TEXTBOX,
    LOG_END,
}LOGTEXTTYPE_t;

typedef struct
{
    long checkChannel;
    int  checkSyncMode;
    int  checkSpeed;
    int  checkDelayms;
    long adjustChannel;
    int  adjustSyncMode;
    int  adjustSpeed;
    int  adjustDelayms;
}CA210Setting_t;

typedef struct
{
    QString series ;
    int  patternLevel ;
    int  nativeDataFmtType ;
    int  gammaEntris ;
    int  compressSize ;
    int  colorMetrixSize ;
    int  gamutType ;
    int  compGmaType ;
    int  gammaTrackType  ;
    BYTE colorTempTrackTypeSet;
    int  colorTempTrackType  ;
    int  colorTemperature  ;
    double  ctSx ;
    double  ctSy ;
    double  gammaPower  ;
    BYTE  darkModifySet  ;
    BYTE  darkModifyEnable;
    int   darkModifyMode;
    int   darkModifyLevel;
    BYTE  brightModifySet;
    BYTE  brightModifyEnable;
    int   brightModifyMode;
    int   brightModifyLevel;
    float maxBrightnessRatio;
    BYTE  targetGamutSet;
    double targetGamutRx;
    double targetGamutRy;
    double targetGamutGx;
    double targetGamutGy;
    double targetGamutBx;
    double targetGamutBy;
    double targetGamutWx;
    double targetGamutWy;
}DeltaESetting_t;

typedef enum ADJUSTTYPE
{
   ADJ_MEASURE,
   ADJ_FILE,
}AdjustType_t;

//**********************************
//global data end
//**********************************
//**********************************
//RGB data start
//**********************************
#define _MAX_PATTERN_COUT   200
#define DEF_COLOR  0
#define _MAX_COMP_GMA_COUT 3*64
#define _MAX_COLOR_MATRIX_COUT 18
#define PATTERN_LEVEL  9
#define _MAX_NATIVE_COUT (4*5*PATTERN_LEVEL)

//**********************************
//RGB data end
//**********************************
//**********************************
//CA210 data start
//**********************************
#define CA210_CHANNEL  0
#define DEF_DALEY_MS   300
#define ADJUST_DALEY_MS  300
#define CHECK_DALEY_MS   300
#define CIE94_RESULT    2.0
//**********************************
//CA210 data end
//**********************************
#endif // GLOBAL_DEFINE_H
