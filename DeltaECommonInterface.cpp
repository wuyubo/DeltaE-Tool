#include "DeltaECcommonInterface.h"

DeltaECommonInterface::DeltaECommonInterface(QObject *parent) : QObject(parent)
{

}
int DeltaECommonInterface::DtEConnect()
{ 
    return mstGetGammaDllVer();
}

int DeltaECommonInterface::DtERun()
{
    setColorTemperature(10);
    return 0;
}

int DeltaECommonInterface::DtECheck()
{
    return 0;
}

int DeltaECommonInterface::DtEAdjust()
{
    return 0;
}
