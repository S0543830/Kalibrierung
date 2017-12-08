#include "TimeStamp.h"
#include <sys/timeb.h>


/************************************************************************/
/*                                                                      */
/************************************************************************/
CTimeStamp::CTimeStamp()
{

}


/************************************************************************/
/*                                                                      */
/************************************************************************/
CTimeStamp::~CTimeStamp()
{

}


/************************************************************************/
/*                                                                      */
/************************************************************************/
double CTimeStamp::getTimeStamp( SYSTEMTIME* pSystemTime )
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
	if (NULL!=pSystemTime)
	{
		GetLocalTime(pSystemTime);
	}
	
	return dTimeStamp;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
double CTimeStamp::getDuration(double clTimeStamp, double clOtherTimeStamp)
{
	return clTimeStamp - clOtherTimeStamp;
}