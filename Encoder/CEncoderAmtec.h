#pragma once

class CEncoderAmtec
{
public:
	CEncoderAmtec( void );
	~CEncoderAmtec( void );


public:
	bool init( void );
	bool setMaxPos( double dMaxPos );
	bool setMinPos( double dMinPos );
	bool setVel( double dVel );
	bool setAcc( double dAcc );
	bool startMovement( void );
	bool stopMovement( void );
	bool getPos( double * pdPos );

private:
	// CAN Device ID
	int m_iDevID;

	// ID der Achse
	int m_iAxisID;

	// Vorwaerts = true
	// Rueckwaerts = false
	bool m_MoveDirection;
};


