#ifndef DDC_APS_H
#define DDC_APS_H

#include "../global.h"

namespace ddc {

#define     COUNTOF(array)       (sizeof(array)/sizeof(array[0]))

using cbverify = bool(*)(quint8 *feedback,quint8 fdlen,quint8 *data,quint8 len);
using cbassemble = burndata_t (*)(quint8 *head,quint8 headsize,quint8 *body,quint8 bodysize);
using setpara = int (*)(QString& text,quint8 *head,quint8& headsize);
#define PAT_R 2
#define PAT_G 3
#define PAT_B 4
#define POSTGMA_MODE     2
#define POSTGMA_CHANEL   3
#define POSTGMA_SIZE     4
#define POSTGMA_OFFSET_H 5
#define POSTGMA_OFFSET_L 6
#define POSTGMA_SETMODE(mode)  (writeDeltaEPostGammacmd.burndata[POSTGMA_MODE] = mode)
#define POSTGMA_SETCHANEL(chn) (writeDeltaEPostGammacmd.burndata[POSTGMA_CHANEL] = chn)
#define POSTGMA_SETSIZE(size)  (writeDeltaEPostGammacmd.burndata[POSTGMA_SIZE] = size)
#define POSTGMA_SETOFFSETH(offset) (writeDeltaEPostGammacmd.burndata[POSTGMA_OFFSET_H] = (offset&0xF0)>>8)
#define POSTGMA_SETOFFSETL(offset) (writeDeltaEPostGammacmd.burndata[POSTGMA_OFFSET_L] = (offset))
#define POSTGMA_SETOFFSET(offset)  POSTGMA_SETOFFSETH(offset);POSTGMA_SETOFFSETL(offset)
#define COLORMATRIX_SETMODE(mode)  (writeDeltaEColorMatrixcmd.burndata[2] = mode)
#define COLORMATRIX_SETSIZE(size)  (writeDeltaEColorMatrixcmd.burndata[3] = size)
typedef struct burncmd{
    //ui relate
    QString name;
    QString description;
    setpara setparafunc;

    //data to send and assemble function
    quint8 *burndata;
    quint8 datalen;
    cbassemble assemblefunc;

    //get feedback and verify
    quint8 feedbacklen;
    cbverify verifyfunc;

    //flow control parameter
    quint8 retrycnt;
    quint32 delay;
    quint32 lastpackdelay;
}burnCmd_t;
extern burnCmd_t enterATcmd;
extern burnCmd_t enterDeltaEDebugcmd;
extern burnCmd_t enterDeltaEAutoGammacmd;
extern burnCmd_t writeDeltaERGBPaternCmd;
extern burnCmd_t exitDeltaEDebugcmd;
extern burnCmd_t exitDeltaEAutoGammacmd;
extern burnCmd_t writeDeltaEPostGammacmd;
extern burnCmd_t writeDeltaEColorMatrixcmd;
extern burnCmd_t readDeltaEACKcmd;
extern burnCmd_t saveDeltaEGmacmd;
}

#endif // DDC_APS_H
