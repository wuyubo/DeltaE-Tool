#include "DeltaEInterface.h"

DeltaEInterface::DeltaEInterface(QObject *parent) : QObject(parent)
{

}
int DeltaEInterface::dteConnect()
{ 
    return mstGetGammaDllVer();
}

int DeltaEInterface::dteRun()
{
    setColorTemperature(10);
    return 0;
}

int DeltaEInterface::dteCheck()
{
    return 0;
}

int DeltaEInterface::dteAdjust()
{
    return 0;
}
