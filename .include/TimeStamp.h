#ifndef __TIMESTAMP_H_INCLUDED__
#define __TIMESTAMP_H_INCLUDED__

// ---- local includes ------------------------------------------------------ ;

//#include "DebugOutput.h"


// ---- global includes ----------------------------------------------------- ;

#include <windows.h>

class CTimeStamp
{

public:
	CTimeStamp();
	~CTimeStamp();
	static double getTimeStamp(SYSTEMTIME* pSystemTime=NULL);
	static double getDuration (double clTimeStamp, double clOtherTimeStamp);

};

/************************************************************************/
/* TimeStamp mit inline Funktionen                                      */
/************************************************************************
inline double CTimeStamp::getTimeStamp()
{
	double dTimeStamp = 0.0;

#if 0
	_timeb nowTimeVal;
	_ftime_s ( &nowTimeVal );
	dTimeStamp = ((double) nowTimeVal.time + ((double)(nowTimeVal.millitm)) /1e+3);
#else

	static __int64 liFrequnency = 0;
	__int64 liCounterValue;

	if (0 == liFrequnency)
	{
	    if (false == QueryPerformanceFrequency((LARGE_INTEGER*)&liFrequnency))
		{
			_timeb nowTimeVal;
			_ftime_s ( &nowTimeVal );
			dTimeStamp = ((double) nowTimeVal.time + ((double)(nowTimeVal.millitm)) /1e+3);
			return dTimeStamp;
		}
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&liCounterValue);
	dTimeStamp = (((double)(liCounterValue))/((double)liFrequnency));
#endif
	return dTimeStamp;
}


inline double CTimeStamp::getDuration (double clTimeStamp, double clOtherTimeStamp)
{
	return clTimeStamp - clOtherTimeStamp;
}

*/

#endif // __CM_TIMESTAMP_H_INCLUDED__