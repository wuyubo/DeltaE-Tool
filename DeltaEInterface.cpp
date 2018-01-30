#include "DeltaEInterface.h"

DeltaEInterface::DeltaEInterface(QObject *parent) : QObject(parent)
{
    pCa210 = new Ca210DllCtr(CA210DLL);
}

DeltaEInterface::~DeltaEInterface()
{
    if(pCa210)
    {
        delete pCa210;
    }
}

bool DeltaEInterface::dteConnect()
{
    if(pCa210)
    {
        return pCa210->caConnect(0);
    }
    return false;
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
