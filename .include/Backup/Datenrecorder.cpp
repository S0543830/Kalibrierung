#include "stdafx.h"
#include "Datenrecorder.h"
#include "XMLParameter.h"

//Initialisierung der Klasse
CDatenrecorder::CDatenrecorder()
{
	this->bRec = false;
	this->iAnzahlScanner = 0;
	this->iTimeOutCheckConnection = 10000;
	this->bIsInitScanner = false;
	this->bIsInitEncoder = false;
	this->bEncoderRec = false;
	this->iScannerThreadState = SCANNER_NOT_RUNNING;
	this->iEncoderThreadState = ENCODER_NOT_RUNNING;
	this->critListScanData = new CRITICAL_SECTION;
	this->critListEncoderData = new CRITICAL_SECTION;
	InitializeCriticalSection(this->critListScanData);
	InitializeCriticalSection(this->critListEncoderData);
	this->bIsDataToGet = false;
	this->dAktPos = 0;

}

//Deinitialisierung der Klasse
CDatenrecorder::~CDatenrecorder()
{
	for (unsigned int ui = 0; ui < this->vobjThread_Record.size(); ui++)
	{
		this->vobjThread_Record.at(ui)->terminateThread();
		this->vobjScanner.at(ui)->exitScanner();
	}
	Sleep(0);

}

//Methode zur Initialisierung des Encoders sowie die Laserscanner
int CDatenrecorder::initSystemOverXML(char * FilePfad)
{
	int iReturn = 1;
	std::string sCOM;
	std::string sParity;
	std::string sDLL_Pfad;
	int iBaud;
	int iData;
	int iStop;
	int iVal;
	const char * ccScannerDLL_Pfad;
	const char * ccIP;
	unsigned int uiScannerID;
	std::vector<unsigned int> vuiScannerID;
	std::vector<ScannerInitParameter> vScannerConfigData;
	//Value from XML for Encoder
	iReturn = this->getEncorderDataFromXML(FilePfad, sDLL_Pfad, sCOM, iVal, iBaud, sParity, iData, iStop);
	if (iReturn == 0)
	{
		const char * ccDLL_Pfad = sDLL_Pfad.c_str();
		//Initialisierung des Encoders
		iReturn = this->initEncoder(ccDLL_Pfad, iVal,(char*) sCOM.c_str(), iBaud, sParity[0], iData, iStop);

		if (iReturn == 0)
		{
			//Value from XML for Laserscanner
			iReturn = getScannerDataFromXML(FilePfad, vScannerConfigData);
			for (int i = 0; i < vScannerConfigData.size(); i++)
			{
				ccScannerDLL_Pfad = vScannerConfigData.at(i).ccDLLPfad.c_str();
				ccIP = vScannerConfigData.at(i).sIP.c_str();
				//Initialisierung des Scanners
				iReturn = this->initScanner(ccScannerDLL_Pfad, ccIP, vScannerConfigData.at(i).iPort, uiScannerID);
				if (iReturn == 0)
				{
					vuiScannerID.push_back(uiScannerID);
				}
			}
		}
		return iReturn;
	}
	else
	{
		return iReturn;
	}

}

//Ermittelt die Werte aus der XML für den Encoder
int CDatenrecorder::getEncorderDataFromXML(char* cPfad, std::string& ccDLLPfad, std::string& sCOM, int& iVal, int& iBaud, std::string& sParity, int& iData, int& iStop)
{
	try
	{
		int iReturn = -1;
		iReturn = CXMLParameter::GetWertAusXML_Str(cPfad, &ccDLLPfad, 4, "Thuema", "Sensoren", "Encoder", "DLL_Name");
		iReturn = CXMLParameter::GetWertAusXML_Str(cPfad, &sCOM, 4, "Thuema", "Sensoren", "Encoder", "COM");
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iBaud, 4, "Thuema", "Sensoren", "Encoder", "Baud");
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iVal, 4, "Thuema", "Sensoren", "Encoder", "Val");
		iReturn = CXMLParameter::GetWertAusXML_Str(cPfad, &sParity, 4, "Thuema", "Sensoren", "Encoder", "Parity");
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iData, 4, "Thuema", "Sensoren", "Encoder", "Data");
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iStop, 4, "Thuema", "Sensoren", "Encoder", "Stop");
		return iReturn;
	}
	catch (exception)
	{
		return -1;
	}

}

//Ermittelt die Werte aus der XML für die Laserscanner
int CDatenrecorder::getScannerDataFromXML(char* cPfad, std::vector<ScannerInitParameter>& vScannerConfigData)
{
	try
	{
		int iReturn = -1;
		int iScannerAnzahl = 0;
		std::string sScannerKnoten;
		ScannerInitParameter tmp;
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iScannerAnzahl, 4, "Thuema", "Sensoren", "Scanner", "Anzahl");
		if (iReturn == 0)
		{
			for (int i = 0; i < iScannerAnzahl; i++)
			{
				sScannerKnoten = "Scanner_" + std::to_string(i);
				iReturn = CXMLParameter::GetWertAusXML_Str(cPfad, &tmp.ccDLLPfad, 5, "Thuema", "Sensoren", "Scanner", sScannerKnoten.c_str(), "DLL_Name");
				iReturn = CXMLParameter::GetWertAusXML_Str(cPfad, &tmp.sIP, 5, "Thuema", "Sensoren", "Scanner", sScannerKnoten.c_str(), "IP");
				iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &tmp.iPort, 5, "Thuema", "Sensoren", "Scanner", sScannerKnoten.c_str(), "Port");
				iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &tmp.iScannerID, 5, "Thuema", "Sensoren", "Scanner", sScannerKnoten.c_str(), "ScannerID");
				tmp.iAnzahlScanner = iScannerAnzahl;
				vScannerConfigData.push_back(tmp);
				tmp.ccDLLPfad.clear();
				tmp.sIP.clear();
			}
		}

		return iReturn;
	}
	catch (exception)
	{
		return -1;
	}
}

//Der jeweilige Scanner wird initialisiert und eine Scanner ID erstellt
int CDatenrecorder::initScanner(const char* ccPPfadScannerDLL, const char * ccIp, unsigned int uiPort, unsigned int& uiScannerID)
{
	int iReturn = -1;
	this->iAnzahlScanner++;
	CScanner* objScanner = new CScanner();
	iReturn = objScanner->initScanner(ccPPfadScannerDLL, ccIp, uiPort);
	if (0 != iReturn)
	{
		objScanner->exitScanner();
		this->bIsInitScanner = false;
		iReturn = -1;
		delete objScanner;
	}
	else
	{

		this->vobjScanner.push_back(objScanner);
		uiScannerID = static_cast<int>(this->vobjScanner.size() - 1);
		this->vlTmpScans.resize(this->vlTmpScans.size() + 1);
		this->vlTmpScans[uiScannerID] = new std::list<SLaserscan>();
		if (SCANNER_NOT_RUNNING != this->StartScannerThread(uiScannerID))
		{
			iReturn = SCANNER_ERROR;
			this->vobjScanner.at(uiScannerID)->setState(SCANNER_ERROR);
		}
	}
	return iReturn;
}

//Der Encoder wird initialisiert
int CDatenrecorder::initEncoder(const char* ccPPfadEncoderDLL, double dVel, char* cCOMPort, int iBaud, char cParity, int iData, int iStop)
{
	int iReturn = -1;
	this->dEncoderSpeed = dVel;
	this->ccCOMPORT = cCOMPort;
	this->objEncoder = new CEncoder();
	if (true == this->objEncoder->initEncoder(ccPPfadEncoderDLL, cCOMPort, iBaud, cParity, iData, iStop))
	{
		this->bIsInitEncoder = true;
		iReturn = 0;
		this->objEncoder->setSpeed(dVel);
		this->bIsInitEncoder = true;
		this->StartEncoderThread();
	}
	else
	{
		iReturn = -2;
		delete this->objEncoder;
	}
	return iReturn;
}

//Schließt die Verbindung mit dem Scanner
int CDatenrecorder::exitScanner()
{
	int iReturn = -1;
	for (int i = 0; i < this->iAnzahlScanner; i++)
	{
		iReturn = this->vobjScanner.at(i)->exitScanner();
		if (iReturn != 0)
		{
			iReturn = -1;
		}
	}
	Sleep(0);

	return iReturn;
}

//Thread für den Scanner mit der übergebenen ID wird gestartet
int CDatenrecorder::StartScannerThread(int iScannerID)
{
	int iVal = -1;
	int iReturn = 0;
	CThread* objThread;
	this->iAktScanID = iScannerID;
	objThread = new CThread();
	iVal = objThread->createThread(Thread_ScanRecord, (void*) this);
	Sleep(50);
	if ((0 == iVal) && (-1 == this->iAktScanID) && (true == objThread->checkThreadRun()))
	{
		this->vobjThread_Record.push_back(objThread);
	}
	else
	{
		this->vobjScanner.at(iScannerID)->setState(SCANNER_ERROR);
		iReturn = SCANNER_ERROR;
	}

	Sleep(50);


	if (0 != iVal)
	{
		OutputDebugStringA("ROTO_WINREC_THREADCREATE_FAIL");
		iReturn = -1;
	}

	if (0 != iReturn)
	{
		this->vobjThread_Record.at(iScannerID)->terminateThread();
		delete this->vobjThread_Record.at(iScannerID);

	}

	return iReturn;
}

//Der Thread für den Encoder wird gestartet
int CDatenrecorder::StartEncoderThread()
{
	int iVal = -1;
	int iReturn = 0;
	if (NULL != this->lEncoder.size())
	{
		this->lEncoder.clear();
	}

	objEncoderThread = new CThread();
	iVal = objEncoderThread->createThread(Thread_EncoderRecord, (void*) this);
	Sleep(50);
	if (false == objEncoderThread->checkThreadRun())
	{
		iReturn = ENCODER_ERROR;
		this->objEncoder->setState(ENCODER_ERROR);
	}

	Sleep(50);


	if (0 != iVal)
	{
		OutputDebugStringA("ENCODER_THREADCREATE_FAIL");
		iReturn = -1;
	}

	if (0 != iReturn)
	{
		this->objEncoderThread->terminateThread();
		delete this->objEncoderThread;
	}
	this->objEncoder->setState(ENCODER_NOT_RUNNING);
	return iReturn;
}

//DieAufnahme wird gestartet
void CDatenrecorder::StartScan()
{
	this->iScannerThreadState = SCANNER_ACTIVE;
	for (int i = 0; i < this->vobjScanner.size(); i++)
	{
		this->vobjScanner.at(i)->setState(SCANNER_ACTIVE);
	}

}

//Der Encoder wird gestartet
void CDatenrecorder::StartEncoder()
{
	if (true == this->bIsInitEncoder)
	{
		this->iEncoderThreadState = ENCODER_ACTIVE;
		this->objEncoder->setState(ENCODER_ACTIVE);
	}

}

//Ein Scan von allen Scannern wird wiedergegeben
int CDatenrecorder::SingleScan(std::vector<SLaserscan>* scanOut)
{
	SLaserscan tmp;
	std::vector<SLaserscan> vScans;
	int iReturn = -1;
	if (true == this->bIsInitScanner)
	{
		for (int i = 0; i < this->iAnzahlScanner; i++)
		{
			iReturn = this->vobjScanner.at(i)->getSingleScan(tmp.lScannerDaten, tmp.lScannerZeiten);
			vScans.push_back(tmp);
		}
		scanOut = &vScans;
	}
	return iReturn;
}

//Ein Ebenenscan wird für den jeweiligen Scanner erstellt
int CDatenrecorder::getActSingleScan(SLaserscan* scanOut, unsigned int uiScannerID)
{
	SLaserscan tmp;
	int iReturn = -1;
	if (true == this->bIsInitScanner)
	{
		for (int i = 0; i < this->iAnzahlScanner; i++)
		{
			iReturn = this->vobjScanner.at(uiScannerID)->getSingleScan(tmp.lScannerDaten, tmp.lScannerZeiten);
		}
		scanOut = &tmp;
	}
	return iReturn;
}

//Die Aufnahme wird gestartet
void CDatenrecorder::StartRecording()
{
	this->StartScan();
	this->StartEncoder();
}

//Die Aufnahme wird gestoppt
void CDatenrecorder::StopRecording()
{
	this->StopScan();
	this->StopEncoder();
}

//Die Aufnahme wird gestoppt
int CDatenrecorder::StopScan()
{
	int iReturn = -1;
	try
	{
		this->iScannerThreadState = SCANNER_NOT_RUNNING;
		this->iEncoderThreadState = ENCODER_NOT_RUNNING;
		for (unsigned int ui = 0; ui < this->vobjScanner.size(); ui++)
		{
			this->vobjScanner.at(ui)->stopRecording();
			this->vobjScanner.at(ui)->setState(SCANNER_NOT_RUNNING);
		}

		this->objEncoder->stopEncoder();
		this->objEncoder->setSpeed(-this->dEncoderSpeed);
		this->objEncoder->startEncoder();
		return 0;
	}
	catch (exception)
	{
		return iReturn;
	}
}

int CDatenrecorder::StopEncoder()
{
	if (true == this->bIsInitEncoder)
	{
		if (true == this->objEncoder->stopEncoder())
		{
			this->objEncoder->setState(ENCODER_NOT_RUNNING);
			return 0;
		}
		else
		{
			this->objEncoder->setState(ENCODER_ERROR);
			return ENCODER_ERROR;
		}
	}
	else
	{
		this->objEncoder->setState(ENCODER_ERROR);
		return -1;
	}
}

//Thread für die Aufnahme
void CDatenrecorder::Thread_ScanRecord(CThread* pclThread)
{
	CDatenrecorder* objDatenrecorder;
	objDatenrecorder = (CDatenrecorder*)pclThread->m_pvThreadObject;
	SLaserscan tmpScan;
	int iVal = -1;
	int iScannerID = objDatenrecorder->iAktScanID;
	objDatenrecorder->iAktScanID = -1;
	CScanner* objScanner = objDatenrecorder->vobjScanner.at(iScannerID);
	int iCount = 0;
	Sleep(10);
	while (false == pclThread->checkThreadStop())
	{
		if (SCANNER_ACTIVE != objDatenrecorder->iScannerThreadState)
		{
			objDatenrecorder->bRec = false;
			Sleep(10);
			if (iCount == objDatenrecorder->iTimeOutCheckConnection)
			{
				//Check ob der Scanner Daten ausliefert bzw. verbunden ist
				iCount = 0;
				objScanner->getSingleScan(tmpScan.lScannerDaten, tmpScan.lScannerZeiten);
				//Wenn der Scanner Daten liefert, ist alles ok
				if (tmpScan.lScannerDaten.size() != 0)
				{
					tmpScan.lScannerDaten.clear();
					tmpScan.lScannerZeiten.clear();
				}
				//Wenn der Scanner keine Daten liefert, versuche den Scaner nue zu verbinden
				else
				{
					//Wenn keine neue Verbindung hergestellt werden konnte, dann gebe eine Fehlermeldung aus
					if (0 != objScanner->reconnectScanner())
					{

						std::string str = "FEHLER: Die Verbindung mit dem Scanner der ID: " + std::to_string(iScannerID) + " ist fehlgeschlagen.";
						OutputDebugStringA(str.c_str());
						objScanner->setState(SCANNER_ERROR);
					}
				}
			}
			else
			{
				iCount++;
			}
		}
		if (SCANNER_ACTIVE == objDatenrecorder->iScannerThreadState)
		{
			//Starte die Aufnahme 
			if (false == objDatenrecorder->bRec)
			{
				iVal = objDatenrecorder->vobjScanner.at(iScannerID)->startRecording();
			}
			objDatenrecorder->bRec = true;
			if (0 == iScannerID)
			{
				iVal = objScanner->getRecordedPoints(tmpScan.lScannerDaten, tmpScan.lScannerZeiten);
			}
			else
			{
				iVal = objScanner->getRecordedPoints(tmpScan.lScannerDaten, tmpScan.lScannerZeiten);
			}
			
			tmpScan.dZeitstempel_Datenempfangen = CTimeStamp::getTimeStamp();
			if (/*(ROTOSCAN_OK == iVal) && */(0 != tmpScan.lScannerDaten.size()))
			{
				EnterCriticalSection(objDatenrecorder->critListScanData);
				objDatenrecorder->vlTmpScans.at(iScannerID)->push_back(tmpScan);
				LeaveCriticalSection(objDatenrecorder->critListScanData);
				tmpScan.lScannerDaten.clear();
				tmpScan.lScannerZeiten.clear();
				objDatenrecorder->bIsDataToGet = true;
			}
		}
		Sleep(0);
	}

	objDatenrecorder->bIsDataToGet = false;
	iVal = objDatenrecorder->vobjScanner.at(iScannerID)->stopRecording();
	objScanner->setState(SCANNER_NOT_RUNNING);
	pclThread->exitThread();

}

//Thread für die Aufnahme
void CDatenrecorder::Thread_EncoderRecord(CThread* pclThread)
{
	CDatenrecorder* objDatenrecorder;
	objDatenrecorder = (CDatenrecorder*)pclThread->m_pvThreadObject;
	double aktPos;
	SEncoder tmpEncoder;
	int iCount = 0;
	while (false == pclThread->checkThreadStop())
		//while (ENCODER_ACTIVE != objDatenrecorder->iEncoderThreadState)
	{
		if (ENCODER_ACTIVE != objDatenrecorder->iEncoderThreadState)
		{
			objDatenrecorder->bEncoderRec = false;
			Sleep(10);
			if (iCount == objDatenrecorder->iTimeOutCheckConnection)
			{
				//Check ob der Encodernoch verbunden ist
				iCount = 0;
				//Wenn der Encoder nicht verbunden ist, dann versuche eine neue Verbindung herzustellen
				if (false == objDatenrecorder->objEncoder->getPosition(aktPos))
				{
					//Wenn keine neue Verbindung hergestellt werden konnte, dann gebe eine Fehlermeldung aus
					if (0 != objDatenrecorder->objEncoder->reconnectEncoder())
					{
						objDatenrecorder->objEncoder->setState(ENCODER_ERROR);
						OutputDebugStringA("FEHLER: Verbindung mit der Encoder unterbrochen.");
					}
				}
			}
			else
			{
				iCount++;
			}
		}
		if (ENCODER_ACTIVE == objDatenrecorder->iEncoderThreadState)
		{
			if (false == objDatenrecorder->bEncoderRec)
			{
				objDatenrecorder->objEncoder->startEncoder();
			}
			objDatenrecorder->bEncoderRec = true;
			objDatenrecorder->objEncoder->getPosition(tmpEncoder.dPosition);
			tmpEncoder.dZeitstempel = CTimeStamp::getTimeStamp();
			objDatenrecorder->dAktPos = tmpEncoder.dPosition;
			EnterCriticalSection(objDatenrecorder->critListEncoderData);
			objDatenrecorder->lEncoder.push_back(tmpEncoder);
			LeaveCriticalSection(objDatenrecorder->critListEncoderData);
		}
		Sleep(0);
	}
	objDatenrecorder->objEncoder->setState(ENCODER_NOT_RUNNING);
	objDatenrecorder->iScannerThreadState = ENCODER_NOT_RUNNING;
	objDatenrecorder->objEncoder->stopEncoder();
	pclThread->exitThread();
}

//Abfrage für des aktuellen Scans von einem bestimmten Scanner 
int CDatenrecorder::getRecScanData(std::list<SLaserscan>* liSLDataScanner, unsigned int uiScannerID)
{
	int iReturn = -1;
	//Wenn Punkte vorhanden sind
	if (nullptr != this->vlTmpScans.at(uiScannerID))
	{
		if (0 < this->vlTmpScans.at(uiScannerID)->size())
		{
			EnterCriticalSection(this->critListScanData);
			*liSLDataScanner = *this->vlTmpScans.at(uiScannerID);
			LeaveCriticalSection(this->critListScanData);
			iReturn = 0;
		}
	}
	return iReturn;
}

//Gibt alle Scandaten von jedem Scanner wieder
int CDatenrecorder::getRecAllScanData(std::vector<std::list<SLaserscan>*>& vlAllData)
{
	try
	{
		if (0 != this->vlTmpScans.size())
		{
			vlAllData.resize(this->iAnzahlScanner);
			vlAllData = this->vlTmpScans;
		}
		return 0;
	}
	catch (exception)
	{
		return -1;
	}
}

//Gibt alle aufgenommenen Enoder Daten wieder
int CDatenrecorder::getRecEncoderData(std::list<SEncoder>& lAllDataEncoder)
{
	try
	{
		if (0 != this->lEncoder.size())
		{
			lAllDataEncoder = this->lEncoder;
			return 0;
		}
		else
		{
			return -2;
		}
	}
	catch (exception)
	{
		return -1;
	}
}

//Gibt den Status eines Scanners wieder
int CDatenrecorder::getScannerStatus(unsigned int uiScannerID)
{
	try
	{
		return this->vobjScanner.at(uiScannerID)->getState();
	}
	catch (exception)
	{
		return SCANNER_ERROR;
	}
}

//Gibt den Status des Encoers wieder
int CDatenrecorder::getEncoderStatus()
{
	try
	{
		return this->objEncoder->getState();
	}
	catch (exception)
	{
		return ENCODER_ERROR;
	}

}

//Gibt die aktuelle Position des Encoders wieder
int CDatenrecorder::getActEncorderPos(double& dActPos)
{
	try
	{
		dActPos = this->dAktPos;
		return 0;
	}
	catch (exception)
	{
		return -1;
	}
}
