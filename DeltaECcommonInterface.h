#ifndef DELTAECOMMONINTERFACE_H
#define DELTAECOMMONINTERFACE_H

#include "lib/mstGenGma.h"
#include <QObject>

class DeltaECommonInterface : public QObject
{
    Q_OBJECT
public:
    explicit DeltaECommonInterface(QObject *parent = 0);
    int dteConnect();
    int dteRun();
    int dteCheck();
    int dteAdjust();
signals:

public slots:
};

#endif // DELTAECOMMONINTERFACE_H
