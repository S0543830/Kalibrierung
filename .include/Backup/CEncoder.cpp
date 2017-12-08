#include "CEncoder.h"
//#include "EncoderGSP.h"

typedef bool(*iINITENCODER)(char*, int, char, int, int);
typedef bool(*iMETHHANDLE)();
typedef bool(*iSETVALUE)(double);
typedef bool(*iGETVALUE)(double*);

CEncoder::CEncoder()
{
	this->m_iState = 0;
}

CEncoder::~CEncoder()
{
	this->m_pEncoderPluginDll = NULL;
}

bool CEncoder::initEncoder(const char* ccFileDLL, char* cComPort, int iBaud, char cParity, int iData, int iStop)
{
	bool p_bReturn = false;

#ifdef UNICODE
	this->m_pEncoderPluginDll = LoadLibrary((LPCWSTR)ccFileDLL);
#else
	this->m_pEncoderPluginDll = LoadLibrary(ccFileDLL);
#endif // !UNICODE

	if (NULL != this->m_pEncoderPluginDll)
	{
		iINITENCODER pInitEncoder = (iINITENCODER)GetProcAddress(this->m_pEncoderPluginDll, "init");
		if (NULL != pInitEncoder)
		{
			p_bReturn = pInitEncoder(cComPort, iBaud, cParity, iData, iStop);
			this->m_ccPfadDll = ccFileDLL;
			this->m_cParameter = cComPort;
			this->m_iBaud = iBaud;
			this->m_cParity = cParity;
			this->m_iData = iData;
			this->m_iStop = iStop;
		}
	}
	return p_bReturn;
}

bool CEncoder::reconnectEncoder(void)
{
	bool p_bReturn = false;
	this->m_pEncoderPluginDll = NULL;

	p_bReturn = this->initEncoder(	this->m_ccPfadDll,
									this->m_cParameter,
									this->m_iBaud,
									this->m_cParity,
									this->m_iData,
									this->m_iStop );

	return p_bReturn;
}

bool CEncoder::startEncoder(void)
{
	bool p_bReturn = false;
	if (NULL != this->m_pEncoderPluginDll)
	{
		iMETHHANDLE pStartEncoder = (iMETHHANDLE)GetProcAddress(this->m_pEncoderPluginDll, "startMovement");
		if (NULL != pStartEncoder)
		{
			p_bReturn = pStartEncoder();
		}
	}

	return p_bReturn;
}

bool CEncoder::stopEncoder(void)
{
	bool p_bReturn = false;
	if (NULL != this->m_pEncoderPluginDll)
	{
		iMETHHANDLE pStopEncoder = (iMETHHANDLE)GetProcAddress(this->m_pEncoderPluginDll, "stopMovement");
		if (NULL != pStopEncoder)
		{
			p_bReturn = pStopEncoder();
		}
	}
	return p_bReturn;
}

bool CEncoder::setSpeed(double dVel)
{
	bool p_bReturn = false;
	if (NULL != this->m_pEncoderPluginDll)
	{
		iSETVALUE pSetSpeed = (iSETVALUE)GetProcAddress(this->m_pEncoderPluginDll, "setVel");
		if (NULL != pSetSpeed)
		{
			p_bReturn = pSetSpeed(dVel);
		}
	}
	return p_bReturn;
}

bool CEncoder::setSpeedUp(double dAcc)
{
	bool p_bReturn = false;
	if (NULL != this->m_pEncoderPluginDll)
	{
		iSETVALUE pSetSpeedUp = (iSETVALUE)GetProcAddress(this->m_pEncoderPluginDll, "setAcc");
		if (NULL != pSetSpeedUp)
		{
			p_bReturn = pSetSpeedUp(dAcc);
		}
	}
	return p_bReturn;
}

bool CEncoder::getPosition(double& pdPos)
{
	bool p_bReturn = false;
	if (NULL != this->m_pEncoderPluginDll)
	{
		iGETVALUE pGetPosition = (iGETVALUE)GetProcAddress(this->m_pEncoderPluginDll, "getPos");
		if (NULL != pGetPosition)
		{
			p_bReturn = pGetPosition(&pdPos);
		}
	}
	return p_bReturn;
}

bool CEncoder::moveToPos(double pos)
{
	bool p_bReturn = false;
	if (NULL != this->m_pEncoderPluginDll)
	{
		iSETVALUE pMoveToPos = (iSETVALUE)GetProcAddress(this->m_pEncoderPluginDll, "moveToPos");
		if (NULL != pMoveToPos)
		{
			p_bReturn = pMoveToPos(pos);
		}
	}
	return p_bReturn;
}

bool CEncoder::setState(int iState)
{
	this->m_iState = iState;
	return true;
}

int CEncoder::getState()
{
	return this->m_iState;
}


