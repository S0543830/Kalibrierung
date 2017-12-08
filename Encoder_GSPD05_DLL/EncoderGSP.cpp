// Encoder.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "EncoderGSP.h"
#include "CEncoderGSP.h"

CEncoderGSP* pEncoder = nullptr;

ENCODER_GSP_API bool init(char* cComPort, int iBaud, char cParity, int iData, int iStop)
{
	if (nullptr == pEncoder)
	{
		pEncoder = new CEncoderGSP();

		if (nullptr != pEncoder)
		{
			if (true == pEncoder->init(cComPort, iBaud, cParity, iData,iStop))
			{
				//pEncoder->negInit();
				return true;
			}
		}
	}

	return false;
}

ENCODER_GSP_API bool startMovement(void)
{
	return pEncoder->startMovement();
}
ENCODER_GSP_API bool stopMovement(void)
{
	return false;
}
ENCODER_GSP_API bool moveToPos(double pos)
{
	return pEncoder->moveToPos(pos);
}
ENCODER_GSP_API bool setVel(double dVel)
{
	return pEncoder->setSpeed( dVel );
}
ENCODER_GSP_API bool setAcc(double dAcc)
{
	return false;
}
ENCODER_GSP_API bool getPos(double * pdPos)
{
	double ulTmp = 0;
	bool bRet = pEncoder->getPos(&ulTmp);
	*pdPos = /*static_cast<double>*/(ulTmp);
	return bRet;
}

ENCODER_GSP_API void close(void)
{
	if (nullptr != pEncoder)
	{
		pEncoder->cleanUp();
		delete pEncoder;
		pEncoder = nullptr;
	}
}

