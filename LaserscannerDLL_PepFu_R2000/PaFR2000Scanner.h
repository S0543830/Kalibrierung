#ifndef __PAFR2000SCANNER_H_INCLUDED__
#define __PAFR2000SCANNER_H_INCLUDED__

#include "Client.h"
#include "ScanData.h"
#include "Thread.h"
#include "TimeStamp.h"
#include <list>
#include <sstream>




class PaFR2000Scanner
{
public:
	PaFR2000Scanner(void);
	~PaFR2000Scanner(void);

	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: erfragt TCP Handle und oeffnet TCP Verbindung, ruft Thread 
				  zum Datenempfang und zur Datenverarbeitung auf 
	Parameter:
	IN string p_strIp – uebergibt IP Adresse
	IN int p_iPort - uebergibt Port
	IN string p_strPacketType - uebergibt Packet Typ A oder B

	Rueckgabe:
	int – erfolgreich mit 0
	*********************************************************************/
	int initScanner(std::string p_strIp, unsigned short usPort, std::string p_strPacketType);


	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: startet ueber Methode Send and Recieve den Scanvorgang
	Parameter:
	Rueckgabe:
	int – erfolgreich mit 0
	*********************************************************************/
	int startScanOutput();


	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: stopt ueber Methode Send and Recieve den Scanvorgang und gibt Handle zurueck
	Parameter:
	Rueckgabe:
	int – erfolgreich mit 0
	*********************************************************************/	
	int stopScanOutput();


	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: schliesst beide Threads, baut Verbindung ab
	Parameter:
	Rueckgabe:
	int – erfolgreich mit 0
	*********************************************************************/
	int closeConnection();


	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: erstellt ein Objekt von ScanData und reicht die Listen 
				  TimeStamp und Punkte in einer Critical Section durch
	Parameter:
	IN list<CPunkt>* listPunkte – enthaelt alle Punkte eines Scans
	IN list<double>* listTimestamps - enthaelt alle Zeitstempel eines Scans
	OUT list listPunkte -
	OUT list listTimestamps
	Rueckgabe:
	int – erfolgreich mit 0
	*********************************************************************/	
	int getRecordPoints(std::list<SPunkt>& listPunkte, std::list<double>& listTimestamps);


	/********************************************************************
	Erstellt:		08.2017
	Autor:			C. Armas Martinez
	Beschreibung: erstellt ein Objekt von ScanData und reicht die Listen
	TimeStamp und Punkte in einer Critical Section durch
	Parameter:
	IN list<CPunkt>* listPunkte – enthaelt alle Punkte eines Scans
	IN list<double>* listTimestamps - enthaelt alle Zeitstempel eines Scans
	OUT list listPunkte -
	OUT list listTimestamps
	Rueckgabe:
	int – erfolgreich mit 0
	*********************************************************************/
	int getFilterRecordPoints(std::list<SPunkt>& listPunkte, std::list<double>& listTimestamps, unsigned int iAnzahl, unsigned int iMode);

	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: erstellt ein Objekt von ScanData und startet den Scan
				  wenn ein Objekt erstellt ist wird Stop Scan aufgerufen
	Parameter:
	IN list<CPunkt>* listPunkte – enthaelt alle Punkte eines Scans
	IN list<double>* listTimestamps - enthaelt alle Zeitstempel eines Scans
	OUT list listPunkte -
	OUT list listTimestamps
	Rueckgabe:
	int – erfolgreich mit 0
	*********************************************************************/
	int getSingleScan(std::list<SPunkt>& listPunkte, std::list<double>& listTimestamps);


	/********************************************************************
	Erstellt:		09.2017
	Autor:			C.Armas Martinez
	Beschreibung: Filtert die Scanpunkte nach Winkeln
	Parameter:
	double dMinAngle - kleinster Winkel
	double dMaxAngle - groesster Winkel
	Rueckgabe:
	int – erfolgreich mit 0
	*********************************************************************/
	int setAngleFilterLimits(double dMinAngle, double dMaxAngle);


	/********************************************************************
	Erstellt:		09.2017
	Autor:			C.Armas Martinez
	Beschreibung: Filtert die Scanpunkte nach Distanz
	Parameter:
	double dMinDist - kleinste Distanz
	double dMaxDist - groesste Distanz
	Rueckgabe:
	int – erfolgreich mit 0
	*********************************************************************/
	int setDistanceFilterLimits(double dMinDist, double dMaxDist);


	/********************************************************************
	Erstellt:		09.2017
	Autor:			C.Armas Martinez
	Beschreibung: Gibt den Status vom Receivethread zurück
	Parameter:
	Rueckgabe:
	int – erfolgreich mit 0
	*********************************************************************/
	int getScanStatus();


		
private:
	CRITICAL_SECTION* m_csStringBuffer;
	CRITICAL_SECTION* m_csObjectBuffer;
	CRITICAL_SECTION* m_csTimeBuffer;
	CDebugOutput* m_objDebugOutput;
	CThread* m_objThread_buffer;
	CThread* m_objThread_convert;
	Client* m_objClientTcp;
	std::list<double> m_listZeitstempel;
	std::list<std::string> m_listPaket;
	std::list<ScanData> m_listScanData;



	std::string m_strHandle;
	std::string m_strIp;
	unsigned short m_usPort;

	double m_dMinAngle;
	double m_dMaxAngle;
	double m_dMinDist;
	double m_dMaxDist;

	/********************************************************************
	Checkvariablen fuer Threads und Socket Initialisierung
	*********************************************************************/
	bool m_bThreadRun;
	bool m_bThreadWork;
	bool m_bThreadRecv;
	bool m_bThreadScanData;
	bool m_bInit;

	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: erstellt einen Befehl in korrekter URI Syntax
	Parameter:
	IN string p_strCom – auszufuehrender Befehl
	IN int p_iCount - gibt an wieviele Parameter übergeben werden, keine ungerade Zahl möglich
	IN string - beliebige Anzahl von Parametern
	Rueckgabe:
	string – Rueckgabe des Befehls bei Fehlern = Error
	*********************************************************************/
	std::string createTelegramm(std::string p_strCom, int p_iCount, ...);

	int reConnectScanner();

	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: stellt eine TCP Verbindung her
	Parameter:
	IN int p_iPort – uebergabe des Ports
	IN string p_strIp - uebergabe der IP Adresse
	Rueckgabe:
	int – erfolgreich mit 0
	*********************************************************************/
	int createTcpConnection(int p_iPort);


	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: Beschreibung einer Methode
	Parameter:
	IN string p_strTelegram - uebergibt das empfangene Telegram
	IN string p_strParameter - uebergibt den zu suchenden Parameter, z.B. Error_Code	
	Rueckgabe:
	string – gibt den gesuchten Parameterwert zurueck
	*********************************************************************/
	std::string parseTelegram(std::string p_strTelegram, std::string p_strParameter);
	

	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: sendet mittels erstellten Cleint ein Telegramm und empfängt
				  Antwort und prueft diese auf ErrorCode
	Parameter:
	IN string p_strSend – Parameter 1 für …
	IN string* p_strRecv - Parameter 2 für …
	IN string strIp - uebergabe der IP Adresse um Client zu erstellen
	OUT int x -
	Rueckgabe:
	int – erfolgreich mit 0
	*********************************************************************/
	int sendAndRecive(std::string p_strSend, std::string* p_strRecv, bool bReadError );
	

	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: empfaengt Nachricht,prueft diese auf das MagicByte erstellt Zeitstempel
				  jeder String stellt ein Paket dar, diese werden in eine Liste gepackt
	Parameter:
	IN CThread p_pclThread a – uebergibt ein Thread Onjekt
	OUT list m_listPaket - 
	OUT list m_listZeitStempel - 
	*********************************************************************/
	static void thread_Receive(CThread* p_pclThread);
	

	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: wenn Pakete in der liste ,m_listPaket, sind werden diese 
				  geparst und die Daten in list ScanData geschrieben
	Parameter:
	IN CThread p_pclThread a – uebergibt ein Thread Objekt
	OUT list ScanData - eine liste mit Objekten von ScanData
	
	*********************************************************************/
	static void thread_ScanData(CThread* p_pclThread);

};
#endif

