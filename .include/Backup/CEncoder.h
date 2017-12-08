#ifndef _CENCODER_H_
#define _CENCODER_H_
#include "EncoderGSP.h"
#include <Windows.h>
using namespace std;

class CEncoder
{
public: CEncoder();
		~CEncoder();
		bool initEncoder(const char* ccFileDLL, char* IPCParameter, int iBaud, char cParity, int iData, int iStop);
		bool reconnectEncoder(void);
		bool startEncoder(void);
		bool stopEncoder(void);
		bool setSpeed(double dVel);
		bool setSpeedUp(double dAcc);
		bool getPosition(double& pdPos);
		bool moveToPos(double pos);
		bool setState(int iState);
		int getState();
private:
	HMODULE m_pEncoderPluginDll;
	char* m_cParameter;
	const char* m_ccPfadDll;
	int m_iState;
	int m_iBaud;
	char m_cParity;
	int m_iData;
	int m_iStop;
};
#endif _CENCODER_H_