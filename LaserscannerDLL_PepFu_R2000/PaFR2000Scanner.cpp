#include "PaFR2000Scanner.h"
#include "DebugOutput.h"

// +++++++++++++++++++++++++++++  Public Methoden  ++++++++++++++++++++++++++++

//++++++++ Konstruktor  ++++++++
PaFR2000Scanner::PaFR2000Scanner(void)
{

	m_csStringBuffer = new CRITICAL_SECTION;
	InitializeCriticalSection(m_csStringBuffer);

	m_csObjectBuffer = new CRITICAL_SECTION;
	InitializeCriticalSection(m_csObjectBuffer);

	m_csTimeBuffer = new CRITICAL_SECTION;
	InitializeCriticalSection(m_csTimeBuffer);

	m_objThread_buffer = new CThread();
	m_objThread_convert = new CThread();

	m_objDebugOutput = new CDebugOutput();
	m_objDebugOutput->initDebugOutput("PaFR2000Scanner");

	this->m_objClientTcp = new Client;

	this->m_strIp = "127.0.0.1";
	this->m_usPort = 80;

	this->m_dMaxAngle = 180;
	this->m_dMinAngle = -180;
	this->m_dMaxDist = 50000;
	this->m_dMinDist = 0;

	m_bInit = false;
	m_bThreadRecv = false;
	m_bThreadScanData = false;
	m_bThreadRun = false;
	m_bThreadWork = false;
}


//++++++++ erstellt Client und verbindet sich mit Scanner, fordert Handle und interpretiert Nachricht   ++++++++
int PaFR2000Scanner::initScanner(std::string strIp, unsigned short usPort, std::string strPacketType)
{
	this->m_strIp = strIp;
	this->m_usPort = usPort;
	std::string p_strRecv;
	std::string p_strErrCode;
	std::string p_strTcpPort;

	int iVal = -1;
	int p_iTcpPort;

	if( false == m_bInit )
	{
		std::stringstream* strstmTextualOutput = new std::stringstream();
		*strstmTextualOutput << "IP: " << m_strIp <<" ";
		*strstmTextualOutput << "Port: " << m_usPort << std::endl;
		m_objDebugOutput->debug(0, strstmTextualOutput->str().c_str());
		delete(strstmTextualOutput);

		// erstellt Telegramm und erfragt Handle vom Scanner, Return 0 = success
		iVal = sendAndRecive(createTelegramm("request_handle_tcp", 4, "packet_type", strPacketType.c_str(),"watchdog","off"), &p_strRecv,true);
		if (0 == iVal)
		{
			// filtert Error Code aus empfangenen Telegramm , Retun 0 = success
			p_strErrCode = parseTelegram(p_strRecv, "error_code");
			if ("0" == p_strErrCode)
			{
				// filtert Port und Handle aus empfangenen Telegramm
				m_strHandle = parseTelegram(p_strRecv, "handle");
				p_strTcpPort = parseTelegram(p_strRecv, "port");
				std::stringstream sstr(p_strTcpPort);
				// konvertiert String in integer
				sstr >> p_iTcpPort;
				if (0 <= p_iTcpPort)
				{
					// erstellt einen TCP Client um Scandaten zu empfangen
					iVal = createTcpConnection(p_iTcpPort);
					if (0 == iVal)
					{
						m_objDebugOutput->debug(0, "init Scanner");

						m_bInit = true;

						// Kontrolliert ob Thread schon/noch aktiv
						if (false == this->m_objThread_buffer->checkThreadRun() )
						{
							iVal = this->m_objThread_buffer->createThread(thread_Receive, (void*)this);
							if (0 == iVal)
							{
								iVal = this->m_objThread_convert->createThread(thread_ScanData, (void*)this);
								if (0 == iVal)
								{
									std::string p_sRecv;
									iVal = this->sendAndRecive(createTelegramm("start_scanoutput", 2, "handle", m_strHandle.c_str()), &p_sRecv,true);
									if (0 == iVal)
									{
										Sleep(10);
										m_objDebugOutput->debug(0, "Start Scanner output");
									}
									else
									{
										iVal = -4;
									}

								}
								else
								{
									iVal = -3;
									m_objDebugOutput->debug(0, "Thread getScanData bereits error");

								}
							}
							else
							{
								iVal = -2;
								m_objDebugOutput->debug(0, "Thread Receive bereits error");

							}
						}
					}
				}
			}
		} // if(0 == iVal) prueft ob Empfang erfolgreich
		else
		{
			m_objDebugOutput->debug(0, "Fehler bei initScanner...");
			iVal = -1;
		}
	}
	else
	{
		m_objDebugOutput->debug(0, "Scanner bereits initialisiert...");
	}
	Sleep(100);
	// return 0 = succes, -1 = Error
	return iVal;
}

//++++++++ sendet Parameter um Scan zu Starten  ++++++++
int PaFR2000Scanner::startScanOutput()
{
	int iVal = -2;

	if (true == m_bInit)
	{
		if (true == this->m_bThreadRecv)
		{
			if (false == this->m_bThreadWork)
			{
				m_objDebugOutput->debug(0, "Start record");
				this->m_bThreadWork = true;
				iVal = 0;
			}
		}
		else
		{
			m_objDebugOutput->debug(0, "Fehler bei Start Scanoutput");
			iVal = -1;
		}
	}
	return iVal;
}

//++++++++ sendet Parameter um Scan zu zu beenden  ++++++++
int PaFR2000Scanner::stopScanOutput()
{
	int iVal = -2;

	if (true == m_bInit)
	{
		if (true == this->m_bThreadRecv)
		{
			if (true == this->m_bThreadWork)
			{
				m_objDebugOutput->debug(0, "Stop record");

				this->m_bThreadWork = false;
				iVal = 0;
		
			}
		}
		else
		{
			m_objDebugOutput->debug(0, "Fehler bei Start Scanoutput");
			iVal = -1;
		}
	}

	return iVal;
}

//++++++++  gibt Scanpunkte zurueck ++++++++
int PaFR2000Scanner::getRecordPoints(std::list<SPunkt>& listPunkte, std::list<double>& listTimestamps)
{
	ScanData p_scanData;
	int p_iVal = -1;
	bool bData = false;

	if (true == this->m_bThreadWork)
	{
		do
		{
			if (0 < m_listScanData.size())
			{
				EnterCriticalSection(m_csObjectBuffer);
				p_scanData = m_listScanData.back();
				m_listScanData.pop_back();
				LeaveCriticalSection(m_csObjectBuffer);

				p_iVal = p_scanData.getPunkteUndZeit(m_dMinAngle, m_dMaxAngle, m_dMinDist, m_dMaxDist, &listPunkte, &listTimestamps);
				bData = true;
			}
			Sleep(0);
		} while (false == bData);
	}
	return p_iVal;
}

//++++++++ gibt gefilterte Scanpunkte zurueck ++++++++
int PaFR2000Scanner::getFilterRecordPoints(std::list<SPunkt>& listPunkte, std::list<double>& listTimestamps, unsigned int iAnzahl, unsigned int iMode )
{
	std::list<SPunkt> lTempPoint;
	std::list<double> lTempTime;
	std::list<SPunkt> lTempPointAkt;
	std::list<double> lTempTimeAkt;
	ScanData p_scanDataFirst;
	ScanData p_scanData;
	ScanData objDatenPuffer;
	int p_iVal = 0;
	unsigned int p_iCount = 0;

	int iTrys = 0;

	do
	{
		if (0 < m_listScanData.size())
		{

			if (0 == p_iCount)
			{
				EnterCriticalSection(m_csObjectBuffer);
				p_scanDataFirst = m_listScanData.front();
				m_listScanData.pop_front();
				LeaveCriticalSection(m_csObjectBuffer);
				p_iCount++;
			}
			else
			{
				EnterCriticalSection(m_csObjectBuffer);
				p_scanData = m_listScanData.front();
				m_listScanData.pop_front();
				LeaveCriticalSection(m_csObjectBuffer);

				if (-1 != p_scanDataFirst.addPunkte(p_scanData))
				{
					p_iCount++;
				}
			}
			
		}
		else
		{
			if (100000 < iTrys)
			{
				return -1;
			}
			iTrys++;
		}
		Sleep(1);
	} while (p_iCount != iAnzahl);

	this->m_objDebugOutput->debug(0, "getFilterPunkte");

	switch (iMode)
	{
		//Mittelwert
		case 1: p_iVal = p_scanDataFirst.getPunkteUndZeit(m_dMinAngle, m_dMaxAngle, m_dMinDist, m_dMaxDist, &listPunkte, &listTimestamps , iMode);
				break;
		//Median
		case 2: p_iVal = p_scanDataFirst.getPunkteUndZeit(m_dMinAngle, m_dMaxAngle, m_dMinDist, m_dMaxDist, &listPunkte, &listTimestamps, iMode);
				break;
		//Mittelwert 8:1
		case 3: p_iVal = p_scanDataFirst.getPunkteUndZeit(m_dMinAngle, m_dMaxAngle, m_dMinDist, m_dMaxDist, &listPunkte, &listTimestamps, iMode-2,8);
			break;
		//Median 8:1
		case 4: p_iVal = p_scanDataFirst.getPunkteUndZeit(m_dMinAngle, m_dMaxAngle, m_dMinDist, m_dMaxDist, &listPunkte, &listTimestamps, iMode-2,8);
			break;

	}

	return p_iVal;
}

//++++++++ erstellt einen Scan   ++++++++
int PaFR2000Scanner::getSingleScan(std::list<SPunkt>& listPunkte, std::list<double>& listTimestamps)
{
	ScanData p_scanData;
	int p_iVal = 0;
	bool p_bCheck = false;

	startScanOutput();
	do
	{
		if (0 < m_listScanData.size())
		{
			EnterCriticalSection(m_csObjectBuffer);
			p_scanData = m_listScanData.front();
			m_listScanData.pop_front();
			m_listScanData.clear();

			LeaveCriticalSection(m_csObjectBuffer);
			p_iVal = p_scanData.getPunkteUndZeit(m_dMinAngle, m_dMaxAngle, m_dMinDist, m_dMaxDist, &listPunkte, &listTimestamps);
			p_bCheck = true;
		}
	} while (p_bCheck == false);
	stopScanOutput();
	return p_iVal;
}

//++++++++ setzte Winkelfilter  ++++++++
int PaFR2000Scanner::setAngleFilterLimits(double dMinAngle, double dMaxAngle)
{
	int iVal = -1;

	if (dMinAngle != dMinAngle)
	{
		if ((dMinAngle >= -180000) && (dMinAngle <= 180000))
		{
			m_dMinAngle = dMinAngle;
			iVal = 0;
		}
		else
		{
			m_dMinAngle = -180000;
			m_objDebugOutput->debug(0, "minAngle ausserhalb des Bereichs, Standardwert wird genutzt");
			iVal = -1;

		}

		if ((dMaxAngle >= -180000) && (dMaxAngle <= 180000))
		{
			m_dMaxAngle = dMaxAngle;
			iVal = 0;
		}
		else
		{
			m_dMaxAngle = 179999;
			m_objDebugOutput->debug(0, "maxAngle ausserhalb des Bereichs, Standardwert wird genutzt");
			iVal = -1;

		}
	}
	else
	{
		m_dMinAngle = -180000;
		m_dMaxAngle = 179999;
		m_objDebugOutput->debug(0, "minAngle ist gleich maxAngle, Standardwerte werden genutzt");
		iVal = -1;
	}

	return iVal;
}

//++++++++  setzt Distanzfilter ++++++++
int PaFR2000Scanner::setDistanceFilterLimits(double dMinDist, double dMaxDist)
{
	int iVal = -1;

	if (dMinDist != dMaxDist)
	{
		if ((dMinDist >= 20) && (dMinDist <= 50000))
		{
			m_dMinDist = dMinDist;
			iVal = 0;
		}
		else
		{
			m_dMinDist = 20;
			m_objDebugOutput->debug(0, "minDistanz ausserhalb des Bereichs, Standardwert wird genutzt");
			iVal = -1;

		}

		if ((dMaxDist >= 20) && (dMaxDist <= 50000))
		{
			m_dMaxDist = dMaxDist;
			iVal = 0;
		}
		else
		{
			m_dMaxDist = 50000;
			m_objDebugOutput->debug(0, "maxDistanz ausserhalb des Bereichs, Standardwert wird genutzt");
			iVal = -1;

		}
	}
	else
	{
		m_dMinDist = 20;
		m_dMaxDist = 50000;
		m_objDebugOutput->debug(0, "minDistanz ist gleich maxDistanz, Standardwerte werden genutzt");
		iVal = -1;
	}

	return iVal;
}


//++++++++  gibt den aktuellen Thread Receive Status ++++++++
int PaFR2000Scanner::getScanStatus()
{
	int p_iVal = -1;
	std::string p_strStatus;
	if (true == m_bThreadRecv)
	{
		p_strStatus = "true";
		p_iVal = 0;
	}
	else
	{
		p_strStatus = "false";
		p_iVal = -1;
	}
	return p_iVal;

}

//++++++++ beendet Thread und TCP Client  ++++++++
int PaFR2000Scanner::closeConnection()
{
	int iVal = 0;
	std::string sRecv;

	
	
	iVal = this->sendAndRecive(createTelegramm("stop_scanoutput", 2, "handle", m_strHandle.c_str()), &sRecv, true);
	if (0 != iVal)
	{
		m_objDebugOutput->debug(0, "Fehler bei stop_scanoutput...");
	}

	iVal = this->sendAndRecive(createTelegramm("release_handle", 2, "handle", m_strHandle.c_str()), &sRecv,false);
	if (0 != iVal)
	{
		m_objDebugOutput->debug(0, "Fehler bei release_handle...");
	}

	this->m_objThread_convert->terminateThread();
	this->m_objThread_buffer->terminateThread();

	this->m_objClientTcp->closeSocket();

	return iVal;
}


// +++++++++++++++++++++++++++++  Private Methoden  ++++++++++++++++++++++++++++


//++++++++ erstellt ein Telegram in gueltiger HTTP Syntax  ++++++++

std::string PaFR2000Scanner::createTelegramm(std::string strCom, int iCount, ...)
{
	std::string p_strParameter = "Error";

	if ((iCount % 2) == 0)
	{
		va_list p_list;
		std::string p_quelle;
		p_strParameter = "GET /cmd/";
		p_strParameter += strCom;
		p_strParameter += "?";

		try
		{
			/*va_start füllt die Liste mit Argumenten, die uebergeben wurden
			quelle holt Argumente aus der Liste und ergaenzt den string*/
			va_start(p_list, iCount);
			for (int i = 1; i <= iCount; i++)
			{
				p_quelle = va_arg(p_list, char*);
				p_strParameter += p_quelle;
				if (iCount != i)
				{
					if ((i % 2) == 1)
					{
						p_strParameter += "=";
					}
					else
					{
						p_strParameter += "&";
					}
				}
			}
			va_end(p_list);
			p_strParameter += " HTTP/1.0\r\n\r\n";
		}
		catch (std::string strA)
		{
			m_objDebugOutput->debug(0, "Fehler bei CreateTelegramm...", strA);
		}
		catch (...)
		{
			m_objDebugOutput->debug(0, "Fehler bei CreateTelegram");
		}
	}// if ((iCount % 2) == 0)

	return p_strParameter;
}

int PaFR2000Scanner::reConnectScanner()
{
	std::string p_strRecv;
	std::string p_strErrCode;
	std::string p_strTcpPort;

	int p_iVal = 0;
	int p_iTcpPort;

	try
	{
		std::stringstream* strstmTextualOutput = new std::stringstream();
		*strstmTextualOutput << "IP: " << m_strIp << " ";
		*strstmTextualOutput << "Port: " << m_usPort << std::endl;
		m_objDebugOutput->debug(0, strstmTextualOutput->str().c_str());
		delete(strstmTextualOutput);

		// erstellt Telegramm und erfragt Handle vom Scanner, Return 0 = success
		p_iVal = sendAndRecive(createTelegramm("request_handle_tcp", 2, "packet_type", 'A'), &p_strRecv, true);
		if (0 == p_iVal)
		{
			// filtert Error Code aus empfangenen Telegramm , Retun 0 = success
			p_strErrCode = parseTelegram(p_strRecv, "error_code");
			if ("0" == p_strErrCode)
			{
				// filtert Port und Handle aus empfangenen Telegramm
				m_strHandle = parseTelegram(p_strRecv, "handle");
				p_strTcpPort = parseTelegram(p_strRecv, "port");
				std::stringstream sstr(p_strTcpPort);
				// konvertiert String in integer
				sstr >> p_iTcpPort;
				if (0 <= p_iTcpPort)
				{
					// erstellt einen TCP Client um Scandaten zu empfangen
					p_iVal = createTcpConnection(p_iTcpPort);
					if (0 == p_iVal)
					{
						m_objDebugOutput->debug(0, "init Scanner");

						m_bInit = true;

						
						std::string p_sRecv;
						p_iVal = this->sendAndRecive(createTelegramm("start_scanoutput", 2, "handle", m_strHandle.c_str()), &p_sRecv, true);
										if (0 == p_iVal)
										{
											Sleep(10);
											m_objDebugOutput->debug(0, "Start Scanner output");
										}
									else
						{
								p_iVal = -4;
						}

					}
				}
			}
		} // if(0 == iVal) prueft ob Empfang erfolgreich
		else
		{
			m_objDebugOutput->debug(0, "Fehler bei initScanner...");
			p_iVal = -1;
		}
	}
	catch (...)
	{
		m_objDebugOutput->debug(0, "Fehler bei initScanner");
	}
	Sleep(100);
	// return 0 = succes, -1 = Error
	return p_iVal;
}

//++++++++  erstellt einen Client mit zugewiesenem Port / TCP ++++++++
int PaFR2000Scanner::createTcpConnection(int iPort)
{
	int p_iVal = 0;

	p_iVal = m_objClientTcp->connectSocket(m_strIp, (unsigned short)iPort);
	if (0 == p_iVal)
	{
		m_objDebugOutput->debug(0, "TCP connect");
	}
	else
	{
		p_iVal = -1;
		m_objDebugOutput->debug(0, "Error Create TCP");
	}
	// return 0 = succes, -1 = Error
	return p_iVal;
}

//++++++++ holt den gesuchten Parameterwert anhand der uebergebenen Daten  ++++++++
std::string PaFR2000Scanner::parseTelegram(std::string strTelegram, std::string strParameter)
{
	std::string strParaWert;
	try
	{
		// ermittelt die laenge des uebergebenen Parameters
		int p_iParaLen = static_cast<int>((strParameter.length() - 1));
		int p_iParaAnfang = 0;
		// ermittelt die Position des ersten Zeichens des Parameters
		p_iParaAnfang = static_cast<int>(strTelegram.find(strParameter));
		if (-1 != p_iParaAnfang)
		{
			// +3, nach Parameter befinden sich 3 Zeichen die nicht benoetigt werden
			int p_iParaStart = p_iParaLen + p_iParaAnfang + 3;
			int p_iParaEnd = static_cast<int>(strTelegram.find(',', p_iParaStart));
			strParaWert = strTelegram.substr(p_iParaStart, (p_iParaEnd)-p_iParaStart);

			if (2 <= strParaWert.length())
			{
				if ('\"' == strParaWert[0])
				{
					strParaWert.erase(strParaWert.begin(), (strParaWert.begin() + 1));
					strParaWert.erase((strParaWert.end() - 1), strParaWert.end());
				}
			}
		}
	}
	catch (...)
	{
		m_objDebugOutput->debug(0, "Fehler ParseTelegramm");
	}
	return strParaWert;
}

//++++++++ erstellt einen neuen Client und uebermittelt uebergebenen Parameter an Scanner und empfaengt Antwort  ++++++++
int PaFR2000Scanner::sendAndRecive(std::string strSend, std::string* strRecv, bool bReadError = true)
{
	std::string p_strErrCode;
	Client p_cClient;
	int p_iVal = 0;
	int p_iErr;
	char p_cRecv[2048];


	p_iVal = p_cClient.connectSocket(m_strIp, m_usPort);
	if (0 == p_iVal)
	{
		p_iVal = p_cClient.sendMessage(strSend);
		Sleep(50);
		if (0 == p_iVal)
		{
			p_iVal = p_cClient.receiveMessage(p_cRecv, 2048);
			if (0 < p_iVal)
			{
				if (true == bReadError)
				{
					*strRecv = p_cRecv;
					p_strErrCode = parseTelegram(*strRecv, "error_code");
					p_iErr = stoi(p_strErrCode);

					switch (p_iErr)
					{
						case 0:	m_objDebugOutput->debug(0, "success");
							p_iVal = 0;
							break;
						case 100:
							m_objDebugOutput->debug(0, "unknown argument ");
							p_iVal = -1;
							break;
						case	110:
							m_objDebugOutput->debug(0, "unknown parameter");
							p_iVal = -1;
							break;
						case	120:
							m_objDebugOutput->debug(0, "invalid handle or no handle provided");
							p_iVal = -1;
							break;
						case 130:
							m_objDebugOutput->debug(0, "required argument missing");
							p_iVal = -1;
							break;
						case 200:
							m_objDebugOutput->debug(0, "invalid value for argument");
							p_iVal = -1;
							break;
						case 210:
							m_objDebugOutput->debug(0, "value for parameter out of range");
							p_iVal = -1;
							break;
						case 220:
							m_objDebugOutput->debug(0, "write-access to read-only parameter");
							p_iVal = -1;
							break;
						case 230:
							m_objDebugOutput->debug(0, "insufficient memory");
							p_iVal = -1;
							break;
						case 240:
							m_objDebugOutput->debug(0, "resource already/still in use");
							p_iVal = -1;
							break;
						case 333:
							m_objDebugOutput->debug(0, "(internal) error while processing command");
							p_iVal = -1;
							break;
						default: p_iVal = 0;

						}
					}
				}
				else
				{
					p_iVal = -1;
				}
			}
		}
		else
		{
			m_objDebugOutput->debug(0, "Error Client Connect Socket");
		}
	
	// return 0 = succes, -1 = Error
	return p_iVal;
}

//++++++++ startet einen Thread und liest Daten ein  ++++++++
void PaFR2000Scanner::thread_Receive(CThread* pclThread)
{
	PaFR2000Scanner* p_objTmp;
	p_objTmp = (PaFR2000Scanner*)pclThread->m_pvThreadObject;

	CDebugOutput* p_debugOut;
	p_debugOut = p_objTmp->m_objDebugOutput;

	std::string p_strMagicKey;
	std::string p_strZielString;
	std::string p_strTemp;
	double p_dTimeAkt = 0;
	double p_dTime = -1;
	bool bClear = false;
	int p_iVal = 0;

	p_strMagicKey.append(1, 0x5c);
	p_strMagicKey.append(1, 0xa2);
	
	p_objTmp->m_bThreadRecv = true;

	do
	{
		char p_cRecv[2048];
		p_iVal = p_objTmp->m_objClientTcp->receiveMessage(p_cRecv, 2048);
		if (true == p_objTmp->m_bThreadWork)
		{
			if (0 < p_iVal)
			{
				bClear = false;
				if (-1 == p_dTime)
				{
					p_dTime = CTimeStamp::getTimeStamp();
					p_dTimeAkt = CTimeStamp::getTimeStamp();
				}
				else
				{
					p_dTimeAkt = CTimeStamp::getTimeStamp();
				}

				std::string p_strCopy(&p_cRecv[0], &p_cRecv[0] + p_iVal);
				p_strTemp += p_strCopy;
				size_t found = p_strTemp.find(p_strMagicKey, 3);
				if (std::string::npos != found)
				{
					p_strZielString = p_strTemp.substr(0, found);
					p_strTemp.erase(0, found);
					
					EnterCriticalSection(p_objTmp->m_csTimeBuffer);
					p_objTmp->m_listZeitstempel.push_back(p_dTime);
					LeaveCriticalSection(p_objTmp->m_csTimeBuffer);
					
					p_dTime = p_dTimeAkt;
					
					EnterCriticalSection(p_objTmp->m_csStringBuffer);
					p_objTmp->m_listPaket.push_back(p_strZielString);
					LeaveCriticalSection(p_objTmp->m_csStringBuffer);
					p_strZielString.clear();
				}
			} 
			else
			{
				p_objTmp->reConnectScanner();
			}
			Sleep(0);
		}
		else
		{
			if (false == bClear)
			{
				EnterCriticalSection(p_objTmp->m_csStringBuffer);
				p_objTmp->m_listPaket.clear();
				LeaveCriticalSection(p_objTmp->m_csStringBuffer);

				EnterCriticalSection(p_objTmp->m_csTimeBuffer);
				p_objTmp->m_listZeitstempel.clear();
				LeaveCriticalSection(p_objTmp->m_csTimeBuffer);

				p_strTemp.clear();
				p_strZielString.clear();
				p_dTime = -1;
				p_dTimeAkt = -1;
				bClear = true;
			}
			Sleep(10);
		}
	} while( true != pclThread->checkThreadStop() );
	
	p_objTmp->m_bThreadRecv = false;
	pclThread->exitThread();
}

//++++++++ interpretiert empfangene Scandaten  ++++++++
void PaFR2000Scanner::thread_ScanData(CThread* pclThread)
{
	PaFR2000Scanner* objScanner;
	objScanner = (PaFR2000Scanner*)pclThread->m_pvThreadObject;

	CDebugOutput* p_debugOut;
	p_debugOut = objScanner->m_objDebugOutput;

	ScanData* p_scanData = new ScanData();

	std::string p_strTmp;
	double p_dTime = 0;
	int p_iVal = 0;
	bool bClear = false;

	objScanner->m_bThreadScanData = true;
	do
	{
		if (true == objScanner->m_bThreadWork)
		{
			bClear = false;
			// Wenn Daten in Liste
			if (0 < objScanner->m_listPaket.size())
			{
				EnterCriticalSection(objScanner->m_csStringBuffer);
				p_strTmp = objScanner->m_listPaket.front();
				objScanner->m_listPaket.pop_front();
				LeaveCriticalSection(objScanner->m_csStringBuffer);
			
				if (0 < objScanner->m_listZeitstempel.size())
				{
					EnterCriticalSection(objScanner->m_csTimeBuffer);
					p_dTime = objScanner->m_listZeitstempel.front();
					objScanner->m_listZeitstempel.pop_front();
					LeaveCriticalSection(objScanner->m_csTimeBuffer);
				}

				p_iVal = p_scanData->parseTelegramm(p_strTmp, p_dTime);
				if (1 == p_iVal)
				{
						EnterCriticalSection(objScanner->m_csObjectBuffer);
						objScanner->m_listScanData.push_back(*p_scanData);
						LeaveCriticalSection(objScanner->m_csObjectBuffer);
						delete p_scanData;
						p_scanData = new ScanData();
				}
				else if (-1 == p_iVal)
				{
					delete p_scanData;
					p_scanData = new ScanData();
				}
			}
			Sleep(0);
		}
		else
		{
			if (false == bClear)
			{
				if (0 != objScanner->m_listScanData.size())
				{
					objScanner->m_objDebugOutput->debug(0, "Clear thread_ScanData");
					EnterCriticalSection(objScanner->m_csObjectBuffer);
					objScanner->m_listScanData.clear();
					LeaveCriticalSection(objScanner->m_csObjectBuffer);
				}
				delete p_scanData;
				p_scanData = new ScanData();
			}
			Sleep(10);
		}
	} while (true != pclThread->checkThreadStop());
	objScanner->m_bThreadScanData = false;
	pclThread->exitThread();
}

//++++++++ Destruktor  ++++++++
PaFR2000Scanner:: ~PaFR2000Scanner(void)
{
	this->closeConnection();
	m_listZeitstempel.clear();
	m_listPaket.clear();
	m_listScanData.clear();
	delete m_objClientTcp;
	delete m_objThread_buffer;
	delete m_objThread_convert;
}
