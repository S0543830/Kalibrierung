#ifndef __RSL430SCAN_H_INCLUDED__
#define __RSL430SCAN_H_INCLUDED__

#include <winsock2.h>
#include <ws2tcpip.h>

#include <list>
#include <string>
#include <iostream>
#include <vector>

#include "Thread.h"
#include "DataTypes.h"
#include "RSL430Structs.h"

#pragma comment( lib, "Ws2_32.lib" )


enum RSL430Return
{
	RSL430_RETURN_OK = 0,
	RSL430_RETURN_INVALIDSOCKET = -11,
	RSL430_RETURN_BINDFAIL = -12,
	RSL430_RETURN_BADPACKET = -13,
	RSL430_RETURN_STARTFAIL = -20,
	RSL430_RETURN_STOPFAIL = -21,
	RSL430_RETURN_CREATEFAIL = -22,
	RSL430_RETURN_NOPOINTS = -2,
	RSL430_RETURN_ERROR = -1,
};

enum RSL430TeleID
{
	RSL430_ID_CONDES = 1,
	RSL430_ID_CONVAL = 6,
	RSL430_HEADER_SIZE = 20,
	RSL430_TIMEOUT = 500,
	RSL430_SINGLESCAN_TRYS = 25
};

typedef struct
{
	RSL430::MessKontBesch stMessBe;
	unsigned long ulScanNr;
	int iDataSollSize;
	double dTimeRecv;
	std::vector< unsigned short > Daten;
	int iMittel;
}RSL430ScanData;



class CRSL430Scan
{
public:

	CRSL430Scan( );
	~CRSL430Scan( );

	int initRSL430UDP( const char* szHostName, unsigned int uiPort );

	int exitRSL430( void );

	void setAngleFilterLimits( double dMinAngle, double dMaxAngle );

	int getRSL430Status( void );

	void setDistanceFilterLimits( double dMinDist, double dMaxDist );

	int getSingleScan( std::list<CPunkt>& lcpPoints, std::list<double>& ldTimeStamps );

	int getRecordedScanPoints( std::list<CPunkt>& lcpRecPoints, std::list<double>&ldRecTimeStamps );

	int startScanRecording( void );
	int stopScanRecording( void );

	double getTimeStamp( void );

private:
	static void thread_ReceiveGatherer( CThread* pclThread );
	int getBeamCount(unsigned short usStart, unsigned short usStop, unsigned short usReso);

	int convertScanDataInKartesisch(RSL430ScanData strScanIn,
									std::list<CPunkt>& lOut,
									std::list<double> &lTimeOut);

	int loadCorrectValueDat(const char*  szDatName);
	int loadCorrectValue(std::vector<double> vecCorValue);



private:
	unsigned int iPufferSize;
	int iThreatState;
	bool bScannerRun;

	SOCKET RSL430Socket;
	SOCKADDR_IN RSL430Addr;

	std::list<RSL430ScanData> listPufferScanData;

	LPCRITICAL_SECTION critListScanData;
	CThread* objThread_Gatherer;
	
	//Scanner Beschränkungen
	double dScanMinAngle;
	double dScanMaxAngle;
	double dScanMinDist;
	double dScanMaxDist;
	double dScanOffsetAngle;
	double dScannerHardMaxDist;
	int iScanTime;
	int iValueSize;
	std::vector<double> vecCorrValue;

	//Serverinformationen
	std::string strHostName;
	unsigned int uiPortNr;
};
#endif