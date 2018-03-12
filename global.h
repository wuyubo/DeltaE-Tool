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

#define SRGB_PATTERN_PATN "sRGB_patternL33.cvt"
#define SRGB_PATTERN_SUFFIX ".cvt"
#define GAMMA_TABLE_PATH  "./GammaTable.txt"
#define COLOR_MATRIX_PATH "./ColorMatrix.txt"
#define NATIVE_PATH   "./Native_PL"
#define NATIVE_SUFFIX   ".txt"
#define DELTAE_DATA_PATH  "./DeltaE_data.txt"

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

typedef enum LOGTEXTTYPE
{
    LOG_NORMAL,
    LOG_PASS,
    LOG_ERROR,
    LOG_WARNING,
    LOG_TEXTBOX,
    LOG_END,
}LOGTEXTTYPE_t;


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
#define ADJUST_DALEY_MS  10
#define CHECK_DALEY_MS   300
#define DET94_RESULT    2.0
//**********************************
//CA210 data end
//**********************************
#endif // GLOBAL_DEFINE_H
