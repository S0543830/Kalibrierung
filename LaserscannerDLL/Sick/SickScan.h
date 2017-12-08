#ifndef __SICKSCAN_H_INCLUDED__
#define __SICKSCAN_H_INCLUDED__

#include <winsock2.h>
#include <ws2tcpip.h>

#include <vector>
#include <list>

#include "DataTypes.h"
#include "Thread.h"

#define NetBuffer 4096

#pragma comment( lib, "Ws2_32.lib" )
using namespace std;

enum cSickScanReturn
{
	SICK_OK		= 0x0000,
	SICK_NORECPOINTS = 0x1001,
	SICK_ALLGERROR = 0x1002,
	SICK_ALLGWARNING = 0x1003,
	SICK_FATALERROR = 0x1004,
	SICK_STARTSCANNERFAIL = 0xA001,
	SICK_STOPSCANNERFAIL  = 0xA002,
	SICK_SETCONFFAIL = 0xB001,
	SICK_CONFREQUENCYERR = 0xB002,
	SICK_CONFRESOLITIONERR = 0xB003,
	SICK_CONFRESANDSCNERR = 0xB004,
	SICK_CONFSCANAREAERR = 0xB005,
	SICK_LOGINERROR = 0xC001,
	SICK_LOGOUTERROR = 0xC002,
	SICK_NETWORKSENDERROR = 0xD001,
	SICK_NETWORKRECVERROR = 0xD002,
	SICK_NETWORKTIMEOUT = 0xD000,
	SICK_NOCONNECTION = 0xD004
};

typedef struct
{
	float fAngleMin;
	float fAngleMax;
	float fAngleInc;
	float fAngleReso;
	float fIncTime;
	float fScanTime;
	float fRangeMin;
	float fRangeMax;
	double dOffset;
	signed int siStartAngle;
	std::vector<float> vecRanges;
	std::vector<float> vecIntensities;
	double dStamp;
}SickScanDataLMS400;

typedef struct
{
	unsigned short usFormat;
	unsigned short usDistScal;
	int  iStartAngle;
	unsigned short usAngularStepWidth;
	unsigned short usMeasSize;
	unsigned short usScanFreq;
	unsigned short usRemScal;
	unsigned short usRemStartVal;
	unsigned short usRemEndVal;
} MeasurementHeader_t;

typedef struct
{
	unsigned char* string;
	int length;
} MeasurementQueueElement_t;

class CSickScan
{
	public:
		
		CSickScan();
		~CSickScan();
		
		int initSickScan( const char* szHostName,  const char *szPort );
		int exitSickScan( void );

		void setAngleFilterLimits( double dMinAngle, double dMaxAngle );
		void setDistanceFilterLimits( double dMinDist, double dMaxDist );

		int startScanRecording();
		int stopScanRecording();

		int getRecordedScanPoints( std::list <CPunkt>& rlstclPoints,
								   std::list <double>& rlstdTimeStamps,
								   bool bInterpoleateTimeStamps );

		int getStatus( void );
		unsigned int readMistake();
		unsigned int readBigMistake();
		int getSingleScan( std::list<CPunkt> &lcpPoints, 
							  std::list<double> &ldTimeStamps);
	
		double getTimeStamp( void );

		int configSick( float fAngleReso,
						int iGScanFreq,
						float fAngleStart,
						float fAngleLen );


	private:

		int startKontiMessung(void);
		int stopKontiMessung(void);
		SOCKET getConnection(const char *szHostName, const char *szPort);

	    int sendToSickLMS( char *s,int len );
		int receiveFromSickLMS(SOCKET s);
		static void thread_ReceiveGatherer( CThread* pclThread );

		SickScanDataLMS400 readMeasurement( SOCKET s );
		int convertDataFromLMS400(SickScanDataLMS400 lms, std::list<CPunkt> &lScanOut, std::list<double> &ldTimeStamps);

	
private:
		std::vector<MeasurementQueueElement_t>* vecMeasurementQueue;
		std::list<SickScanDataLMS400> listPufferScanData;
		LPCRITICAL_SECTION critListScanData;
		CThread* objThread_Gatherer;

		SOCKET SickSocket;
		
		int iMinIntensity;
		int iThreatState;
		int iVerbose;
		bool bScannerRun;
		bool isScanStop;
		double dScanMinAngle;
		double dScanMaxAngle;
		double dScanMinDist;
		double dScanMaxDist;
		double dScanOffsetAngle;
		unsigned int iPufferSize;
};

#endif