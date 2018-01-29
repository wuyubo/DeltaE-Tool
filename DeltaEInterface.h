#ifndef DELTAEINTERFACE_H
#define DELTAEINTERFACE_H

#include "lib/mstGenGma.h"
#include <QObject>

class DeltaEInterface : public QObject
{
    Q_OBJECT
public:
    explicit DeltaEInterface(QObject *parent = 0);
    int dteConnect();
    int dteRun();
    int dteCheck();
    int dteAdjust();
signals:

public slots:
};

#endif // DELTAEINTERFACE_H
