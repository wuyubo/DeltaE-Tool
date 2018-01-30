#include "Ca210Ctrl_Export.h"

namespace ca210{

Ca210DllCtr::Ca210DllCtr(const QString& fileName):QLibrary(fileName)
{
    if(this->load())
    {
        pfcaConnect = (func_caConnect)this->resolve("ca210Connect");
        pfcaSetChannel = (func_caSetChannel)this->resolve("ca210SetChannel");
        pfcaDisConnect = (func_caDisConnect)this->resolve("ca210DisConnect");
        pfcaMeasure = (func_caMeasure)this->resolve("ca210Measure");
        pfcaZeroCal = (func_caZeroCal)this->resolve("ca210ZeroCal");
        //pfcaGetAverageMeasureXYZ = (func_caGetAverageMeasureXYZ)this->resolve("ca210GetAverageMeasureXYZ");
        pfcaSetSyncMode = (func_caSetSyncMode)this->resolve("ca210SetSyncMode");
        pfcaSetSpeed = (func_caSetSpeed)this->resolve("ca210SetSpeed");
        pfcaGetCATypeName = (func_caGetCATypeName)this->resolve("ca210GetCATypeName");
        pfcaGetCAVersionName = (func_caGetCAVersionName)this->resolve("ca210GetCAVersionName");

    }
}

Ca210DllCtr::~Ca210DllCtr()
{
    if(this->isLoaded() && pfcaDisConnect)
    {
        pfcaDisConnect();
        this->unload();
    }
}

//---------------------------------------------------------------------------
// Description  : Connect to CA210
// Input Value  : None
// Output Value : return true if connect success, else return false
//---------------------------------------------------------------------------
bool Ca210DllCtr::caConnect(long lChannelNO)
{
    if(this->isLoaded()) return true;
    if(this->isLoaded() && pfcaConnect)
    {
        return pfcaConnect(lChannelNO);
    }
    return false;
}

//---------------------------------------------------------------------------
// Description  : Select Channel CA210
// Input Value  : None
// Output Value : return true if connect success, else return false
//---------------------------------------------------------------------------
bool Ca210DllCtr::caSetChannel(long lChannelNO)
{
    if(this->isLoaded() && pfcaConnect)
    {
       return pfcaConnect(lChannelNO);
    }
    return false;
}

//---------------------------------------------------------------------------
// Description  : DisConnect to CA210
// Input Value  : None
// Output Value : None
//---------------------------------------------------------------------------
void Ca210DllCtr::caDisConnect()
{
    if(this->isLoaded()&&pfcaDisConnect)
    {
        pfcaDisConnect();
    }
}

//---------------------------------------------------------------------------
// Description  : Measure and get the data from ca210
// Input Value  : None
// Output Value : the structure include all data ca210 got once measure
//---------------------------------------------------------------------------
LPCA210DATASTRUCT Ca210DllCtr::caMeasure()
{
    if(this->isLoaded()&&pfcaMeasure)
    {
       c210_data = pfcaMeasure();
       return &c210_data;
    }
    return NULL;
}

//---------------------------------------------------------------------------
// Description  : Call Zero calculate function, user should set ca210 to 0-cal level
// Input Value  : None
// Output Value : None
//---------------------------------------------------------------------------
void Ca210DllCtr::caZeroCal()
{
    if(this->isLoaded()&&pfcaZeroCal)
    {
       pfcaZeroCal();
    }
}

//---------------------------------------------------------------------------
// Description  : Call Average Measure function
// Input Value  : None
// Output Value : None
//---------------------------------------------------------------------------
//XYZCOLOR caGetAverageMeasureXYZ(int nTimes);

//---------------------------------------------------------------------------
// Description  : Set Measure Sync Mode
// Input Value  : nSyncMode: select one from SYNC_MODE_TYPE
// Output Value : None
//---------------------------------------------------------------------------
void Ca210DllCtr::caSetSyncMode(int nSyncMode)
{
    if(this->isLoaded()&&pfcaSetSyncMode)
    {
       pfcaSetSyncMode(nSyncMode);
    }
}

//---------------------------------------------------------------------------
// Description  : Set Measure Speed
// Input Value  : nSpeed: select one from SPEED_TYPE
// Output Value : None
//---------------------------------------------------------------------------
void Ca210DllCtr::caSetSpeed(int nSpeed)
{
    if(this->isLoaded() && pfcaSetSpeed)
    {
        pfcaSetSpeed(nSpeed);
    }
}

//---------------------------------------------------------------------------
// Description  : Get CAType
// Input Value  : CAType
// Output Value : None
//---------------------------------------------------------------------------
int  Ca210DllCtr::caGetCATypeName(BYTE *bCAType)
{
    if(this->isLoaded() && pfcaGetCATypeName)
    {
       return pfcaGetCATypeName(bCAType);
    }
    return 0;
}

//---------------------------------------------------------------------------
// Description  : Get CAVersion
// Input Value  : CAVersion
// Output Value : None
//---------------------------------------------------------------------------
int  Ca210DllCtr::caGetCAVersionName(BYTE *bCAVersion)
{
    if(this->isLoaded() && pfcaGetCAVersionName)
    {
        return pfcaGetCAVersionName(bCAVersion);
    }
    return 0;
}

} //name space ca210
