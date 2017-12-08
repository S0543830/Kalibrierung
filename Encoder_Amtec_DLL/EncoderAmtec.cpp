// Encoder.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include "EncoderAmtec.h"
#include "CEncoderAmtec.h"

// Globales Klassenelement
CEncoderAmtec* pEncoder = nullptr;

ENCODER_AMTEC_API bool init( InitParameter const * pInitParam )
{

	if ( nullptr == pEncoder )
	{
		pEncoder = new CEncoderAmtec();

		if ( nullptr != pEncoder )
		{
			if ( true == pEncoder->init() )
			{
				pEncoder->setMaxPos( pInitParam->dMaxPos );
				pEncoder->setMinPos( pInitParam->dMinPos );
				pEncoder->setVel( pInitParam->dVel );
				pEncoder->setAcc( pInitParam->dAcc );

				return true;
			}
		}
	}

	return false;
}
ENCODER_AMTEC_API bool startMovement( void )
{
	return pEncoder->startMovement();
}
ENCODER_AMTEC_API bool stopMovement( void )
{
	return pEncoder->stopMovement();
}
ENCODER_AMTEC_API bool setVel( double dVel )
{
	return pEncoder->setVel( dVel );
}
ENCODER_AMTEC_API bool setAcc( double dAcc )
{
	return pEncoder->setAcc( dAcc );
}
ENCODER_AMTEC_API bool getPos( double * pdPos )
{
	return pEncoder->getPos( pdPos );
}
ENCODER_AMTEC_API void close( void )
{
	if ( nullptr != pEncoder )
	{
		delete pEncoder;
		pEncoder = nullptr;
	}
}

