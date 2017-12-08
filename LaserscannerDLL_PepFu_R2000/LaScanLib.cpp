#include "LaScanLib.h"
#include "PaFR2000Scanner.h"
#include <string>
#include <vector>



std::vector<PaFR2000Scanner*> p_vecPaFScanner;

LASCANLIB_API int initLaScanLib(unsigned int* iHandle, const char* szHostName, unsigned int uiPortNumber)
{
	int iRetVal = ERR_LASCANLIB_UNKOWN_ERROR;
	PaFR2000Scanner* p_objScan = new PaFR2000Scanner();

	if (nullptr != p_objScan)
	{
		*iHandle = static_cast<int>(p_vecPaFScanner.size());
		p_vecPaFScanner.push_back(p_objScan);
	
		iRetVal = p_vecPaFScanner.back()->initScanner(szHostName, static_cast<unsigned short>(uiPortNumber), "A");
	}
	else
	{
		iRetVal = ERR_LASCANLIB_SCANNER_NULL;
	}

	return iRetVal;
}

LASCANLIB_API int exitLaScanLib(unsigned int iHandle)
{
	int iRetVal = ERR_LASCANLIB_UNKOWN_ERROR;

	if (p_vecPaFScanner.size() > iHandle)
	{
		PaFR2000Scanner* p_pScanner = p_vecPaFScanner.at(iHandle);

		if (nullptr != p_pScanner)
		{
			iRetVal = p_pScanner->closeConnection();

			if (0 == iRetVal)
			{
				delete p_pScanner;
				p_vecPaFScanner.at(iHandle) = nullptr;
				iRetVal = LASCANLIB_OK;
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

LASCANLIB_API int setAngleFilterLimits(unsigned int iHandle, double dMinAngle, double dMaxAngle)
{
	int iRetVal = LASCANLIB_OK;

	if (p_vecPaFScanner.size() > iHandle)
	{
		PaFR2000Scanner * p_pScanner = p_vecPaFScanner.at(iHandle);

		if (nullptr != p_pScanner )
		{
			p_pScanner->setAngleFilterLimits(dMinAngle, dMaxAngle);
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

LASCANLIB_API int setDistanceFilterLimits(unsigned int iHandle, double dMinDist, double dMaxDist)
{
	int iRetVal = LASCANLIB_OK;


	if (p_vecPaFScanner.size() > iHandle)
	{
		PaFR2000Scanner * p_pScanner = p_vecPaFScanner.at(iHandle);

		if (nullptr != p_pScanner )
		{
			p_pScanner->setDistanceFilterLimits(dMinDist, dMaxDist);
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

LASCANLIB_API int getLaScanLibStatus(unsigned int iHandle)
{
	int iRetVal = LASCANLIB_OK;


	if (p_vecPaFScanner.size() > iHandle)
	{
		PaFR2000Scanner * p_pScanner = p_vecPaFScanner.at(iHandle);

		if (nullptr != p_pScanner )
		{
			iRetVal = p_pScanner->getScanStatus();
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

LASCANLIB_API int getSingleScan(unsigned int iHandle, std::list<SPunkt> &rlstclPoints, std::list<double> &rlstdTimeStamps)
{
	int iRetVal = LASCANLIB_OK;


	if (p_vecPaFScanner.size() > iHandle)
	{
		PaFR2000Scanner * p_pScanner = p_vecPaFScanner.at(iHandle);

		if (nullptr != p_pScanner)
		{
			iRetVal = p_pScanner->getSingleScan(rlstclPoints, rlstdTimeStamps);
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

LASCANLIB_API int startScanRecording(unsigned int iHandle)
{
	int iRetVal = LASCANLIB_OK;

	if (p_vecPaFScanner.size() > iHandle)
	{
		PaFR2000Scanner * p_pScanner = p_vecPaFScanner.at(iHandle);

		if (nullptr != p_pScanner)
		{
			iRetVal = p_pScanner->startScanOutput();
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

LASCANLIB_API int stopScanRecording(unsigned int iHandle)
{
	int iRetVal = LASCANLIB_OK;

	if (p_vecPaFScanner.size() > iHandle)
	{
		PaFR2000Scanner * p_pScanner = p_vecPaFScanner.at(iHandle);

		if (nullptr != p_pScanner)
		{
			iRetVal = p_pScanner->stopScanOutput();
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

LASCANLIB_API int getFilterRecordPoints(unsigned int iHandle, std::list <SPunkt>& rlstclPoints, std::list <double>& rlstdTimeStamps, unsigned int iAnzahl, unsigned int iMode)
{
	int iRetVal = LASCANLIB_OK;

	if (p_vecPaFScanner.size() > iHandle)
	{
		PaFR2000Scanner * p_pScanner = p_vecPaFScanner.at(iHandle);

		if (nullptr != p_pScanner)
		{
			iRetVal = p_pScanner->getFilterRecordPoints(rlstclPoints, rlstdTimeStamps,iAnzahl, iMode);
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

LASCANLIB_API int getRecordedScanPoints(unsigned int iHandle, std::list <SPunkt>& rlstclPoints, std::list <double>& rlstdTimeStamps,
	bool bInterpoleateTimeStamps)
{
	int iRetVal = LASCANLIB_OK;
	bInterpoleateTimeStamps;
	if (p_vecPaFScanner.size() > iHandle)
	{
		PaFR2000Scanner * p_pScanner = p_vecPaFScanner.at(iHandle);

		if (nullptr != p_pScanner)
		{
			iRetVal = p_pScanner->getRecordPoints(rlstclPoints, rlstdTimeStamps);
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

LASCANLIB_API double getTimeStamp()
{
	return CTimeStamp::getTimeStamp();
}
