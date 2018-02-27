#include "DeltaE.h"
#include<math.h>

RefsRGBDeltaStruct Refer_sRGBDelta[32] =
{
{12.25,0.00,0.00,},
{27.09,0.00,0.00,},
{40.73,0.00,0.00,},
{53.59,0.00,0.00,},
{65.87,0.00,0.00,},
{77.70,0.00,0.00,},
{89.18,0.00,0.00,},
{100.00,0.00,0.00,},
{25.53,48.05,38.06,},
{45.97,51.10,28.67,},
{53.23,80.11,67.22,},
{68.21,48.19,22.69,},
{46.23,-51.70,49.90,},
{68.87,-59.33,52.46,},
{87.74,-86.19,83.19,},
{90.63,-59.90,49.70,},
{12.98,47.51,-64.70,},
{35.13,40.25,-66.97,},
{32.30,79.19,-107.85,},
{59.20,33.10,-63.46,},
{51.87,-12.93,56.68,},
{75.62,-15.57,61.63,},
{97.14,-21.56,94.49,},
{97.77,-16.54,59.99,},
{29.78,58.94,-36.49,},
{50.31,65.91,-41.73,},
{60.32,98.25,-60.83,},
{72.17,64.94,-42.08,},
{48.26,-28.84,-8.47,},
{71.28,-33.97,-10.21,},
{91.12,-48.08,-14.13,},
{93.16,-35.23,-10.87,},
};
double fRefer_DeltaC[32] =
{
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
0.00,
61.30,
58.59,
104.57,
53.27,
71.86,
79.20,
119.78,
77.83,
80.27,
78.14,
133.81,
71.57,
58.14,
63.56,
96.92,
62.23,
69.32,
78.00,
115.56,
77.39,
30.06,
35.48,
50.12,
36.87,
};

const float fRefSRGB_100W_X = 0.95;
const float fRefSRGB_100W_Y = 1.00;
const float fRefSRGB_100W_Z = 1.09;
sRGBDeltaStruct sRGBDeltaE[32]={0,0,0};

double GetRelativ_XYZ(double fRaw,double fReference)
{
	return fRaw/fReference;
}

double GetFn_XYZ(double fX,double fReference)
{
    double fResult;
    double fTmp = fX/fReference;
  if(fTmp<= 0.008856)
 	{
 		fResult = (7.787*fTmp)+0.13793;
 	}
 	else
 	{
 		fResult = pow(fTmp,0.3333);
 	}
	return fResult;
}

double GetTestDisplay_L(double fY)
{
    double fResult;
  if(fY <= 0.008856)
 	{
 		fResult = 9.033*fY;
 	}
 	else
 	{
 		fResult = 1.16*pow(fY,0.3333) - 0.16;
 	}
	return fResult*100;
}
double GetTestDisplay_a(double fFnX,double fFnY)
{
    double fResult;
	fResult = 500*(fFnX - fFnY);

	return fResult;
}
double GetTestDisplay_b(double fFnY,double fFnZ)
{
    double fResult;
	fResult = 200*(fFnY - fFnZ);

	return fResult;
}
double GetDelta_L(int pattern_index,double fTestDisplay_L)
{
	return fTestDisplay_L-Refer_sRGBDelta[pattern_index].L;
}
double GetDelta_a(int pattern_index,double fTestDisplay_a)
{
	return fTestDisplay_a-Refer_sRGBDelta[pattern_index].a;
}
double GetDelta_b(int pattern_index,double fTestDisplay_b)
{
	return fTestDisplay_b-Refer_sRGBDelta[pattern_index].b;
}
double GetDeltaE_CIE76(double fDelta_L,double fDelta_a,double fDelta_b)
{
  double fResult;
  double fTmpL,fTmpa,fTmpb;
  fTmpL = pow(fDelta_L,2);
  fTmpa = pow(fDelta_a,2);
  fTmpb = pow(fDelta_b,2);
  fResult = fTmpL+fTmpa+fTmpb;
  fResult = pow(fResult,0.5);
	return fResult;
}

double GetMeas_C(double fTestDisplay_a,double fTestDisplay_b )
{
  double fResult;
  fResult = pow(fTestDisplay_a,2)+pow(fTestDisplay_b,2);
  fResult = sqrt (fResult);
	return fResult;
}
double GetDelta_C(int pattern_index,double fMeas_C )
{
  double fResult;
  fResult = fMeas_C - fRefer_DeltaC[pattern_index];
	return fResult;
}
double GetDelta_H(double fDeltaE_CIE1976,double fDelta_L,double fDelta_C)
{
  double fResult;
  double fTmpE,fTmpL,fTmpC;
  fTmpE = pow(fDeltaE_CIE1976,2);
  fTmpL = pow(fDelta_L,2);
  fTmpC = pow(fDelta_C,2);
  if((fTmpE-fTmpL-fTmpC)>= 0)
  {
  		fResult = fTmpE-fTmpL-fTmpC;
  		fResult = sqrt (fResult);
  }
  else
  {
  	fResult = 0;
  }

	return fResult;
}

double GetComp_C(int pattern_index,double fDelta_C )
{
  double fResult;
  fResult = 1+0.045*fabs(fRefer_DeltaC[pattern_index]);
  fResult = fDelta_C/fResult;
	return fResult;
}

double GetComp_H(int pattern_index,double fDelta_H )
{
  double fResult;
  fResult = 1+0.015*fabs(fRefer_DeltaC[pattern_index]);
  fResult = fDelta_H/fResult;
	return fResult;
}

double GetDeltaE_CIE94(double fComp_L,double fComp_C,double fComp_H)
{
  double fResult;
  double fTmpL,fTmpC,fTmpH;
  fTmpL = pow(fComp_L,2);
  fTmpC = pow(fComp_C,2);
  fTmpH = pow(fComp_H,2);
  fResult = fTmpL+fTmpC+fTmpH;
  fResult = sqrt (fResult);
	return fResult;
}
float GetDeltaE_OnePatCIE94(int pattern_index, float fX, float fY, float fZ, float f100W_Raw_Y)
{

    sRGBDeltaE[pattern_index].fRaw_X = fX;
    sRGBDeltaE[pattern_index].fRaw_Y = fY;
    sRGBDeltaE[pattern_index].fRaw_Z = fZ;

    sRGBDeltaE[pattern_index].fRelative_X = GetRelativ_XYZ(sRGBDeltaE[pattern_index].fRaw_X,f100W_Raw_Y);
    sRGBDeltaE[pattern_index].fRelative_Y = GetRelativ_XYZ(sRGBDeltaE[pattern_index].fRaw_Y,f100W_Raw_Y);
    sRGBDeltaE[pattern_index].fRelative_Z = GetRelativ_XYZ(sRGBDeltaE[pattern_index].fRaw_Z,f100W_Raw_Y);

    sRGBDeltaE[pattern_index].fFn_X = GetFn_XYZ(sRGBDeltaE[pattern_index].fRelative_X,fRefSRGB_100W_X);
    sRGBDeltaE[pattern_index].fFn_Y = GetFn_XYZ(sRGBDeltaE[pattern_index].fRelative_Y,fRefSRGB_100W_Y);
    sRGBDeltaE[pattern_index].fFn_Z = GetFn_XYZ(sRGBDeltaE[pattern_index].fRelative_Z,fRefSRGB_100W_Z);

    sRGBDeltaE[pattern_index].fTestDisplay_L = GetTestDisplay_L(sRGBDeltaE[pattern_index].fRelative_Y);
    sRGBDeltaE[pattern_index].fTestDisplay_a = GetTestDisplay_a(sRGBDeltaE[pattern_index].fFn_X,sRGBDeltaE[pattern_index].fFn_Y);
    sRGBDeltaE[pattern_index].fTestDisplay_b = GetTestDisplay_b(sRGBDeltaE[pattern_index].fFn_Y,sRGBDeltaE[pattern_index].fFn_Z);

    sRGBDeltaE[pattern_index].fDelta_L = GetDelta_L(pattern_index,sRGBDeltaE[pattern_index].fTestDisplay_L);
    sRGBDeltaE[pattern_index].fDelta_a = GetDelta_a(pattern_index,sRGBDeltaE[pattern_index].fTestDisplay_a);
    sRGBDeltaE[pattern_index].fDelta_b = GetDelta_b(pattern_index,sRGBDeltaE[pattern_index].fTestDisplay_b);

    sRGBDeltaE[pattern_index].fDeltaE_CIE76 = GetDeltaE_CIE76(sRGBDeltaE[pattern_index].fDelta_L,sRGBDeltaE[pattern_index].fDelta_a,sRGBDeltaE[pattern_index].fDelta_b);

    sRGBDeltaE[pattern_index].fMeas_C = GetMeas_C(sRGBDeltaE[pattern_index].fTestDisplay_a,sRGBDeltaE[pattern_index].fTestDisplay_b);
    sRGBDeltaE[pattern_index].fDelta_C = GetDelta_C(pattern_index,sRGBDeltaE[pattern_index].fMeas_C);
    sRGBDeltaE[pattern_index].fDelta_H = GetDelta_H(sRGBDeltaE[pattern_index].fDeltaE_CIE76,sRGBDeltaE[pattern_index].fDelta_L,sRGBDeltaE[pattern_index].fDelta_C);

    sRGBDeltaE[pattern_index].fComp_L = sRGBDeltaE[pattern_index].fDelta_L;
    sRGBDeltaE[pattern_index].fComp_C = GetComp_C(pattern_index,sRGBDeltaE[pattern_index].fDelta_C);
    sRGBDeltaE[pattern_index].fComp_H = GetComp_H(pattern_index,sRGBDeltaE[pattern_index].fDelta_H);

    sRGBDeltaE[pattern_index].fDelta_E = GetDeltaE_CIE94(sRGBDeltaE[pattern_index].fComp_L,sRGBDeltaE[pattern_index].fComp_C,sRGBDeltaE[pattern_index].fComp_H);
    return sRGBDeltaE[pattern_index].fDelta_E;
}

#if 0
double sRGB_DeltaEVerify(void)
{
	int pattern_index=0;
    double fResult = 0;
    double fTmpL,fTmpC,fTmpH;
    double f100W_Raw_Y = 0;
		.....;//SEND 100% White pattern
		.....;//延时300ms		
		.....;//获取CA210 的XYZ值
       f100W_Raw_Y = ....;//		
	for(pattern_index = 0;pattern_index<32;pattern_index++)
	{

		.....;//发送测试Pattern
		.....;//延时300ms
		.....;//获取CA210 的XYZ值
		sRGBDeltaE[pattern_index].fRaw_X = .....;
		sRGBDeltaE[pattern_index].fRaw_Y = .....;
		sRGBDeltaE[pattern_index].fRaw_Z = .....;
		
		
		sRGBDeltaE[pattern_index].fRelative_X = GetRelativ_XYZ(sRGBDeltaE[pattern_index].fRaw_X,f100W_Raw_Y);
		sRGBDeltaE[pattern_index].fRelative_Y = GetRelativ_XYZ(sRGBDeltaE[pattern_index].fRaw_Y,f100W_Raw_Y);		
		sRGBDeltaE[pattern_index].fRelative_Z = GetRelativ_XYZ(sRGBDeltaE[pattern_index].fRaw_Z,f100W_Raw_Y);				
		
		sRGBDeltaE[pattern_index].fFn_X = GetFn_XYZ(sRGBDeltaE[pattern_index].fRelative_X,fRefSRGB_100W_X);
		sRGBDeltaE[pattern_index].fFn_Y = GetFn_XYZ(sRGBDeltaE[pattern_index].fRelative_Y,fRefSRGB_100W_Y);		
		sRGBDeltaE[pattern_index].fFn_Z = GetFn_XYZ(sRGBDeltaE[pattern_index].fRelative_Z,fRefSRGB_100W_Z);					
		
		sRGBDeltaE[pattern_index].fTestDisplay_L = GetTestDisplay_L(sRGBDeltaE[pattern_index].fRelative_Y);
		sRGBDeltaE[pattern_index].fTestDisplay_a = GetTestDisplay_a(sRGBDeltaE[pattern_index].fFn_X,sRGBDeltaE[pattern_index].fFn_Y);		
		sRGBDeltaE[pattern_index].fTestDisplay_b = GetTestDisplay_b(sRGBDeltaE[pattern_index].fFn_Y,sRGBDeltaE[pattern_index].fFn_Z);					
		
		sRGBDeltaE[pattern_index].fDelta_L = GetDelta_L(pattern_index,sRGBDeltaE[pattern_index].fTestDisplay_L);
		sRGBDeltaE[pattern_index].fDelta_a = GetDelta_a(pattern_index,sRGBDeltaE[pattern_index].fTestDisplay_a);		
		sRGBDeltaE[pattern_index].fDelta_b = GetDelta_b(pattern_index,sRGBDeltaE[pattern_index].fTestDisplay_b);					
		
		sRGBDeltaE[pattern_index].fDeltaE_CIE76 = GetDeltaE_CIE76(sRGBDeltaE[pattern_index].fDelta_L,sRGBDeltaE[pattern_index].fDelta_a,sRGBDeltaE[pattern_index].fDelta_b);					

		sRGBDeltaE[pattern_index].fMeas_C = GetMeas_C(RGBDeltaE[pattern_index].fTestDisplay_a,RGBDeltaE[pattern_index].fTestDisplay_b);
		sRGBDeltaE[pattern_index].fDelta_C = GetDelta_C(pattern_index,sRGBDeltaE[pattern_index].fMeas_C);		
		sRGBDeltaE[pattern_index].fDelta_H = GetDelta_H(sRGBDeltaE[pattern_index].fDeltaE_CIE76,sRGBDeltaE[pattern_index].fDelta_L,sRGBDeltaE[pattern_index].fDelta_C);						
		
		sRGBDeltaE[pattern_index].fComp_L = sRGBDeltaE[pattern_index].fDelta_L;
		sRGBDeltaE[pattern_index].fComp_C = GetComp_C(pattern_index,sRGBDeltaE[pattern_index].fDelta_C);		
		sRGBDeltaE[pattern_index].fComp_H = GetComp_H(pattern_index,sRGBDeltaE[pattern_index].fDelta_H);				
		
		sRGBDeltaE[pattern_index].fDelta_E = GetDeltaE_CIE94(sRGBDeltaE[pattern_index].fComp_L,sRGBDeltaE[pattern_index].fComp_C,sRGBDeltaE[pattern_index].fComp_H);
		fResult += sRGBDeltaE[pattern_index].fDelta_E;
		
	}
		fResult = fResult/32;
		return fResult;
}
#endif
