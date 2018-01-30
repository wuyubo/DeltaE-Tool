#ifndef DELTAEINTERFACE_H
#define DELTAEINTERFACE_H

#include "lib/mstgengmactr.h"
#include "dll/Ca210Ctrl_Export.h"
#include <QObject>
using namespace ca210;
using namespace gengma;
class DeltaEInterface : public QObject
{
    Q_OBJECT
public:
    explicit DeltaEInterface(QObject *parent = 0);
    ~DeltaEInterface();
    bool dteConnect();
    int dteRun();
    int dteCheck();
    int dteAdjust();
signals:

public slots:
private:
    Ca210DllCtr *pCa210;
    MstGenGmaCtr *pMstGenGma;
};

#endif // DELTAEINTERFACE_H
