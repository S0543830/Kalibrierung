#include "CEncoderAmtec.h"
#include "m5api.h"
#include <math.h>


CEncoderAmtec::CEncoderAmtec() :
	m_MoveDirection( false ),
	m_iAxisID( 11 )
{

}

CEncoderAmtec::~CEncoderAmtec( void )
{	
	// Achsen anhalten
	PCube_haltAll( m_iDevID );

	PCube_closeDevice( m_iDevID );
}

bool CEncoderAmtec::init()
{
	bool bRet = false;

	int iRet = PCube_openDevice( &m_iDevID, "ESD:0,250" );

	if ( iRet == 0 )
	{
		PCube_resetAll( m_iDevID );

		// Modulestates abfragen
		unsigned long lState1;
		PCube_getModuleState( m_iDevID, m_iAxisID, &lState1 );

		// Ueberrpruefen ob das Module schon auf Home gefahren wurde
		if ( !( lState1 & STATEID_MOD_HOME ) )
		{
			// auf Home fahren
			PCube_homeModule( m_iDevID, m_iAxisID );
		}

		bRet = true;
	}

	return bRet;
}

bool CEncoderAmtec::setMaxPos( double dMaxPos )
{
	float fValue = static_cast<float>( dMaxPos );
	int iRet = PCube_setMaxPos( m_iDevID, m_iAxisID, fValue );
	if ( iRet == 0 )
	{
		return true;
	}

	return false;
}

bool CEncoderAmtec::setMinPos( double dMinPos )
{
	float fValue = static_cast<float>( dMinPos );
	int iRet = PCube_setMinPos( m_iDevID, m_iAxisID, fValue );
	if ( iRet == 0 )
	{
		return true;
	}

	return false;
}

bool CEncoderAmtec::setVel( double dVel )
{
	float fValue = static_cast<float>( dVel );

	if ( 0 > fValue )
	{
		// Negative Geschwindigkeit rueckwaerts
		m_MoveDirection = false;
		fValue = fabsf( fValue );
	}
	else
	{
		// Positive Geschwindigkeit vorwaerts
		m_MoveDirection = true;
	}

	int iRet = PCube_setRampVel( m_iDevID, m_iAxisID, fValue );
	if ( iRet == 0 )
	{
		return true;
	}

	return false;
}

bool CEncoderAmtec::setAcc( double dAcc )
{
	float fValue = static_cast<float>( dAcc );
	int iRet = PCube_setRampAcc( m_iDevID, m_iAxisID, fValue );
	if ( iRet == 0 )
	{
		return true;
	}

	return false;
}

bool CEncoderAmtec::startMovement( void )
{
	float fTargetPos;
	int iRet = -1;
	bool bRet = false;

	if ( false == m_MoveDirection )
	{
		iRet = PCube_getMaxPos( m_iDevID, m_iAxisID, &fTargetPos );
	}
	else
	{
		iRet = PCube_getMinPos( m_iDevID, m_iAxisID, &fTargetPos );
	}

	if ( 0 == iRet )
	{
		iRet = PCube_movePos( m_iDevID, m_iAxisID, fTargetPos );

		if ( iRet == 0 )
		{
			bRet = true;
		}
	}

	return bRet;

}

bool CEncoderAmtec::stopMovement( void )
{
	// Achsen anhalten
	PCube_haltAll( m_iDevID );
	PCube_resetAll( m_iDevID );
	return true;
}

bool CEncoderAmtec::getPos( double * pdPos )
{
	float fValue;
	int iRet = PCube_getPos( m_iDevID, m_iAxisID, &fValue );
	if ( iRet == 0 )
	{
		*pdPos = fValue;
		return true;
	}

	return false;
}

