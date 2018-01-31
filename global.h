#ifndef GLOBAL_DEFINE_H
#define GLOBAL_DEFINE_H

//global include
#include <QtCore>
#include <qstring.h>
#include <qfile.h>
#include <qglobal.h>
#include <QDataStream>
/**********************************
*** mglobal define here
**********************************/
#define VERSION  "DeltaE V1.0 @CVTE"

#define CA210DLL "CA210Ctrl.dll"  // put dll file to folder where is exe

#define EN_DEBUG 1
#if EN_DEBUG
#define DEBUGMSG(fmt, args...) qDebug(fmt, ##args)
#else
#define DEBUGMSG()
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
//**********************************
//global data end
//**********************************
#endif // GLOBAL_DEFINE_H
