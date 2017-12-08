#include "LaScanLib.h"
#include "SickScan.h"
#include "TimeStamp.h"
#include <string>
#include <vector>


std::vector<CSickScan*> vecObjSiSc;

LASCANLIB_API int initLaScanLib (unsigned int* iHandle, const char* szHostName, unsigned int uiPortNumber)
{
	int iRetVal = ERR_LASCANLIB_UNKOWN_ERROR;

	CSickScan* objScan = new CSickScan();
	
	if (nullptr != objScan )
	{
		*iHandle = static_cast<int>(vecObjSiSc.size());
		vecObjSiSc.push_back(objScan);

		char cTmpPort[5];
		sprintf_s(cTmpPort, "%u", uiPortNumber);

		iRetVal = vecObjSiSc.back()->initSickScan(szHostName, cTmpPort);
	}
	else
	{
		iRetVal = ERR_LASCANLIB_SCANNER_NULL;
	}
	
	return iRetVal;
}

LASCANLIB_API int exitLaScanLib (unsigned int iHandle)
{
	int iRetVal = ERR_LASCANLIB_UNKOWN_ERROR;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner )
		{
			iRetVal = pScanner->exitSickScan();
			if (0 == iRetVal)
			{
				delete pScanner;
				vecObjSiSc.at(iHandle) = nullptr;
				int iRetVal = LASCANLIB_OK;
			}
		}
		else
		{
			iRetVal = ERR_LASCANLIB_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_LASCANLIB_SCANNER_ID;
	}

	return iRetVal;
}

LASCANLIB_API int setAngleFilterLimits (unsigned int iHandle,double dMinAngle, double dMaxAngle)
{	
	int iRetVal = LASCANLIB_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner )
		{
			pScanner->setAngleFilterLimits(dMinAngle, dMaxAngle);
		}
		else
		{
			iRetVal = ERR_LASCANLIB_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_LASCANLIB_SCANNER_ID;
	}
	return iRetVal;
}

LASCANLIB_API int setDistanceFilterLimits (unsigned int iHandle,double dMinDist, double dMaxDist)
{
	int iRetVal = LASCANLIB_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner )
		{
			pScanner->setDistanceFilterLimits(dMinDist, dMaxDist);
		}
		else
		{
			iRetVal = ERR_LASCANLIB_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_LASCANLIB_SCANNER_ID;
	}
	return iRetVal;
}

LASCANLIB_API int getRotoScanStatus (unsigned int iHandle)
{
	int iRetVal = LASCANLIB_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner )
		{
			iRetVal = pScanner->getStatus();
		}
		else
		{
			iRetVal = ERR_LASCANLIB_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_LASCANLIB_SCANNER_ID;
	}
	return iRetVal;
}

LASCANLIB_API int getSingleScan (unsigned int iHandle,
								std::list<SPunkt> &rlstclPoints,
							    std::list<double> &rlstdTimeStamps)
{
	int iRetVal = LASCANLIB_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner )
		{
			iRetVal = pScanner->getSingleScan(rlstclPoints, rlstdTimeStamps);
			if (iRetVal != 0)
			{
				iRetVal = ERR_LASCANLIB_UNKOWN_ERROR;
			}
		}
		else
		{
			iRetVal = ERR_LASCANLIB_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_LASCANLIB_SCANNER_ID;
	}

	return iRetVal;
}

LASCANLIB_API int startScanRecording (unsigned int iHandle)
{
	int iRetVal = LASCANLIB_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner)
		{
			iRetVal = pScanner->startScanRecording();
			if (iRetVal != 0)
			{
				iRetVal = ERR_LASCANLIB_UNKOWN_ERROR;
			}
		}
		else
		{
			iRetVal = ERR_LASCANLIB_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_LASCANLIB_SCANNER_ID;
	}

	return iRetVal;
}

LASCANLIB_API int stopScanRecording (unsigned int iHandle)
{
	int iRetVal = LASCANLIB_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner )
		{
			iRetVal = pScanner->stopScanRecording();
			if (iRetVal != 0)
			{
				iRetVal = ERR_LASCANLIB_UNKOWN_ERROR;
			}
		}
		else
		{
			iRetVal = ERR_LASCANLIB_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_LASCANLIB_SCANNER_ID;
	}

	return iRetVal;
}

LASCANLIB_API int getFilterRecordPoints(unsigned int iHandle, std::list<SPunkt>& rlstclPoints, std::list<double>& rlstdTimeStamps, unsigned int iAnzahl)
{
	int iRetVal = LASCANLIB_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner)
		{
			iRetVal = pScanner->getFilterRecordPoints(rlstclPoints,
				rlstdTimeStamps,
				iAnzahl);
			if (iRetVal != 0)
			{
				iRetVal = ERR_LASCANLIB_UNKOWN_ERROR;
			}
		}
		else
		{
			iRetVal = ERR_LASCANLIB_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_LASCANLIB_SCANNER_ID;
	}

	return iRetVal;
}


LASCANLIB_API int getRecordedScanPoints (unsigned int iHandle, 
										std::list <SPunkt>& rlstclPoints,
										std::list <double>& rlstdTimeStamps, 
										bool bInterpoleateTimeStamps )
{
	int iRetVal = LASCANLIB_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner )
		{
			iRetVal = pScanner->getRecordedScanPoints(rlstclPoints,
																	rlstdTimeStamps,
																	bInterpoleateTimeStamps);
			if (iRetVal != 0)
			{
				iRetVal = ERR_LASCANLIB_UNKOWN_ERROR;
			}
		}
		else
		{
			iRetVal = ERR_LASCANLIB_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_LASCANLIB_SCANNER_ID;
	}

	return iRetVal;
}

LASCANLIB_API double getTimeStamp ( )
{
	return CTimeStamp::getTimeStamp();
}
