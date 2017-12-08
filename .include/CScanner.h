#ifndef _CSCANNER_H_
#define _CSCANNER_H_
#include <list>
#include "DataTypes.h"
#include <Windows.h>

class CScanner
{
public:
	CScanner();
	~CScanner();
	int initScanner(const char* ccFileDLL, const char* ccIp, unsigned int uiPort);
	int reconnectScanner();
	int exitScanner();
	int startRecording();
	int stopRecording();
	int getRecordedPoints(std::list<SPunkt>& lPunkte, std::list<double>& ldZeiten);
	int getFilterRecordedPoints(std::list<SPunkt>& lPunkte, std::list<double>& ldZeiten, unsigned int uiAnzahl, unsigned int iMode);
	int getSingleScan(std::list<SPunkt>& lPunkt, std::list<double>& ldZeiten);
	int setState(int iState);
	int getState();
private:
	unsigned int m_iHandle;
	HMODULE m_pRotoPluginDll;
	const char* m_ccIP;
	unsigned int m_uiPort;
	const char* m_cDLLPfad;
	int m_iState;
};

#endif _CSCANNER_H_