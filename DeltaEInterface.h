#ifndef DELTAEINTERFACE_H
#define DELTAEINTERFACE_H

#include "lib/mstgengmactr.h"
#include "dll/Ca210Ctrl_Export.h"
#include "lib/isp_iic.h"
#include "ddc/ddc_aps.h"
#include "ddc/burnsetting.h"
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
    int dteRun();
    int dteCheck();
    bool dteAdjust();
    QString getBackupMsg();
    //I2C
    bool connectI2C();
    void readI2CSetting();

    //deltaE
    void sendPattern(cRGB_t rgb);
    void delayMs(unsigned int msec);
    bool sRGB_DeltaEVerify();
signals:
    void sendPatSignal(cRGB_t rgb);
public slots:
private:
    Ca210DllCtr *pCa210;
    MstGenGmaCtr *pMstGenGma;
    //I2C
    Isp_I2C *i2cdevice;
    BurnSetting_T* burnsettings;
    QString backupMsg;
    //deltaE
    double sRGBResult;
};

#endif // DELTAEINTERFACE_H
