#ifndef _CKALIBRIERUNG_H_
#define _CKALIBRIERUNG_H_
#include "DataTypes.h"
#include "CEncoder.h"
#include "CScanner.h"
#include "XMLParameter.h"
#include <list>

struct StructGeradengleichung
{
	double dSteigung;
	double dSchnittPunkt;
	double dKorreleationskoeffiezient;
	double dStartwertX;
	double dStartWertY;
	double dEndWertX;
	double dEndWertY;
};

struct RotWinkel
{
	double dRotX;
	double dRotY;
	double dRotZ;
};

struct structMainRregression
{
	double dLaenge;
	StructGeradengleichung strgerade;
	SPunkt cpStartpunkt;
	SPunkt cpEndpunkt;
};

struct structScheidelpunkt
{
	double x;
	double y;
};

struct structParameterGerade
{
	double zahl1;
	double zahl2;
	double zahl3;
};

struct ScannerInitParameter
{
	std::string ccDLLPfad;
	std::string sIP;
	int iPort;
	int iScannerID;
	int iAnzahlScanner;
};

struct StructGeradenPunkte
{
	std::list<SPunkt> lcpGeradenPunkte;
	StructGeradengleichung sgGerade;
};

enum EncoderThreadStates
{
	ENCODER_NOT_RUNNING = 0LU,	// initial value - thread not startet yet
	ENCODER_ACTIVE = 1LU,	// the thread is collecting data from the scanner and stores them
	ENCODER_TERMINATE_REQUEST = 2LU,	// thread is requested to terminate
	ENCODER_TERMINATED = 3LU,	// thread is running no more
	ENCODER_ERROR = 4LU	// error ( e.g. socket or network problem )
};

class CKalibrierung
{
public: 
	CKalibrierung();
	~CKalibrierung();
	int startKalibrierung(std::vector<std::list<SLaserscan>*> vlScan, std::vector<SKalibrierung>& vSKalibrierungOut, char cScanRichtung, int EncoderSteps);
	int filterBodyFromScan(std::vector<std::list<SLaserscan>*> vlScan, std::vector<std::list<SLaserscan>>& vlFilteredScan);
	

private: 
	int interpolation(SPunkt A, SPunkt B, int iAnzahl, std::list<SPunkt>& lOut);
	bool isPointInList(std::list<SPunkt> lListIn, SPunkt A);
	bool isPointInList(std::list<SLaserscan> lListIn, SPunkt A); 
	int filterBody(std::list<SLaserscan> lScanIn, std::list<SLaserscan>& lScanOut);
	int setReferenceSystem();
	int filterObject(std::vector<std::list<SLaserscan>*> vlScanWithObject, std::vector<std::list<SLaserscan>*> vlScanWithoutObject,
		/*std::list<SEncoder> lEncoderWithoutObject, std::list<SEncoder> lEncoderWithObject,*/ std::vector<std::list<SLaserscan>>& vlFilteredScan);
	static double calculateGradeOfLine(SPunkt A, SPunkt B);
	//int ermittleRegressionsGeraden(std::vector<std::list<SLaserscan>*> vlslScanIn, std::vector<std::list<std::list<std::list<StructGeradenPunkte>>>>& vlslRegOut, char cScanRichtung);
	int calculateZenith(structParameterGerade paraIn, structScheidelpunkt& paraOut);
	int rotWinkelBerechnen(std::vector<std::list<std::list<StructGeradenPunkte>>> vRegIn, std::vector<SKalibrierung>& vRotWinkel);
	int setComponentSystem(std::vector<std::list<SLaserscan>> vScanIn, char cScanRichtung);
	double calculateAngleOfIntersection(StructGeradengleichung streightLineA, StructGeradengleichung streightLineB);
	double calculateAngleOfIntersection(double dSteigungA, double dSteigungB);
	int ermittleRegressionsGeraden(std::list<SLaserscan>::iterator lScanIn, std::list<SLaserscan> lOut, int iMaxSize);
	int ermittleRegressionsGeraden(std::vector<std::list<SLaserscan>*> vlslScanIn, std::vector<std::list<std::list<StructGeradenPunkte>>>& vlRegOut);
	int berechneRegGeraden(std::list<SPunkt> lPointsIn, StructGeradengleichung& sgGeradeOut, char cScanRichtung);
	int berechneRegGeraden(std::list<SPunkt> lPointsIn, StructGeradengleichung& sgGeradeOut) const;
	//int rotWinkelBerechnen(std::vector<std::list<std::list<std::list<StructGeradenPunkte>>>> vRegIn, std::vector<SKalibrierung>& vRotWinkel, char cScanRichtung );
	int berechneLotfussppunkt(StructGeradengleichung gerade, SPunkt& Start, SPunkt& End) const;
	int getEncorderDataFromXML(char* cPfad, std::string& ccDLLPfad, std::string& sCOM, int& iVal, int& iBaud, std::string& sParity, int& iData, int& iStop);
	int getScannerDataFromXML(char* cPfad, std::vector<ScannerInitParameter>& vScannerConfigData);
	int getParaDataFromXML(char* cPfad, int& iTiefe, int& iHoehe, int& iBreite, int& iAnzahl);
	double berechnerDistanz(SPunkt A, SPunkt B) const;
	SPunkt calculateIntersectionPoint(StructGeradengleichung gerade1, StructGeradengleichung gerade2);
	int initEncoder(const char* ccPPfadEncoderDLL, double dVel, char* cCOMPort, int iBaud, char cParity, int iData, int iStop);
	int initScanner(const char* ccPPfadScannerDLL, const char * ccIp, unsigned int uiPort, unsigned int& uiScannerID);
	int getFilteredPoitns(std::vector<std::list<SLaserscan>>* scanOut, unsigned int filter, unsigned int iMode);
	bool moveEncoder(double pos);
	double getPosEncoder();
	
private:
	std::vector<SPunkt> m_OriginComponentSystem;
	double PI;
	CEncoder* m_objEncoder;
	std::vector<CScanner*> vobjScanner;
	int iAnzahlScanner;
	bool bIsInitScanner;
	bool bEncoderRec;
	double moveToPos;
	char* cParaPfad;

};

#endif _CKALIBRIERUNG_H_