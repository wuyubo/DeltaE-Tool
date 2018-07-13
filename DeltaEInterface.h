#ifndef DELTAEINTERFACE_H
#define DELTAEINTERFACE_H

#include "lib/mstgengmactr.h"
#include "dll/Ca210Ctrl_Export.h"
#include "lib/isp_iic.h"
#include "ddc/ddc_aps.h"
#include "ddc/burnsetting.h"
#include "ddc/transfer.h"
#include "deltaE/DeltaE.h"
#include "data/data.h"
#include "global.h"
#include <QObject>

using namespace ca210;
using namespace gengma;
using namespace ddc;
class DeltaEInterface : public QObject
{
    Q_OBJECT
public:
    explicit DeltaEInterface(QObject *parent = 0);
    ~DeltaEInterface();
    bool dteConnect();
    bool connectCa210();
    void dteDisConnect();
    void setConnectFlag(bool bcnt);
    void setstandardResult(float value);
    int dteRun();
    int dteCheck();
    bool dteAdjust();
    void dteTest(int r, int g, int b);
    QString getBackupMsg();
    void showMsg(QString msg, LOGTEXTTYPE_t logType = LOG_NORMAL);
    bool isStatus(FUNCSTATUS_t status);
    void setStatus(FUNCSTATUS_t status);
    void setAdjType(AdjustType_t adjtype);
    bool isAdjType(AdjustType_t adjtype);
    //I2C
    bool connectI2C();
    bool IsConnectI2C();
    BurnSetting_T * readI2CSetting();
    //ddc
    bool cmdSend(QString CmdStr);
    bool cmdSend(burnCmd_t *cmd);
    bool cmdSend(burnCmd_t *cmd,quint8 *data, quint32 size,quint8 source = 1);
    bool sRGB_DeltaERun();
    bool sRGB_DeltaERunstep0();
    bool sRGB_DeltaERunstep1();
    bool sRGB_DeltaERunstep2();
    bool sRGB_DeltaERunstep3();
    bool sRGB_DeltaERunstep4();
    //ca210
    bool IsConnectCA210();
    void sendPattern(cRGB_t rgb);
    void delayMs(unsigned int msec);
    bool sRGB_DeltaEVerify();
    bool sRGB_DeltaEVerifyStep0();
    bool sRGB_DeltaEVerifyStep1();
    bool sRGB_DeltaEVerifyStep2();
    bool sRGB_DeltaEVerifyStep3();
    //DeltaE lib
    bool sRGB_DeltaEAdjust();
    bool sRGB_DeltaEAdjustStep0(); //generate Gamma
    bool sRGB_DeltaEAdjustStep1(); //generate Gamma
    bool sRGB_DeltaEAdjustStep2(); //send Gamma to monitor
signals:
    void sendPatSignal(cRGB_t rgb);
    void updateMsgSignal(LOGTEXTTYPE_t logType);
public slots:
    void recvMsg(QString msg);
private:
    QString backupMsg;
    Ca210DllCtr *pCa210;
    MstGenGmaCtr *pMstGenGma;
    FUNCSTATUS_t func_status;
    int m_delayTimeMs;
    bool m_isConnect;
    AdjustType_t m_adjType;
    //I2C
    Isp_I2C *i2cdevice;
    BurnSetting_T *m_pBurnsettings;
    DDCProtocol_T *m_pDDCprotocol;
    bool m_bI2cConnect;

    //Transfer layer
    Transfer_T *m_transfer=nullptr;
    //ca210
    double sRGBResult;
    double m_d100W_Raw_Y;
    bool m_bCa210Connect;

    //DeltaE lib
    //data
    Data *m_pdata;
    float m_standard;
};

#endif // DELTAEINTERFACE_H
