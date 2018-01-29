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
/**********************************
*** global define end
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
