#ifndef __CENCGSPC_H_INCLUDED__
#define __CENCGSPC_H_INCLUDED__

class CEncoderGSP
{
public:
	CEncoderGSP(void);
	~CEncoderGSP(void);


public:
	bool init( char* cComPort, int iBaud, char cParity, int iData, int iStop);
	bool cleanUp(void);
	bool negInit(void);
	bool startMovement(void);	
	bool getPos(unsigned long *pos);
	bool getPos(double *pos);
	bool getPos_steps(unsigned long *pos);
	bool setSpeed(double speed);
	bool moveToPos(double pos);
private:
	bool m_MoveDirection;
	unsigned long dEnd;
	unsigned long dBeginn;

	bool setAbsPos(double pos_mm, bool bBlock);
	bool setRelPos(double pos_rel_mm);
	bool getMaxPos(unsigned long *pos);

	unsigned long GenCom(char *command, char *data);
	unsigned char CalcCheckSum(char *data, unsigned long len);
	unsigned char CharToNibble(char c);
	char NibbleToChar(unsigned char nibble);
	int Analyse(char* answ, char* data, unsigned char* status);
	
	char        axis;
	double      Incr_pro_mm;         // Incremente / mm
	void*      HCom;
	unsigned long	   MaxStep;
	unsigned long	   MaxFreq;
	double		ActSpeed;
};

#endif