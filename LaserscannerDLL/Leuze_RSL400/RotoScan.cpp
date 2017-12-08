#include "RSL430Scan.h"
#include "RotoScan.h"
#include "TimeStamp.h"
#include <vector>

std::vector<CRSL430Scan*> vecObjLeSc;

ROTOSCAN_API int initRotoScan (unsigned int* iHandle, const char* szHostName, unsigned int uiPortNumber)
{
	int iRetVal = ERR_ROTOSCAN_UNKOWN_ERROR;

	CRSL430Scan* objScan = new CRSL430Scan();

	if (nullptr != objScan)
	{
		*iHandle = static_cast<int>(vecObjLeSc.size());
		vecObjLeSc.push_back(objScan);

		iRetVal = vecObjLeSc.back()->initRSL430UDP(szHostName, uiPortNumber);
		if (RSL430_RETURN_OK != iRetVal)
		{
			if (RSL430_RETURN_INVALIDSOCKET == iRetVal)
			{
				iRetVal = MY_SOCKET_ERROR;
			}
			else if (RSL430_RETURN_BINDFAIL == iRetVal)
			{
				iRetVal = MY_SOCKET_ERROR_CREATE;
			}
			else if (RSL430_RETURN_ERROR == iRetVal)
			{
				iRetVal = ERR_ROTOSCAN_UNKOWN_ERROR;
			}
		}
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

	if (vecObjLeSc.size() > iHandle)
	{
		CRSL430Scan * pScanner = vecObjLeSc.at(iHandle);
		if (nullptr != pScanner)
		{
			iRetVal = pScanner->exitRSL430();
			if (0 == iRetVal)
			{
				delete pScanner;
				vecObjLeSc.at(iHandle) = nullptr;
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

ROTOSCAN_API int setAngleFilterLimits (unsigned int iHandle, double dMinAngle, double dMaxAngle)
{	
	int iRetVal = ROTOSCAN_OK;

	if (vecObjLeSc.size() > iHandle)
	{
		CRSL430Scan * pScanner = vecObjLeSc.at(iHandle);

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

ROTOSCAN_API int setDistanceFilterLimits (unsigned int iHandle, double dMinDist, double dMaxDist)
{
	int iRetVal = ROTOSCAN_OK;

	if (vecObjLeSc.size() > iHandle)
	{
		CRSL430Scan * pScanner = vecObjLeSc.at(iHandle);

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

	if (vecObjLeSc.size() > iHandle)
	{
		CRSL430Scan * pScanner = vecObjLeSc.at(iHandle);

		if (nullptr != pScanner)
		{
			iRetVal = pScanner->getRSL430Status();
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

ROTOSCAN_API int getSingleScan (unsigned int iHandle,std::list <CPunkt>& rlstclPoints, std::list <double>& rlstdTimeStamps)
{
	int iRetVal = ROTOSCAN_OK;

	if (vecObjLeSc.size() > iHandle)
	{
		CRSL430Scan * pScanner = vecObjLeSc.at(iHandle);

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

	if (vecObjLeSc.size() > iHandle)
	{
		CRSL430Scan * pScanner = vecObjLeSc.at(iHandle);

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

	if (vecObjLeSc.size() > iHandle)
	{
		CRSL430Scan * pScanner = vecObjLeSc.at(iHandle);

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


ROTOSCAN_API int getRecordedScanPoints (unsigned int iHandle, std::list <CPunkt>& rlstclPoints,
										std::list <double>& rlstdTimeStamps, 
										bool bInterpoleateTimeStamps)
{
	int iRetVal = ROTOSCAN_OK;

	if (vecObjLeSc.size() > iHandle)
	{
		CRSL430Scan * pScanner = vecObjLeSc.at(iHandle);

		if (nullptr != pScanner )
		{
			iRetVal = pScanner->getRecordedScanPoints(rlstclPoints, rlstdTimeStamps );
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
