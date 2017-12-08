#include "SickScan.h"
#include "TimeStamp.h"
#include "math_na.h"


/***********************************************************/
/* Konstruktor                                             */
/* Initialisierung WinSock                                 */
/* Initialisierung Critical Section                        */
/***********************************************************/
CSickScan::CSickScan(): iPufferSize(200),
						dScanMinAngle(0),
						dScanMaxAngle(180),
						dScanMinDist(0),
						dScanMaxDist(100000),
						dScanOffsetAngle(90),
						iVerbose(0),
						iMinIntensity(1)
{
	WSADATA wsaData;
	WSAStartup( MAKEWORD( 2,2 ), &wsaData );

	this->objThread_Gatherer = new CThread();
	this->critListScanData   = new CRITICAL_SECTION;

	this->vecMeasurementQueue = new std::vector<MeasurementQueueElement_t>;

	this->iThreatState = RECEIVE_THREAD_NOT_RUNNING;
	this->bScannerRun = false;
	InitializeCriticalSection ( this->critListScanData );
}

/***********************************************************/
/*Destruktor                                               */
/*Thread schließen/aufräumen                               */
/***********************************************************/
CSickScan::~CSickScan()
{
	if( true == this->objThread_Gatherer->checkThreadRun( ) )
	{
		this->objThread_Gatherer->terminateThread();
		this->listPufferScanData.clear();
	}

	if( 0 < this->SickSocket )
	{
		closesocket(this->SickSocket);
	}
}

/************************************************************/
/* szHostName: IP Addresse Scanner							*/
/* szPort: Port vom Scanner									*/
/* Baut die Verbidnugn über Winsock zum Scanner auf			*/
/* und startet den Thread zur aufnahme der Messwerte		*/
/************************************************************/
int CSickScan::initSickScan(const char *szHostName, const char *szPort)
{
	int iReturn = 0;
	
	this->SickSocket = this->getConnection(szHostName, szPort);
	if (this->SickSocket == INVALID_SOCKET)
	{
		iReturn = -1;
	}
	
	if (0 == iReturn)
	{
		this->configSick(0.1f, 0, 55.0f, 70.0f);

		this->objThread_Gatherer->createThread(thread_ReceiveGatherer, (void*) this);
		this->iThreatState = RECEIVE_THREAD_NOT_RUNNING;

		int iTimeOutCounter = 100;
		while (true != this->objThread_Gatherer->checkThreadRun())
		{
			if (0 == iTimeOutCounter)
			{
				iReturn = SICK_STARTSCANNERFAIL;
				this->objThread_Gatherer->terminateThread();
				this->iThreatState = RECEIVE_THREAD_ERROR;
				break;
			}
			Sleep(10);
			iTimeOutCounter--;
		}
	}

	return iReturn;
}

/***********************************************************/
/*private Methode                                          */
/*Socket Verbindung zum Scaner                             */
/***********************************************************/
SOCKET CSickScan::getConnection(const char *szHostName, const char *szPort)
{
	int iResult = 0;

	struct addrinfo *strAdResult = NULL;
	
	struct addrinfo	hints;

	SOCKET returnSocket;
	returnSocket = INVALID_SOCKET;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo(szHostName, szPort, &hints, &strAdResult);
	if (iResult != 0)
	{
		WSACleanup();
		return returnSocket;
	}

	struct addrinfo	*strAdPtr = strAdResult;

	// Socket vom Server
	returnSocket = socket(strAdPtr->ai_family, strAdPtr->ai_socktype, strAdPtr->ai_protocol);
	if (returnSocket == INVALID_SOCKET)
	{
		freeaddrinfo(strAdResult);
		WSACleanup();
		return returnSocket;
	}

	// Connect to server.
	iResult = connect(returnSocket, strAdPtr->ai_addr, (int)strAdPtr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(returnSocket);
		returnSocket = INVALID_SOCKET;
	}

	freeaddrinfo(strAdResult);

	if (returnSocket == INVALID_SOCKET)
	{
		WSACleanup();
	}

	return returnSocket;
}

/***********************************************************/
/* Startet die kontinuierliche ausgabe der Messwerte vom   */
/* Scanner zum Host	                                       */
/***********************************************************/
int CSickScan::startKontiMessung()
{
	char cTmp[NetBuffer];
	char cCom[NetBuffer];
	unsigned char ucCheckSum = 0;
	int iVar = 0;
	int iLen = 0;
	int iReturn = -1;

	iLen = sprintf_s(cTmp, "sMN mLRreqdata %x", 0x20);
	cCom[0] = 0x02;
	cCom[1] = 0x02;
	cCom[2] = 0x02;
	cCom[3] = 0x02;

	cCom[4] = (iLen >> 24) & 0xff;
	cCom[5] = (iLen >> 16) & 0xff;
	cCom[6] = (iLen >> 8) & 0xff;
	cCom[7] = (iLen) & 0xff;

	for (int i = 0; i < iLen; i++)
	{
		cCom[i + 8] = cTmp[i];
		ucCheckSum ^= cTmp[i];
	}

	cCom[8 + iLen] = ucCheckSum;
	cCom[9 + iLen] = 0x00;

	iVar = this->sendToSickLMS(cCom, 9 + iLen);

	if (SICK_OK == iVar)
	{
		iReturn = 0;
		//Abfragen der Telegram Bestätigung
		if (-1 == receiveFromSickLMS(this->SickSocket))
		{
			iReturn = SICK_NETWORKRECVERROR;
		}
		//Abfragen der Telegram Antwort
		else if (-1 == receiveFromSickLMS(this->SickSocket))
		{
			iReturn = SICK_NETWORKRECVERROR;
		}
	}

	return iReturn;
}

/***********************************************************/
/* Stoppt die kontinuierliche ausgabe der Messwerte vom    */
/* Scanner zum Host	                                       */
/***********************************************************/
int CSickScan::stopKontiMessung()
{
	char cTmp[NetBuffer * 2];
	char cCom[NetBuffer * 2];
	unsigned char ucChecksum = 0;
	int iVar = 0;
	int iLen = 0;
	int iReturn = -1;

	iLen = sprintf_s(cTmp, "sMN mLRstopdata");

	cCom[0] = 0x02;
	cCom[1] = 0x02;
	cCom[2] = 0x02;
	cCom[3] = 0x02;

	cCom[4] = (iLen >> 24) & 0xff;
	cCom[5] = (iLen >> 16) & 0xff;
	cCom[6] = (iLen >> 8) & 0xff;
	cCom[7] = (iLen) & 0xff;

	for (int i = 0; i < iLen; i++)
	{
		cCom[i + 8] = cTmp[i];
		ucChecksum ^= cTmp[i];
	}

	cCom[8 + iLen] = ucChecksum;
	cCom[9 + iLen] = 0x00;

	iVar = send(this->SickSocket, cCom, 9 + iLen, 0);//
	if (iVar > 0)
	{
		iReturn = 0;
		//Abfrgaen der Telegram Bestätigung
		if (-1 == receiveFromSickLMS(this->SickSocket))
		{
			iReturn = SICK_NETWORKRECVERROR;
		}
		//Abfragen der Telegram Antwort
		else if (-1 == receiveFromSickLMS(this->SickSocket))
		{
			iReturn = SICK_NETWORKRECVERROR;
		}
	}

	return iReturn;
}

/***********************************************************/
/* Ändert den Thread State								   */
/* Zum Start der Konti. Aufzeichnung der Messwerte         */
/***********************************************************/
int CSickScan::startScanRecording()
{
	int iReturn = SICK_OK;

	this->iThreatState = RECEIVE_THREAD_ACTIVE;
	if (true == this->bScannerRun)
	{
		iReturn = SICK_OK;
	}
	else
	{
		int iTimeOutCounter = 500;
		while (true != this->bScannerRun)
		{
			if (0 == iTimeOutCounter)
			{
				iReturn = SICK_STARTSCANNERFAIL;
				break;
			}

			Sleep(10);
			iTimeOutCounter--;
		}
	}

	return iReturn;
}

/***********************************************************/
/* Ändert den Thread State								   */
/* Zum Stoppen der Konti. Aufzeichnung der Messwerte       */
/***********************************************************/
int CSickScan::stopScanRecording()
{
	int iReturn = SICK_OK;
	isScanStop = true;
	this->iThreatState = RECEIVE_THREAD_NOT_RUNNING;
	if (false == this->bScannerRun)
	{
		iReturn = SICK_OK;
	}
	else
	{
		int iTimeOutCounter = 500;
		while (false != this->bScannerRun)
		{
			if (0 == iTimeOutCounter)
			{
				iReturn = SICK_STOPSCANNERFAIL;
				break;
			}

			Sleep(0);
			iTimeOutCounter--;
		}
	}
	return iReturn;
}

/***********************************************************/
/*Thread zum befühllen des Ringpuffers                     */
/*Speichert Daten in den Ringpuffer bei Konti. Messung     */
/***********************************************************/
void CSickScan::thread_ReceiveGatherer(CThread *pclThread)
{
	CSickScan* objSickScan;
	objSickScan = (CSickScan*)pclThread->m_pvThreadObject;

	
	SickScanDataLMS400 ssdDaten;
	int iRet = 0;

	while (false == pclThread->checkThreadStop())
	{
		if ((RECEIVE_THREAD_IDLE == objSickScan->iThreatState) ||
			(RECEIVE_THREAD_ACTIVE == objSickScan->iThreatState))
		{
			if (false == objSickScan->bScannerRun)
			{
				iRet = objSickScan->startKontiMessung();
				objSickScan->iVerbose = 2;

				if (0 == iRet)
				{
					objSickScan->bScannerRun = true;
				}
				else
				{
					objSickScan->bScannerRun = false;
					objSickScan->iThreatState = RECEIVE_THREAD_ERROR;
					continue;//Thread schlafen legen
				}
				Sleep(10);
			}

			ssdDaten = objSickScan->readMeasurement(objSickScan->SickSocket );
			if (0 < ssdDaten.vecRanges.size() )
			{
				if (RECEIVE_THREAD_ACTIVE == objSickScan->iThreatState)
				{
					if (objSickScan->listPufferScanData.size() > objSickScan->iPufferSize)
					{
						EnterCriticalSection(objSickScan->critListScanData);
						objSickScan->listPufferScanData.pop_front();
						LeaveCriticalSection(objSickScan->critListScanData);
					}

					EnterCriticalSection(objSickScan->critListScanData);
					objSickScan->listPufferScanData.push_back(ssdDaten);
					LeaveCriticalSection(objSickScan->critListScanData);
				}

			}
			else
			{
				objSickScan->iThreatState = RECEIVE_THREAD_ERROR;
			}
		}
		else
		{
			if (true == objSickScan->bScannerRun)
			{
				OutputDebugStringA("ROTO_THREAD_FAIL");
				iRet = objSickScan->stopKontiMessung();
				if (0 == iRet)
				{
					objSickScan->bScannerRun = false;
				}
			}
		}
		Sleep(1);
	};

	objSickScan->iThreatState = RECEIVE_THREAD_TERMINATED;
	pclThread->exitThread();
}

/***********************************************************/
/* Liest Messungen vom Scanner vom Socket                  */
/***********************************************************/
SickScanDataLMS400 CSickScan::readMeasurement( SOCKET s )
{
	SickScanDataLMS400 ssdScan;
	char cBuf[NetBuffer];
  

	if ( false == this->vecMeasurementQueue->empty())
	{
		memset(cBuf, 0, 256);
		memcpy_s(	cBuf,
					NetBuffer,
					this->vecMeasurementQueue->front().string,
					this->vecMeasurementQueue->front().length + 1 );
	}
	else
	{
		int iLen = 0;
		if (2 == this->iVerbose)
		{
			iLen = recv(s, cBuf, 8, 0);
			if (0 > iLen)
			{
				if (0 < this->iVerbose)
				{
					return ssdScan;
				}
			}
			else if ((0x02 == cBuf[0]) ||
					 (0x02 == cBuf[1]) ||
					 (0x02 == cBuf[2]) ||
					 (0x02 == cBuf[3]))
			{
				int iTelLen = 0;
				int iAktRead = 0;
				// find message length
				iTelLen = ((cBuf[4] << 24) | (cBuf[5] << 16) | (cBuf[6] << 8) | (cBuf[7]));
				
				//Read Data Message
				do
				{
					iLen = recv(s, &cBuf[iAktRead], iTelLen - iAktRead, 0);
					iAktRead += iLen;
				} while (iAktRead < iTelLen);

				//read Checksum
				char cCheckSum = 0;
				iLen = recv(s, &cCheckSum, 1, 0);

				char cCalcSum = 0;
				for (int i = 0; i < iTelLen; i++)
				{
					cCalcSum ^= cBuf[i];
				}

				if (cCheckSum != cCalcSum)
				{
						return ssdScan;
				}
			}			
		}
	}

	ssdScan.dStamp = CTimeStamp::getTimeStamp();

	// parse measurements header and fill in the configuration parameters
	MeasurementHeader_t meas_header;
	memcpy_s(	&meas_header, 
				sizeof(MeasurementHeader_t), 
				(void *)cBuf, 
				sizeof(MeasurementHeader_t) );

	ssdScan.fAngleMin = static_cast<float>(meas_header.iStartAngle / 10000.0);
	ssdScan.fAngleReso = static_cast<float>(meas_header.usAngularStepWidth / 10000.0);
	ssdScan.fAngleMax = static_cast<float>((meas_header.usMeasSize * ssdScan.fAngleReso) + ssdScan.fAngleMin);
	ssdScan.fAngleInc = static_cast<float>(meas_header.usAngularStepWidth / 10000.0);
	ssdScan.fRangeMin = 0.7f;
	ssdScan.fRangeMax = 3.6f;

	ssdScan.vecRanges.resize(meas_header.usMeasSize);
	ssdScan.vecIntensities.resize(meas_header.usMeasSize);
	ssdScan.siStartAngle = meas_header.iStartAngle / 10000;

	unsigned short usDist;
	unsigned char ucRem;
	int iIndex = sizeof(MeasurementHeader_t);
	// Parse the read buffer and copy values into our distance/intensity buffer
	for (int i = 0; i < meas_header.usMeasSize; i++)
	{
		if( ( 0x20 == meas_header.usFormat) || (0x21 == meas_header.usFormat) )
		{
			memcpy_s(&usDist, sizeof(unsigned short), &cBuf[iIndex], sizeof(unsigned short));
			iIndex += sizeof(unsigned short);
		}
		if ((0x20 == meas_header.usFormat) || (0x22== meas_header.usFormat))
		{
			memcpy_s(&ucRem, sizeof(unsigned char), &cBuf[iIndex], sizeof(unsigned char));
			iIndex += sizeof(unsigned char);
		}

		ssdScan.vecRanges[i] = static_cast<float>(usDist*meas_header.usDistScal);
		ssdScan.vecIntensities[i] = static_cast<float>(ucRem*meas_header.usRemScal);
	}

	return (ssdScan);
}

/***********************************************************/
/*Konvertiert die Scanner Telegramme in SPunkt             */
/***********************************************************/
int CSickScan::convertDataFromLMS400(SickScanDataLMS400 lms,std::list<SPunkt> &lScanOut,std::list<double> &ldTimeStamps, int iMittelwertAnzahl = 1)
{
	SPunkt cpTmp;
	double dAktAngle = static_cast<double>(lms.siStartAngle);
	float usTmpDist = 0;
	std::list<SPunkt> lMittelwertPunkte;
	std::list<double> lMittelwertZeit;
	double x = 0;
	double y = 0;
	double z = 0;
	double dDistance = 0;
	double angle = 0;
	for (unsigned int i = 0; i < lms.vecRanges.size(); i++)//Anzahl der Messwerte eines Scans
	{
		usTmpDist = lms.vecRanges.at(i);
		cpTmp.x = 0;
		cpTmp.y = 0;
		cpTmp.z = 0;
		cpTmp.dOrientierung = dAktAngle;
		cpTmp.dDistanz = usTmpDist;
		if (this->iMinIntensity < lms.vecIntensities.at(i))
		{
			if ((dAktAngle > this->dScanMinAngle) && (dAktAngle < this->dScanMaxAngle))
			{
				if ((usTmpDist >= this->dScanMinDist) && (usTmpDist <= this->dScanMaxDist))
				{
					cpTmp.y = usTmpDist * sin(dAktAngle * M_DEG2RAD);//Y-Koordinate wird konvertiert
					cpTmp.x = usTmpDist * cos(dAktAngle * M_DEG2RAD);//X-Koordinate wird konvertiert
					cpTmp.z = 0;
				}
			}
		}
		lMittelwertPunkte.push_back(cpTmp);
		lMittelwertZeit.push_back(lms.dStamp);
		dAktAngle += (lms.fAngleInc);
		if (lMittelwertPunkte.size() == iMittelwertAnzahl)
		{
			for (std::list<SPunkt>::iterator myPoints = lMittelwertPunkte.begin(); myPoints != lMittelwertPunkte.end(); ++myPoints)
			{
				x = x + myPoints->x;
				y = y + myPoints->y;
				z = z + myPoints->z;
				angle = angle + myPoints->dOrientierung;
				dDistance = dDistance + myPoints->dDistanz;
			}
			cpTmp.dDistanz = dDistance / iMittelwertAnzahl;
			cpTmp.dOrientierung = angle / iMittelwertAnzahl;
			cpTmp.x = x / iMittelwertAnzahl;
			cpTmp.y = y / iMittelwertAnzahl;
			cpTmp.z = z / iMittelwertAnzahl;
			lScanOut.push_back(cpTmp);
			ldTimeStamps.push_back(lMittelwertZeit.front());
			lMittelwertPunkte.clear();
			lMittelwertZeit.clear();
			x = 0;
			y = 0;
			z = 0;
			angle = 0;
			dDistance = 0;
		}
		
		
	}
	return static_cast<int>(lScanOut.size());
}

/***********************************************************/
/*Empfängt eine Messung vom Scanner	       			       */
/*Konvertiert diese in SPunkt                              */
/***********************************************************/
int CSickScan::getSingleScan( std::list<SPunkt> &lcpPoints, 
							  std::list<double> &ldTimeStamps )
{
	int iVar = 0;

	iVar = this->startKontiMessung( );
	
	if (0 == iVar)
	{
		this->iVerbose = 2;
		SickScanDataLMS400 ssdScanData;
		ssdScanData = readMeasurement(this->SickSocket);
		this->stopKontiMessung();
		convertDataFromLMS400(ssdScanData, lcpPoints, ldTimeStamps);
	}
	else
	{
		return -1;
	}
	
	return 0;
}

/***********************************************************/
/*Konfiguriert den Scanner                                 */
/*fAngleReso: Winkelauflösung in Grad                      */
/*iGScanFreq: GrobauswahlScanFrequenz                      */
/*00h(schnell) 01h(mittel) 02h(langsam)					   */	
/*fAngleStart: Start Messbereich in Grad				   */
/*fAngleLen: Laenge Messbereich in Grad					   */	
/***********************************************************/
int CSickScan::configSick(	float fAngleReso,
							int iGScanFreq,
							float fAngleStart,
							float fAngleLen)
{
	int iLen = 0;
	int iVar = 0;
	int iReturn = -1;
	char cTmp[NetBuffer];
	char cCom[NetBuffer];
	unsigned char ucChecksum = 0;

	
	iLen = sprintf_s(	cTmp,
						"sMN mSCconfigbyang 03 F4724744 +%0.1f +%i +%f +%f",
						fAngleReso,
						iGScanFreq,
						fAngleStart,
						fAngleLen );
	
	cCom[0] = 0x02;
	cCom[1] = 0x02;
	cCom[2] = 0x02;
	cCom[3] = 0x02;

	cCom[4] = (iLen >> 24) & 0xff;
	cCom[5] = (iLen >> 16) & 0xff;
	cCom[6] = (iLen >> 8) & 0xff;
	cCom[7] = (iLen ) & 0xff;

	for( int i = 0; i < iLen; i++ )
	{
		cCom[i+8] = cTmp[i];
		ucChecksum ^= cTmp[i];
	}

	cCom[8 + iLen] = ucChecksum;
	cCom[9 + iLen] = 0x00;

	iVar = this->sendToSickLMS(cCom, 9 + iLen);

	if (SICK_OK == iVar)
	{
		iReturn = 0;
		//Abfragen der Telegram Bestätigung
		if (-1 == receiveFromSickLMS(this->SickSocket))
		{
			iReturn = ERR_LASCANLIB_TELEGRAM_CRC_ERROR;
		}
		//Abfragen der Telegram Antwort
		else if (-1 == receiveFromSickLMS(this->SickSocket))
		{
			iReturn = ERR_LASCANLIB_TELEGRAM_CRC_ERROR;
		}
	}

	return iReturn;
}

/***********************************************************/
/*Holt eine Messung aus dem Ringpuffer                     */
/*Konvertiert diese in SPunkt                              */
/***********************************************************/
int CSickScan::getRecordedScanPoints( std::list <SPunkt>& rlstclPoints, 
									  std::list <double>& rlstdTimeStamps, 
									  bool bInterpoleateTimeStamps )
{
	if( true == this->listPufferScanData.empty() )
	{
		return SICK_NORECPOINTS;
	}
	else
	{
		int iReturn = SICK_OK;
		SickScanDataLMS400 tmpScanData;
	
		EnterCriticalSection(this->critListScanData);
		tmpScanData = this->listPufferScanData.back();
		this->listPufferScanData.pop_back();
		LeaveCriticalSection(this->critListScanData);

		if (0 > this->convertDataFromLMS400(tmpScanData, rlstclPoints, rlstdTimeStamps))
		{
			iReturn = SICK_ALLGERROR;
		}
		return iReturn;
	}

	
}

int CSickScan::getFilterRecordPoints(std::list<SPunkt>& rlstclPoints, std::list<double>& rlstdTimeStamps, int iAnzahl)
{
	/*if (true == this->listPufferScanData.empty())
	{
		return SICK_NORECPOINTS;
	}
	else
	{*/
	while (this->listPufferScanData.size() == 0)
	{
		Sleep(10);
	}
	int iReturn = SICK_OK;
	SickScanDataLMS400 tmpScanData;

	EnterCriticalSection(this->critListScanData);
	tmpScanData = this->listPufferScanData.back();
	this->listPufferScanData.pop_back();
	LeaveCriticalSection(this->critListScanData);
		
	if (0 > this->convertDataFromLMS400(tmpScanData, rlstclPoints, rlstdTimeStamps))
	{
		iReturn = SICK_ALLGERROR;
	}
	return iReturn;
	
}


/***********************************************************/
/*                                                         */
/*                                                         */
/*                                                         */
/***********************************************************/
unsigned int CSickScan::readMistake()
{
	unsigned int iReturn = 0;
	//TODO
	return iReturn;
}

/***********************************************************/
/*                                                         */
/*                                                         */
/*                                                         */
/***********************************************************/
unsigned int CSickScan::readBigMistake()
{
	unsigned int iReturn = 0;
	//TODO
	return iReturn;
}

/***********************************************************/
/* Set Methode für Öffnungswinkel Filter                   */
/***********************************************************/
void CSickScan::setAngleFilterLimits(double dMinAngle, double dMaxAngle)
{
	this->dScanMaxAngle =  dMaxAngle + this->dScanOffsetAngle;
	this->dScanMinAngle =  dMinAngle + this->dScanOffsetAngle;
}

/**********************************************************/
/* Set Methode für Distanz Filter						  */
/**********************************************************/
void CSickScan::setDistanceFilterLimits(double dMinDist, double dMaxDist )
{
	this->dScanMaxDist = dMaxDist;
	this->dScanMinDist = dMinDist;
}

/**********************************************************/
/* Schließt den Thread und löscht den Ringpuffer          */
/**********************************************************/
int CSickScan::exitSickScan()
{

	if( true == this->objThread_Gatherer->checkThreadRun() )
	{
		this->objThread_Gatherer->terminateThread();
		this->listPufferScanData.clear();
	}
	return 0;
}

/***********************************************************/
/*Gibt den Thread Status zurück                            */
/***********************************************************/
int CSickScan::getStatus()
{
	return this->iThreatState;
}

/***********************************************************/
/*Gibt einen akt. Zeitstempel zurück		               */
/***********************************************************/
double CSickScan::getTimeStamp( void )
{
	return CTimeStamp::getTimeStamp( );
}

/***********************************************************/
/*Sendet das Telegram über Socket an den Scanner           */
/***********************************************************/
int CSickScan::sendToSickLMS(char *s, int len )
{
	int iReturn = SICK_OK;

	if (0 > send(this->SickSocket, s, 9 + len, 0))//Anfrage )
	{
		iReturn = SICK_NETWORKSENDERROR;
	}

	return iReturn;
}

/***********************************************************/
/*Liest die Antwort Telegramm des Scanners vom Socket      */
/***********************************************************/
int CSickScan::receiveFromSickLMS(SOCKET s )
{
	int iLen = 0;
	char cBuf[NetBuffer];

	memset(cBuf, 0, 256);

	iLen = recv(s, cBuf, 8, 0);
	if (0 > iLen)
	{
		return (-1);
	}
	else if ((0x02 == cBuf[0]) ||
		(0x02 == cBuf[1]) ||
		(0x02 == cBuf[2]) ||
		(0x02 == cBuf[3]))
	{
		int iTelLen = 0;
		int iAktRead = 0;
		// find message length
		iTelLen = ((cBuf[4] << 24) | (cBuf[5] << 16) | (cBuf[6] << 8) | (cBuf[7]));

		//Read Data Message
		do
		{
			iLen = recv(s, &cBuf[iAktRead], iTelLen - iAktRead, 0);
			iAktRead += iLen;
		} while (iAktRead < iTelLen);

		//read Checksum
		char cCheckSum = 0;
		iLen = recv(s, &cCheckSum, 1, 0);

		char cCalcSum = 0;
		for (int i = 0; i < iTelLen; i++)
		{
			cCalcSum ^= cBuf[i];
		}

		if (cCheckSum != cCalcSum)
		{
			return -1;
		}
		else if (strncmp((const char*)cBuf, "sFA", 3) == 0)
		{
			return -1;
		}
		else if ('s' == cBuf[0])
		{
			return 0;
		}
		else if (0x20 == cBuf[0])
		{
			return this->receiveFromSickLMS(s);
		}
		else if (sizeof(MeasurementHeader_t) < iTelLen)
		{
			if (0 < this->iVerbose)
			{
				MeasurementQueueElement_t q;
				q.string = (unsigned char*)malloc(iTelLen + 1);
				memcpy_s(q.string, iTelLen + 1, cBuf, iTelLen + 1);
				q.length = iTelLen;
				this->vecMeasurementQueue->push_back(q);
				return this->receiveFromSickLMS(s);
			}
		}
	}

	return 0;

}
