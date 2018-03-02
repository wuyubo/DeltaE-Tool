#ifndef DELTAEINTERFACE_H
#define DELTAEINTERFACE_H

#include "lib/mstgengmactr.h"
#include "dll/Ca210Ctrl_Export.h"
#include "lib/isp_iic.h"
#include "ddc/ddc_aps.h"
#include "ddc/burnsetting.h"
#include "deltaE/DeltaE.h"
#include "data/data.h"
#include <QObject>
#include "global.h"
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
    int dteRun();
    int dteCheck();
    bool dteAdjust();
    QString getBackupMsg();
    void showMsg(QString msg);
    //I2C
    bool connectI2C();
    void readI2CSetting();

    //ca210
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
    void updateMsgSignal();
public slots:
private:
    Ca210DllCtr *pCa210;
    MstGenGmaCtr *pMstGenGma;
    //I2C
    Isp_I2C *i2cdevice;
    BurnSetting_T* burnsettings;
    QString backupMsg;
    //ca210
    double sRGBResult;
    double m_d100W_Raw_Y;

    //DeltaE lib
    //data
    Data *m_pdata;
};

#endif // DELTAEINTERFACE_H
