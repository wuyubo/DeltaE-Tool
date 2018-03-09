#ifndef _DELTAE_H_
#define _DELTAE_H_

#ifdef __cplusplus
extern "C" {
#endif
typedef struct
{
    float fRaw_X;
    float fRaw_Y;
    float fRaw_Z;
    float fRelative_X;
    float fRelative_Y;
    float fRelative_Z;
    float fFn_X;
    float fFn_Y;
    float fFn_Z;
    float fTestDisplay_L;
    float fTestDisplay_a;
    float fTestDisplay_b;
    float fDelta_L;
    float fDelta_a;
    float fDelta_b;
    float fDeltaE_CIE76;
    float fMeas_C;
    float fDelta_C;
    float fDelta_H;
    float fComp_L;
    float fComp_C;
    float fComp_H;
	
    float fDelta_E;		//final result
}sRGBDeltaStruct;

typedef struct
{

    float L;
    float a;
    float b;
}RefsRGBDeltaStruct;


double GetRelativ_XYZ(double fRaw,double fReference);
double GetFn_XYZ(double fX,double fReference);
double GetTestDisplay_L(double fY);double GetTestDisplay_a(double fFnX,double fFnY);
double GetTestDisplay_b(double fFnY,double fFnZ);
double GetDelta_L(int pattern_index,double fTestDisplay_L);
double GetDelta_a(int pattern_index,double fTestDisplay_a);
double GetDelta_b(int pattern_index,double fTestDisplay_b);
double GetDeltaE_CIE76(double fDelta_L,double fDelta_a,double fDelta_b);
double GetMeas_C(double fTestDisplay_a,double fTestDisplay_b );
double GetDelta_C(int pattern_index,double fMeas_C );
double GetDelta_H(double fDeltaE_CIE1976,double fDelta_L,double fDelta_C);
double GetComp_C(int pattern_index,double fDelta_C );
double GetComp_H(int pattern_index,double fDelta_H );
double GetDeltaE_CIE94(double fComp_L,double fComp_C,double fComp_H);
float GetDeltaE_OnePatCIE94(int pattern_index, float fX, float fY, float fZ, float f100W_Raw_Y);
sRGBDeltaStruct *getsRGBDeltaData();
#ifdef __cplusplus
}//extern "C" {
#endif
#endif //_DELTAE_H_
