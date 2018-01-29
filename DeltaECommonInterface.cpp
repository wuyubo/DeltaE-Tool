#include "DeltaECcommonInterface.h"

DeltaECommonInterface::DeltaECommonInterface(QObject *parent) : QObject(parent)
{

}
int DeltaECommonInterface::dteConnect()
{ 
    return mstGetGammaDllVer();
}

int DeltaECommonInterface::dteRun()
{
    setColorTemperature(10);
    return 0;
}

int DeltaECommonInterface::dteCheck()
{
    return 0;
}

int DeltaECommonInterface::dteAdjust()
{
    return 0;
}
