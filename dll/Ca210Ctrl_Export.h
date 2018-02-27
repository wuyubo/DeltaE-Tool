#ifndef _CA210CTRL_H_
#define _CA210CTRL_H_

#include "../global.h"
#include <QLibrary>

namespace ca210{

enum SYNC_MODE_TYPE
{
    SYNC_MODE_NTSC = 0,
    SYNC_MODE_PAL,
    SYNC_MODE_EXT,
    SYNC_MODE_UNIV,
    SYNC_MODE_INT
};

enum SPEED_TYPE
{
    SPEED_SLOW = 0,
    SPEED_FAST,
    SPEED_AUTO
};

//this structure include all data user can get from ca210
typedef struct tagCA210DATASTRUCT
{
    float   fX;
    float   fY;
    float   fZ;
    float   fFlckrJEITA;
    long    lNumber;
    float   fLv;
    float   fUd;
    float   fVd;
    long    lT;
    float   fDEUser;
    float   fFlckrFMA;
    float   fSy;
    float   fSx;
    float   fDuv;
    float   fUsUser;
    float   fVsUser;
    float   fLsUser;
    float   fLvfL;
    float   fR;
    float   fG;
    float   fB;
    long    lRd;
    long    lRad;
    long    lRfma;
    long    lRjeita;
}CA210DATASTRUCT, *LPCA210DATASTRUCT;

typedef struct tagXYZCOLOR{
    float   fX;
    float   fY;
    float   fZ;
}XYZCOLOR;

//define function point
typedef bool (*func_caConnect)(long lChannelNO);
typedef bool (*func_caSetChannel)(long lChannelNO);
typedef void (*func_caDisConnect)();
typedef CA210DATASTRUCT (*func_caMeasure)();
typedef void (*func_caZeroCal)();
typedef XYZCOLOR (*func_caGetAverageMeasureXYZ)(int nTimes);
typedef void (*func_caSetSyncMode)(int nSyncMode);
typedef void (*func_caSetSpeed)(int nSpeed);
typedef int  (*func_caGetCATypeName)(BYTE *bCAType);
typedef int  (*func_caGetCAVersionName)(BYTE *bCAVersion);

class Ca210DllCtr: public QLibrary
{
public:
    explicit Ca210DllCtr(const QString& fileName);
    ~Ca210DllCtr();
public:
    bool caConnect(long lChannelNO);
    bool caSetChannel(long lChannelNO);
    void caDisConnect();
    LPCA210DATASTRUCT caMeasure();
    void caZeroCal();
    XYZCOLOR caGetAverageMeasureXYZ(int nTimes);
    void caSetSyncMode(int nSyncMode);
    void caSetSpeed(int nSpeed);
    int  caGetCATypeName(BYTE *bCAType);
    int  caGetCAVersionName(BYTE *bCAVersion);
    bool isConnect()const;
private:
    CA210DATASTRUCT c210_data;
    func_caConnect pfcaConnect;
    func_caSetChannel pfcaSetChannel;
    func_caDisConnect pfcaDisConnect;
    func_caMeasure pfcaMeasure;
    func_caZeroCal pfcaZeroCal;
    func_caGetAverageMeasureXYZ pfcaGetAverageMeasureXYZ;
    func_caSetSyncMode pfcaSetSyncMode;
    func_caSetSpeed pfcaSetSpeed;
    func_caGetCATypeName pfcaGetCATypeName;
    func_caGetCAVersionName pfcaGetCAVersionName;
    bool bIsConnect;
};
}
#endif //#ifndef _CA210CTRL_H_
