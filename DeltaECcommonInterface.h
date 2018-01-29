#ifndef DELTAECOMMONINTERFACE_H
#define DELTAECOMMONINTERFACE_H

#include "lib/mstGenGma.h"
#include <QObject>

class DeltaECommonInterface : public QObject
{
    Q_OBJECT
public:
    explicit DeltaECommonInterface(QObject *parent = 0);
    int DtEConnect();
    int DtERun();
    int DtECheck();
    int DtEAdjust();
signals:

public slots:
};

#endif // DELTAECOMMONINTERFACE_H
