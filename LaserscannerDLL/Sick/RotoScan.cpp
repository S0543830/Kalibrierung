#include "RotoScan.h"
#include "SickScan.h"
#include "TimeStamp.h"
#include <string>
#include <vector>

using namespace std;

std::vector<CSickScan*> vecObjSiSc;

ROTOSCAN_API int initRotoScan (unsigned int* iHandle, const char* szHostName, unsigned int uiPortNumber)
{
	int iRetVal = ERR_ROTOSCAN_UNKOWN_ERROR;

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
		iRetVal = ERR_ROTOSCAN_SCANNER_NULL;
	}
	
	return iRetVal;
}

ROTOSCAN_API int exitRotoScan (unsigned int iHandle)
{
	int iRetVal = ERR_ROTOSCAN_UNKOWN_ERROR;

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
				int iRetVal = ROTOSCAN_OK;
			}
		}
		else
		{
			iRetVal = ERR_ROTOSCAN_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_ROTOSCAN_SCANNER_ID;
	}

	return iRetVal;
}

ROTOSCAN_API int setAngleFilterLimits (unsigned int iHandle,double dMinAngle, double dMaxAngle)
{	
	int iRetVal = ROTOSCAN_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner )
		{
			pScanner->setAngleFilterLimits(dMinAngle, dMaxAngle);
		}
		else
		{
			iRetVal = ERR_ROTOSCAN_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_ROTOSCAN_SCANNER_ID;
	}
	return iRetVal;
}

ROTOSCAN_API int setDistanceFilterLimits (unsigned int iHandle,double dMinDist, double dMaxDist)
{
	int iRetVal = ROTOSCAN_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner )
		{
			pScanner->setDistanceFilterLimits(dMinDist, dMaxDist);
		}
		else
		{
			iRetVal = ERR_ROTOSCAN_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_ROTOSCAN_SCANNER_ID;
	}
	return iRetVal;
}

ROTOSCAN_API int getRotoScanStatus (unsigned int iHandle)
{
	int iRetVal = ROTOSCAN_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner )
		{
			iRetVal = pScanner->getStatus();
		}
		else
		{
			iRetVal = ERR_ROTOSCAN_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_ROTOSCAN_SCANNER_ID;
	}
	return iRetVal;
}

ROTOSCAN_API int getSingleScan (unsigned int iHandle,
								std::list<CPunkt> &rlstclPoints,
							    std::list<double> &rlstdTimeStamps)
{
	int iRetVal = ROTOSCAN_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner )
		{
			iRetVal = pScanner->getSingleScan(rlstclPoints, rlstdTimeStamps);
			if (iRetVal != 0)
			{
				iRetVal = ERR_ROTOSCAN_UNKOWN_ERROR;
			}
		}
		else
		{
			iRetVal = ERR_ROTOSCAN_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_ROTOSCAN_SCANNER_ID;
	}

	return iRetVal;
}

ROTOSCAN_API int startScanRecording (unsigned int iHandle)
{
	int iRetVal = ROTOSCAN_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner)
		{
			iRetVal = pScanner->startScanRecording();
			if (iRetVal != 0)
			{
				iRetVal = ERR_ROTOSCAN_UNKOWN_ERROR;
			}
		}
		else
		{
			iRetVal = ERR_ROTOSCAN_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_ROTOSCAN_SCANNER_ID;
	}

	return iRetVal;
}

ROTOSCAN_API int stopScanRecording (unsigned int iHandle)
{
	int iRetVal = ROTOSCAN_OK;

	if (vecObjSiSc.size() > iHandle)
	{
		CSickScan * pScanner = vecObjSiSc.at(iHandle);

		if (nullptr != pScanner )
		{
			iRetVal = pScanner->stopScanRecording();
			if (iRetVal != 0)
			{
				iRetVal = ERR_ROTOSCAN_UNKOWN_ERROR;
			}
		}
		else
		{
			iRetVal = ERR_ROTOSCAN_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_ROTOSCAN_SCANNER_ID;
	}

	return iRetVal;
}


ROTOSCAN_API int getRecordedScanPoints (unsigned int iHandle, 
										std::list <CPunkt>& rlstclPoints,
										std::list <double>& rlstdTimeStamps, 
										bool bInterpoleateTimeStamps )
{
	int iRetVal = ROTOSCAN_OK;

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
				iRetVal = ERR_ROTOSCAN_UNKOWN_ERROR;
			}
		}
		else
		{
			iRetVal = ERR_ROTOSCAN_SCANNER_NULL;
		}
	}
	else
	{
		iRetVal = ERR_ROTOSCAN_SCANNER_ID;
	}

	return iRetVal;
}

ROTOSCAN_API double getTimeStamp (unsigned int iHandle)
{
	return CTimeStamp::getTimeStamp();
}
