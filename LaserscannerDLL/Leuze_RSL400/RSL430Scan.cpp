#include "RSL430Scan.h"
#include "TimeStamp.h"
#include "math_na.h"
#include <fstream>

#define RSL430_BUFFER_SIZE 2048000

/***********************************************************/
/* Konstruktor                                             */
/* Initialisierung WinSock                                 */
/* Initialisierung Critical Section                        */
/***********************************************************/
CRSL430Scan::CRSL430Scan( void ): iPufferSize(500),
						    dScanMinAngle(-135.1),
						    dScanMaxAngle(135.1),
						    dScanMinDist(0),
						    dScanMaxDist(65535),
						    dScanOffsetAngle(-135),
							dScannerHardMaxDist(65535),
							iScanTime(40),
							iValueSize(2700)
							
{
	//Initialisierung Netzwerkschnittstelle 
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	this->RSL430Socket = INVALID_SOCKET;

	//Initialisierung Thread
	this->objThread_Gatherer = new CThread();
	this->iThreatState = RECEIVE_THREAD_NOT_RUNNING;
	this->bScannerRun = false;

	//Initialisierung Semaphore
	this->critListScanData   = new CRITICAL_SECTION;
	InitializeCriticalSection ( this->critListScanData );

	//IniInitialisierung Korrektur_Vektor
	this->vecCorrValue.resize(iValueSize,0) ;
	OutputDebugStringA("RSL430_iPufferSize(5000)");
}

/***********************************************************/
/*Destruktor                                               */
/*Thread schließen/aufräumen                               */
/***********************************************************/
CRSL430Scan::~CRSL430Scan( void )
{
	if( true == this->objThread_Gatherer->checkThreadRun() )
	{
		this->objThread_Gatherer->terminateThread();
		this->listPufferScanData.clear();
	}

	if( INVALID_SOCKET != this->RSL430Socket )
	{
		closesocket(this->RSL430Socket);
	}
}

/************************************************************/
/* szHostName: IP Addresse Scanner							*/
/* szPort: Port vom Scanner									*/
/* Baut die Verbidnugn über Winsock zum Scanner auf			*/
/* und startet den Thread zur aufnahme der Messwerte		*/
/* Init Non Blocking UDP Socket auf dem übergbenden Port    */
/***********************************************************/
int CRSL430Scan::initRSL430UDP( const char* szHostName,  unsigned int uiPort )
{
	int iReturn = RSL430_RETURN_OK;
	
	//Wurde schon ein SOCKET erstellt?
	if( INVALID_SOCKET == this->RSL430Socket )
	{
		this->strHostName = szHostName;
		this->uiPortNr = uiPort;

		this->RSL430Addr.sin_family = AF_INET;
		this->RSL430Addr.sin_port = static_cast<u_short>(htons( this->uiPortNr ));
		this->RSL430Addr.sin_addr.s_addr = htonl(INADDR_ANY);

		sockaddr_in add;
        add.sin_family = AF_INET;
        add.sin_addr.s_addr = htonl(INADDR_ANY);
        add.sin_port = static_cast<u_short>(htons(this->uiPortNr));
		
		this->RSL430Socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
		if( INVALID_SOCKET == this->RSL430Socket )
		{
			OutputDebugStringA("RSL430_RETURN_INVALIDSOCKET");
			iReturn = RSL430_RETURN_INVALIDSOCKET;
		}
		else
		{
			int iVal = 0;
			static int siTimeout = 100;
			iVal = setsockopt( this->RSL430Socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&siTimeout, sizeof(siTimeout));
			if( SOCKET_ERROR == iVal )
			{
				OutputDebugStringA("RSL430_RETURN_OPTFAIL");
				iReturn = RSL430_RETURN_INVALIDSOCKET;
			}
			else
			{   		
				iVal = bind( this->RSL430Socket, reinterpret_cast<SOCKADDR *>(&add), sizeof(add) );
				if( SOCKET_ERROR == iVal )
				{
					OutputDebugStringA("RSL430_RETURN_BINDFAIL");
					iReturn = RSL430_RETURN_BINDFAIL;
				}
				else
				{
					iVal = this->objThread_Gatherer->createThread( thread_ReceiveGatherer, (void*) this );
					if( 0 != iVal )
					{
						OutputDebugStringA("RSL430_RETURN_CREATEFAIL");
						iReturn = RSL430_RETURN_ERROR;
					}
					//Scanner Korrektur lesen(corr'PortNr'.dat)
					char cDatTmp[64];
					sprintf_s(cDatTmp, "corr%i.dat", uiPort);
					iVal = this->loadCorrectValueDat(cDatTmp);
					if( RSL430_RETURN_OK != iVal)
					{
						OutputDebugStringA("RSL430_NO_CORRECTION_LOAD");
					}
				}
			}
		}
	}

	return iReturn;
}

/**********************************************************/
/* Schließt den Thread und löscht den Ringpuffer          */
/* Beendet den Socket und setzt ihn auf Invalid           */
/**********************************************************/
int CRSL430Scan::exitRSL430( void )
{

	if( true == this->objThread_Gatherer->checkThreadRun() )
	{
		this->objThread_Gatherer->terminateThread();
		this->listPufferScanData.clear();
	
	}
	closesocket( this->RSL430Socket );
	this->RSL430Socket = INVALID_SOCKET;
	return RSL430_RETURN_OK;
}

/***********************************************************/
/* Liest aus dem UDP Stream ein Scan herraus               */
/* kann nach einer bestimmten Anzahl von Versuchen         */
/* kein Paket zusammengestellt werden return mit fehler    */
/***********************************************************/
int CRSL430Scan::getSingleScan( std::list<CPunkt>& lcpPoints, std::list<double>& ldTimeStamps )
{
	OutputDebugStringA("RSL430_getSingleScan");

	int iReturn = RSL430_RETURN_OK;
	int iVal = 0;

	bool bScanReady = false;
	bool bScanComplete = false;
	int iTryCounter = 0;
	int iLastBlockNr = 0;
	int iDataSize = 0;
	
	sockaddr_in senderAddr;	
	int senderAddrSize = sizeof(senderAddr);
	
	
	char *cReceBuffer = new char[RSL430_BUFFER_SIZE];
	
	double dRecvTimeStamp = 0;
	RSL430ScanData stDat;
	stDat.iDataSollSize = -1;

	while( false == bScanComplete )
	{
		iVal = recvfrom( this->RSL430Socket,
						 cReceBuffer,
						 RSL430_BUFFER_SIZE-1,
						 0,
						 ( SOCKADDR* ) &senderAddr,
						 &senderAddrSize );
		dRecvTimeStamp = CTimeStamp::getTimeStamp( );

		if( 0 < iVal )
		{
			RSL430::pUDPTelegramType teleType = (RSL430::pUDPTelegramType) cReceBuffer;

			if( RSL430_ID_CONDES == teleType->ID )
			{
				OutputDebugStringA("RSL430_UDP_DATAHEAD");
				RSL430::pStateImageType stateImage = (RSL430::pStateImageType) &cReceBuffer[20];
				stDat.stMessBe =  stateImage->KontBesch;
				stDat.ulScanNr = teleType->ScanNr;
				stDat.iDataSollSize = this->getBeamCount( stDat.stMessBe.Start, 
														  stDat.stMessBe.Stop, 
														  stDat.stMessBe.Resolution );	
				stDat.dTimeRecv = dRecvTimeStamp;
				iLastBlockNr = -1;
				bScanReady = true;
			}
			else if( RSL430_ID_CONVAL == teleType->ID )
			{
				if( true == bScanReady )
				{
					if( teleType->BlockNr == (iLastBlockNr+1) )
					{
						iDataSize = ( teleType->H1.TotalSize - RSL430_HEADER_SIZE ) / 2;
						unsigned short *usaBeam = new unsigned short[iDataSize];
						usaBeam  = (unsigned short*) &cReceBuffer[20];

						stDat.Daten.insert( stDat.Daten.end( ), &usaBeam[0], &usaBeam[iDataSize] );
						stDat.iDataSollSize -= iDataSize;
						iLastBlockNr =  teleType->BlockNr;
					}
					else
					{
						OutputDebugStringA("RSL430_BAD_UDP_BLOCKNR");
						bScanReady = false;
						stDat.Daten.clear( );
						stDat.iDataSollSize = -1;
						iTryCounter++;
					}
				}
			}

			if( 0 ==  stDat.iDataSollSize )
			{
				OutputDebugStringA("RSL430_UDP_DATAF");
				bScanComplete = true;
			}
			else if( RSL430_SINGLESCAN_TRYS < iTryCounter )
			{
				OutputDebugStringA("RSL430_getSingleScan_tryCounter");
				bScanComplete = true;
				iReturn = RSL430_RETURN_BADPACKET;
			}
		}
		else
		{
			iReturn = RSL430_RETURN_ERROR;
			bScanComplete = true;
		}


		Sleep(0);
	}

	if( RSL430_RETURN_OK == iReturn )
	{
		iVal = convertScanDataInKartesisch( stDat, lcpPoints, ldTimeStamps );

		if( iVal == 0 )
		{
			iReturn = RSL430_RETURN_BADPACKET;
		}
	}


	return iReturn;
}

/***********************************************************/
/*Holt eine Messung aus dem Ringpuffer                     */
/*Konvertiert diese in CPunkt                              */
/***********************************************************/
int CRSL430Scan::getRecordedScanPoints( std::list<CPunkt>& lcpRecPoints, std::list<double>& ldRecTimeStamps )
{
	int iReturn = RSL430_RETURN_OK;

	if( true == this->listPufferScanData.empty() )
	{
		OutputDebugStringA("RSL430_RING_EMPTY");
		iReturn = RSL430_RETURN_ERROR;
	}
	else
	{
		int iVal = 0;
		RSL430ScanData tmpScanData;

		EnterCriticalSection(this->critListScanData);
		tmpScanData = this->listPufferScanData.back();
		this->listPufferScanData.pop_back();
		LeaveCriticalSection(this->critListScanData);

		iVal = convertScanDataInKartesisch( tmpScanData, 
											lcpRecPoints,
											ldRecTimeStamps );

		if( 0 > iVal )
		{
			iReturn = RSL430_RETURN_NOPOINTS;
		}
	}

	return iReturn;
}

/***********************************************************/
/* Ändert den Thread State								   */
/* Zum Start der Konti. Aufzeichnung der Messwerte         */
/***********************************************************/
int CRSL430Scan::startScanRecording( void )
{
	int iReturn = RSL430_RETURN_OK;
	int iTimeOutCounter = RSL430_TIMEOUT;

	this->iThreatState = RECEIVE_THREAD_ACTIVE;
	if( true == this->bScannerRun )
	{
		iReturn =  RSL430_RETURN_OK;
	}
	else
	{
		while( true != this->bScannerRun )
		{
			if(  0 == iTimeOutCounter )
			{
				iReturn = RSL430_RETURN_STARTFAIL;
				break;
			}

			Sleep(10);
			iTimeOutCounter--;
		}
	}
	Sleep(100);
	return iReturn;

}
/***********************************************************/
/* Ändert den Thread State								   */
/* Zum Stoppen der Konti. Aufzeichnung der Messwerte       */
/***********************************************************/
int CRSL430Scan::stopScanRecording( void )
{
	int iTimeOutCounter = 500;
	int iReturn = RSL430_RETURN_OK;

	this->iThreatState = RECEIVE_THREAD_NOT_RUNNING;

	if( false == this->bScannerRun )
	{
		iReturn =  RSL430_RETURN_OK;
	}
	else
	{
		while( false != this->bScannerRun )
		{
			if(  0 == iTimeOutCounter )
			{
				iReturn = RSL430_RETURN_STOPFAIL;
				break;
			}

			Sleep(10);
			iTimeOutCounter--;
		}
	}
	return iReturn;
}

/***********************************************************/
/*Thread zum befühllen des Ringpuffers                     */
/*Speichert Daten in den Ringpuffer bei Konti. Messung     */
/***********************************************************/
void CRSL430Scan::thread_ReceiveGatherer( CThread *pclThread )
{
	CRSL430Scan* objRSL430Scan;
	objRSL430Scan = ( CRSL430Scan* ) pclThread->m_pvThreadObject;
	

	char *cReceBuffer = new char[RSL430_BUFFER_SIZE];
	
	std::vector<char*> vecPacket;
	
	RSL430ScanData stDat;
	stDat.iDataSollSize = -1;

	int iDataSize = 0;
	int iVal = 0;
	std::string stPuffer;
	std::string sTelegram;
	std::vector<std::string> vecDaten;
	sockaddr_in senderAddr;
	int iLastBlockNr = 0;
	int senderAddrSize = sizeof(senderAddr);

	bool bScanReady = false;
	double dRecvTimeStamp = 0;


	while( false == pclThread->checkThreadStop( ) )
	{
		if( ( RECEIVE_THREAD_IDLE == objRSL430Scan->iThreatState ) ||
			( RECEIVE_THREAD_ACTIVE == objRSL430Scan->iThreatState ) )
		{
			objRSL430Scan->bScannerRun = true;

			iVal = recvfrom( objRSL430Scan->RSL430Socket,
							 cReceBuffer,
							 RSL430_BUFFER_SIZE-1,
							 0,
							 ( SOCKADDR* ) &senderAddr,
							 &senderAddrSize );

			dRecvTimeStamp = CTimeStamp::getTimeStamp( );
		
			if( 0 < iVal )
			{
				RSL430::pUDPTelegramType teleType = (RSL430::pUDPTelegramType) cReceBuffer;
				
				if( RSL430_ID_CONDES == teleType->ID )
				{
					RSL430::pStateImageType stateImage = (RSL430::pStateImageType) &cReceBuffer[20];
					stDat.stMessBe =  stateImage->KontBesch;
					stDat.ulScanNr = teleType->ScanNr;
					stDat.iDataSollSize = objRSL430Scan->getBeamCount( stDat.stMessBe.Start, 
																	   stDat.stMessBe.Stop, 
																	   stDat.stMessBe.Resolution );	

					stDat.dTimeRecv =  dRecvTimeStamp;
					bScanReady = true;
					iLastBlockNr = -1;
				}
				else if( RSL430_ID_CONVAL == teleType->ID )
				{
					if( true == bScanReady )
					{
						if( teleType->BlockNr == (iLastBlockNr+1) )
						{
							iDataSize = ( teleType->H1.TotalSize - RSL430_HEADER_SIZE ) / 2;
							unsigned short *usaBeam = new unsigned short[iDataSize];
							usaBeam  = (unsigned short*) &cReceBuffer[20];

							stDat.Daten.insert( stDat.Daten.end( ), &usaBeam[0], &usaBeam[iDataSize] );
							stDat.iDataSollSize -= iDataSize;
							iLastBlockNr =  teleType->BlockNr;
						}
						else
						{
							OutputDebugStringA("RSL430_BAD_UDP_BLOCKNR");
							bScanReady = false;
							stDat.Daten.clear( );
							stDat.iDataSollSize = -1;
						}
					}
				}

				
				if( 0 ==  stDat.iDataSollSize )
				{
					if( objRSL430Scan->iPufferSize < objRSL430Scan->listPufferScanData.size( ) )
					{
						OutputDebugStringA("RSL430_RING_FULL");

						EnterCriticalSection( objRSL430Scan->critListScanData );
						objRSL430Scan->listPufferScanData.pop_front( );
						objRSL430Scan->listPufferScanData.push_back( stDat );
						LeaveCriticalSection( objRSL430Scan->critListScanData );
					}
					else
					{
						EnterCriticalSection( objRSL430Scan->critListScanData );
						objRSL430Scan->listPufferScanData.push_back( stDat );
						LeaveCriticalSection( objRSL430Scan->critListScanData );
					}
					stDat.Daten.clear( );
					bScanReady = false;
				}
			}
		}
		else
		{
			objRSL430Scan->bScannerRun = false;
			objRSL430Scan->listPufferScanData.clear( );
			stDat.Daten.clear( );
			dRecvTimeStamp = 0;
			stDat.iDataSollSize = -1;
			bScanReady = false;
		}

		Sleep(0);
	}

	objRSL430Scan->iThreatState = RECEIVE_THREAD_TERMINATED;
	pclThread->exitThread();
}

/***********************************************************/
/*                                                         */
/*                                                         */
/*Kopiert den übergebenen                                                         */
/***********************************************************/
int CRSL430Scan::loadCorrectValue(std::vector<double> vecCorValue)
{
	try
	{
		if(vecCorValue.size() == iValueSize)
		{
			this->vecCorrValue = vecCorValue;
			return 0;
		}else
		{
			return -1;
		}
	}
	catch(exception)
	{
		return -1;
	}
}

/***********************************************************/
/*Konvertiert die Scanner Telegramme in CPunkt             */
/***********************************************************/
int CRSL430Scan::convertScanDataInKartesisch( RSL430ScanData strScanIn,
											   std::list<CPunkt>& lOut, 
											   std::list<double>& lTimeOut )
{
	OutputDebugStringA("RSL430_convertScanDataInKartesisch");

	if( 0 == strScanIn.Daten.size( ) )
	{
		return 0;
	}
	
	double dAktAngle = (strScanIn.stMessBe.Start * 0.1) + dScanOffsetAngle;
	double dStep = 0.1 * strScanIn.stMessBe.Resolution;
	CPunkt cpTmp = {0,0,0};
	double usTmpDist = 0;
	for( unsigned int i = 0; i < strScanIn.Daten.size( ); i++ )
	{
		
		usTmpDist = strScanIn.Daten.at(i) - vecCorrValue[i];
		if( ( dAktAngle > this->dScanMinAngle ) && ( dAktAngle < this->dScanMaxAngle ) )
		{
			if( ( usTmpDist >= this->dScanMinDist ) && ( usTmpDist <= this->dScanMaxDist ) )
			{
				cpTmp.x = usTmpDist  * cos( dAktAngle * M_DEG2RAD );
				cpTmp.y = usTmpDist  * sin( dAktAngle * M_DEG2RAD );
				cpTmp.z = 0;
				cpTmp.dOrientierung = dAktAngle;
				cpTmp.dDistanz = usTmpDist;

				lOut.push_back(cpTmp);
				lTimeOut.push_back( strScanIn.dTimeRecv );
			}
		}
		dAktAngle += dStep;
	}

	return static_cast<int>(lOut.size());
}

/***********************************************************/
/*Berechnet die zu erwartenden Messwerte als Rückgabewert  */
/***********************************************************/
int CRSL430Scan::getBeamCount( unsigned short usStart, 
							   unsigned short usStop, 
							   unsigned short usReso )
{
	return 1 + (int)ceil( ( usStop - usStart) / (double)usReso );
}

/***********************************************************/
/*Lädt Korrekturwerte für den Scanner aus der Datei        */
/*und kopiert diese in den vorgesehenden Vector            */
/***********************************************************/
int CRSL430Scan::loadCorrectValueDat(const char* szDatName)
{
	fstream fDat;
	int iReturn = RSL430_RETURN_NOPOINTS;

	fDat.open(szDatName, ios::in);
	if (true == fDat.is_open())
	{
		std::string s;
		double dTmp = 0;
		std::vector<double> vecCorrTmp;
		while (true == fDat.eof())
		{
			std::getline(fDat, s);
			sscanf_s(s.c_str(), "%lf", &dTmp);
			vecCorrTmp.push_back(dTmp);
		}
		this->loadCorrectValue(vecCorrTmp);
		iReturn = RSL430_RETURN_OK;
	}
	return iReturn;
}

/***********************************************************/
/*Gibt einen akt. Zeitstempel zurück		               */
/***********************************************************/
double CRSL430Scan::getTimeStamp( void )
{
	return CTimeStamp::getTimeStamp( );
}

/***********************************************************/
/*Gibt den Thread Status zurück                            */
/***********************************************************/
int CRSL430Scan::getRSL430Status( void )
{
	int iReturn = RECEIVE_THREAD_IDLE;
	if( true == this->bScannerRun )
	{
		iReturn = RECEIVE_THREAD_ACTIVE;
	}

	return iReturn;
}

/***********************************************************/
/* Set Methode für Öffnungswinkel Filter                   */
/***********************************************************/
void CRSL430Scan::setAngleFilterLimits(double dMinAngle, double dMaxAngle)
{
	this->dScanMaxAngle = dMaxAngle; //+this->dScanOffsetAngle;
	this->dScanMinAngle = dMinAngle;// +this->dScanOffsetAngle;
}

/**********************************************************/
/* Set Methode für Distanz Filter						  */
/**********************************************************/
void CRSL430Scan::setDistanceFilterLimits(double dMinDist, double dMaxDist)
{
	this->dScanMaxDist = dMaxDist;
	this->dScanMinDist = dMinDist;
}

