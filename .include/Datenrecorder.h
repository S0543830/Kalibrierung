#ifndef _DATENRECORDER_H_
#define _DATENRECORDER_H_
#include "CScanner.h"
#include "Thread.h"
#include "CEncoder.h"
#include <vector>
#include <windows.h>

typedef struct
{
	std::string ccDLLPfad;
	std::string sIP;
	int iPort;
	int iScannerID;
	int iAnzahlScanner;
}ScannerInitParameter;

typedef struct
{
	unsigned int ID;
	unsigned int State;
}ScannerIpState;

enum ScannerThreadStates
{
	SCANNER_NOT_RUNNING = 0LU,	// initial value - thread not startet yet
	SCANNER_ACTIVE = 1LU,	// the thread is collecting data from the scanner and stores them
	SCANNER_TERMINATE_REQUEST = 2LU,	// thread is requested to terminate
	SCANNER_TERMINATED = 3LU,	// thread is running no more
	SCANNER_ERROR = 4LU	// error ( e.g. socket or network problem )
};

enum EncoderThreadStates
{
	ENCODER_NOT_RUNNING = 0LU,	// initial value - thread not startet yet
	ENCODER_ACTIVE = 1LU,	// the thread is collecting data from the scanner and stores them
	ENCODER_TERMINATE_REQUEST = 2LU,	// thread is requested to terminate
	ENCODER_TERMINATED = 3LU,	// thread is running no more
	ENCODER_ERROR = 4LU	// error ( e.g. socket or network problem )
};

class CDatenrecorder
{
public:	
		CDatenrecorder();
		~CDatenrecorder();
		int initSystemOverXML(char* FilePfad);
		int exitScanner();
		int SingleScan(std::vector<SLaserscan>* scanOut);
		int getActSingleScan(SLaserscan* scanOut,unsigned int uiScannerID);
		void StartRecording();
		void StopRecording();
		int getRecAllScanData(std::vector<std::list<SLaserscan>*>& vlAllData);
		int getRecScanData(std::list<SLaserscan>* liSLDataScanner, unsigned int uiScannerID);
		int getRecEncoderData(std::list<SEncoder>& lAllDataEncoder);
		int getScannerStatus(unsigned int uiScannerID);
		int getEncoderStatus();
		int getActEncorderPos(double& dActPos);

private: 
		 int initScanner(const char* ccPPfadScannerDLL, const char * ccIp, unsigned int uiPort, unsigned int& uiScannerID);
		 int initEncoder(const char* ccPPfadEncoderDLL, double dVel, char* ccCOMPort, int iBaud, char cParity, int iData, int iStop);
		 int getEncorderDataFromXML(char* cPfad, std::string& ccDLLPfad, std::string& sCOM, int& iVal, int& iBaud, std::string& sParity, int& iData, int& iStop);
		 int getScannerDataFromXML(char* cPfad, std::vector<ScannerInitParameter>& vScannerConfigData);
		 int StartScannerThread(int iScannerID);
		 int StartEncoderThread();
		 int StopScan();
		 int StopEncoder();
		 void StartScan();
		 void StartEncoder();
		 static void Thread_ScanRecord(CThread* pclThread);
		 static void Thread_EncoderRecord(CThread* pclThread);

private:
		 bool bRec;
		 bool bEncoderRec;
		 bool bIsInitEncoder;
		 bool bIsInitScanner;
		 bool bIsDataToGet;
		 int iScannerThreadState;
		 int iEncoderThreadState;
		 int iTimeOutCheckConnection;
		 int iAnzahlScanner;
		 int iAktScanID;
		 double dAktPos;
		 double dEncoderSpeed;
		 char* ccCOMPORT;
		 std::list<SPunkt> lcpEinE;
		 std::list<SEncoder> lEncoder;
		 std::vector<CScanner*> vobjScanner;
		 std::vector<ScannerIpState> vScannerState;
		 std::vector<CThread*> vobjThread_Record;
		 std::vector<std::list<SLaserscan>*> vlTmpScans;
		 LPCRITICAL_SECTION critListScanData;
		 LPCRITICAL_SECTION critListEncoderData;
		 CThread* objEncoderThread;
		 CEncoder* objEncoder;
};

#endif _DATENRECORDER_H_