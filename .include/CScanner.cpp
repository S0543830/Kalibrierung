#include "CScanner.h"
#include <Windows.h>

typedef int(*iINITROTOSCAN)(unsigned int* , const char* , unsigned int);
typedef int(*iMETHHANDLE)(unsigned int);
typedef int(*iMETHHANDLEDOUUBLE)(unsigned int , double , double);
typedef int(*iGETSINGLE)(unsigned int , std::list <SPunkt>& , std::list <double>& );
typedef int(*iGETRECORD)(unsigned int, std::list <SPunkt>&, std::list <double>&, bool);
typedef int(*iGETFILTERREC)(unsigned int, std::list<SPunkt>&, std::list<double>&, unsigned int, unsigned int);
typedef double(*dMETHHANDLE)(unsigned int);

CScanner::CScanner():
	m_iHandle(0),
	m_pRotoPluginDll(nullptr),
	m_ccIP(nullptr), m_uiPort(0),
	m_cDLLPfad(nullptr), m_iState(0)
{
}

CScanner::~CScanner()
{

}

//Initialisierug der Scanner
int CScanner::initScanner(const char* ccFileDLL, const char * ccIp, unsigned int uiPort)
{
#ifdef UNICODE
	this->m_pRotoPluginDll = LoadLibrary((LPCWSTR)ccFileDLL);
#else
	this->m_pRotoPluginDll = LoadLibrary(ccFileDLL);
#endif // !UNICODE

	int iReturn = -1;
	if (NULL != this->m_pRotoPluginDll)
	{
		iINITROTOSCAN pInitRotoScan = (iINITROTOSCAN)GetProcAddress(this->m_pRotoPluginDll, "initLaScanLib");
		if (NULL != pInitRotoScan)
		{
			iReturn = pInitRotoScan(&this->m_iHandle, ccIp, uiPort);
			this->m_ccIP = ccIp;
			this->m_uiPort = uiPort;
			this->m_cDLLPfad = ccFileDLL;
		}
	}
	
	return iReturn;

}

//Initialisiert den Scanner neu
int CScanner::reconnectScanner()
{
	try
	{
		int iReturn = -1;
		if (NULL != this->m_pRotoPluginDll)
		{
			iINITROTOSCAN pInitRotoScan = (iINITROTOSCAN)GetProcAddress(this->m_pRotoPluginDll, "initLaScanLib");
			if (NULL != pInitRotoScan)
			{
				iReturn = pInitRotoScan(&this->m_iHandle, this->m_ccIP, this->m_uiPort);
			}
		}
		else
		{
			//this->m_pRotoPluginDll = LoadLibrary(this->m_cDLLPfad);
			if (NULL != this->m_pRotoPluginDll)
			{
				iINITROTOSCAN pInitRotoScan = (iINITROTOSCAN)GetProcAddress(this->m_pRotoPluginDll, "initLaScanLib");
				if (NULL != pInitRotoScan)
				{
					iReturn = pInitRotoScan(&this->m_iHandle, this->m_ccIP, this->m_uiPort);
				}
			}
		}
		return iReturn;
	}
	catch (std::exception)
	{
		return -1;
	}
}

//Schließt die Verbindung mit dem Scanner
int CScanner::exitScanner()
{
	int iReturn = -1;
	if (NULL != this->m_pRotoPluginDll)
	{
		iMETHHANDLE pExitRotoScan = (iMETHHANDLE)GetProcAddress(this->m_pRotoPluginDll, "exitLaScanLib");
		if (NULL != pExitRotoScan)
		{
			iReturn = pExitRotoScan(this->m_iHandle);
		}
	}
	return iReturn;
}

//Start Scan
int CScanner::startRecording()
{
	int iReturn = -1;
	if (NULL != this->m_pRotoPluginDll)
	{
		iMETHHANDLE pStartScanRec = (iMETHHANDLE)GetProcAddress(this->m_pRotoPluginDll, "startScanRecording");
		if (NULL != pStartScanRec)
		{
			iReturn = pStartScanRec(this->m_iHandle);
		}
	}
	return iReturn;	
}

//Stoppt den Scan
int CScanner::stopRecording()
{
	int iReturn = -1;
	if (NULL != this->m_pRotoPluginDll)
	{
		iMETHHANDLE pStopScanRec = (iMETHHANDLE)GetProcAddress(this->m_pRotoPluginDll, "stopScanRecording");
		if (NULL != pStopScanRec)
		{
			iReturn = pStopScanRec(this->m_iHandle);
		}
	}
	return iReturn;
}

//Rufe Scanpunkte und Zeitstempel auf
int CScanner::getRecordedPoints(std::list<SPunkt>& lPunkte, std::list<double>& ldZeiten)
{
	int iReturn = -1;
	lPunkte.clear();
	ldZeiten.clear();

	if (NULL != this->m_pRotoPluginDll)
	{
		iGETRECORD pGetRecPoint = (iGETRECORD)GetProcAddress(this->m_pRotoPluginDll, "getRecordedScanPoints");
		if (NULL != pGetRecPoint)
		{
			iReturn = pGetRecPoint(this->m_iHandle,lPunkte, ldZeiten, true);
		}
	}
	return iReturn;
}

int CScanner::getFilterRecordedPoints(std::list<SPunkt>& lPunkte, std::list<double>& ldZeiten, unsigned int uiAnzahl, unsigned int iMode)
{
	int iReturn = -1;
	lPunkte.clear();
	ldZeiten.clear();

	if (NULL != this->m_pRotoPluginDll)
	{
		iGETFILTERREC pGetRecPoint = (iGETFILTERREC)GetProcAddress(this->m_pRotoPluginDll, "getFilterRecordPoints");
		if (NULL != pGetRecPoint)
		{
			iReturn = pGetRecPoint(this->m_iHandle, lPunkte, ldZeiten,uiAnzahl, iMode);
		}
	}
	return iReturn;
}

int CScanner::getSingleScan(std::list<SPunkt>& lPunkt, std::list<double>& ldZeiten)
{
	int iReturn = -1;
	lPunkt.clear();
	ldZeiten.clear();

	if (NULL != this->m_pRotoPluginDll)
	{
		iGETSINGLE pGetSingleScan = (iGETSINGLE)GetProcAddress(this->m_pRotoPluginDll, "getSingleScan");
		if (NULL != pGetSingleScan)
		{
			iReturn = pGetSingleScan(this->m_iHandle, lPunkt, ldZeiten);
		}
	}
	return iReturn;
}

int CScanner::setState(int iState)
{
	try
	{
		this->m_iState = iState;
		return 0;
	}
	catch (std::exception)
	{
		return -1;
	}
}

int CScanner::getState()
{
	return this->m_iState;
}
