#include "CEncoderGSP.h"
#include "ComDev.h"

#include <Winsock2.h>
#include <windows.h>
#include <wchar.h>
#include <stdio.h>
#include <math.h>
#include <string>


CEncoderGSP::CEncoderGSP(void)
{
	this->axis = '1';
	this->Incr_pro_mm = 67.264573;
	this->dEnd = 0;
	this->dBeginn = 320;
}

CEncoderGSP::~CEncoderGSP(void)
{

}

bool CEncoderGSP::init(char* cComPort, int iBaud, char cParity, int iData, int iStop)
{
	unsigned long   ComLen = 0;
	char     data[128] = { 0 };
	char     buf[128] = { 0 };
	unsigned char     status = 0;

	std::string s = "baud=" + std::to_string(iBaud) + " parity="+ cParity+ " data="+ std::to_string(iData)+" stop="+ std::to_string(iStop);
	HCom = COMDEV_Init(cComPort, (char*) s.c_str(),100);

	if (HCom == INVALID_HANDLE_VALUE)
		return false;

	//Parameter erfragen
	//Achsenbegrenzung
	ComLen = GenCom("PG?", buf);

	COMDEV_Write(HCom, buf, ComLen);
	if (COMDEV_Read(HCom, buf, sizeof(buf)) == 0) {
		COMDEV_Close(HCom);
		return false;
	}

	if (Analyse(buf, data, &status) == FALSE) {
		COMDEV_Close(HCom);
		return false;
	}

	MaxStep = atol(data);


	//Maximale Frequenz
	ComLen = GenCom("IF?", buf);

	COMDEV_Write(HCom, buf, ComLen);
	if (COMDEV_Read(HCom, buf, sizeof(buf)) == 0) 
	{
		COMDEV_Close(HCom);
		return false;
	}

	if (Analyse(buf, data, &status) == FALSE) 
	{
		COMDEV_Close(HCom);
		return false;
	}

	MaxFreq = atol(data);

	//Parameter setzen
	//Lauffrequenz
	ComLen = GenCom("PF1000", buf);

	COMDEV_Write(HCom, buf, ComLen);
	COMDEV_Read(HCom, buf, sizeof(buf));

	//Offset vom Neg Initiator
	ComLen = GenCom("PM50", buf);

	COMDEV_Write(HCom, buf, ComLen);
	COMDEV_Read(HCom, buf, sizeof(buf));

	//Offset vom Pos Initiator
	ComLen = GenCom("PP50", buf);

	COMDEV_Write(HCom, buf, ComLen);
	COMDEV_Read(HCom, buf, sizeof(buf));

	//Status-Abfrage
	ComLen = GenCom("IS?", buf);

	COMDEV_Write(HCom, buf, ComLen);
	COMDEV_Read(HCom, buf, sizeof(buf));


	Analyse(buf, data, &status);
	if ((status & 0x79) != 0) 
	{
		COMDEV_Close(HCom);
		return false;
	}

	this->getMaxPos(&this->dEnd);
	this->dEnd -= 350;

	return true;
}

bool CEncoderGSP::cleanUp( )
{
	if (this->HCom != INVALID_HANDLE_VALUE)
	{
		int iTmp = COMDEV_Close(this->HCom);
		if (TRUE == iTmp)
		{
			return(true);
		}
	}	

	return (false);
}

bool CEncoderGSP::negInit(void) 
{
	unsigned long   ComLen;
	char     data[128] = { 0 };
	char     buf[128] = { 0 };
	unsigned char     status = 0;

	ComLen = GenCom("GI-", buf);
	COMDEV_Write(HCom, buf, ComLen);
	if (COMDEV_Read(HCom, buf, sizeof(buf)) == 0)
		return false;

	do {
		ComLen = GenCom("PC?", buf);
		COMDEV_Write(HCom, buf, ComLen);
		COMDEV_Read(HCom, buf, sizeof(buf));
		Analyse(buf, data, &status);
	} while (status & 0x01);

	return true;
}

bool CEncoderGSP::startMovement(void)
{
	bool bRet = false;

	if( false == m_MoveDirection )
	{
		bRet = this->setAbsPos(this->dEnd, false);
	}
	else
	{
		bRet = this->setAbsPos(this->dBeginn, false);
	}

	return bRet;

}

bool CEncoderGSP::setAbsPos(double pos_mm, bool bBlock)
{
	unsigned long   ComLen;
	char     command[128] = { 0 };
	char     buf[128] = { 0 };
	unsigned char     status = 0;
	unsigned long    pos;

	//Positionsangabe umwandeln
	pos = (unsigned long)(pos_mm * Incr_pro_mm + 0.5);

	if (pos > MaxStep) {
		//MessageBox(NULL, "ungültige Positionsangabe!!", "Fehler", MB_OK);
		return FALSE;
	}//if()

	sprintf_s(command, 128, "GA%d", pos);
	ComLen = GenCom(command, buf);
	COMDEV_Write(HCom, buf, ComLen);
	COMDEV_Read(HCom, buf, sizeof(buf));
	
	if (true == bBlock)
	{
		ComLen = GenCom("PC?", command);
		do 
		{
			COMDEV_Write(HCom, command, ComLen);
			COMDEV_Read(HCom, buf, sizeof(buf));
			Analyse(buf, NULL, &status);
		} while (((status & 0x01) == 1) && ((status & 0x78) == 0));
	}


	if ((status & 0x78) > 0) {
		ComLen = GenCom("IS?", buf);
		COMDEV_Write(HCom, buf, ComLen);
		COMDEV_Read(HCom, buf, sizeof(buf));
		//MessageBox(NULL, "Fehler sind aufgetreten", "Fehler", MB_OK);
		//MessageBox(NULL, buf, "Fehlercode", MB_OK);
		return FALSE;
	}//if

	return TRUE;

}

bool CEncoderGSP::setRelPos(double pos_rel_mm)
{
	unsigned long   ComLen;
	char     command[128] = { 0 };
	char     buf[128] = { 0 };
	unsigned char     status = 0;
	unsigned long    pos;


	getPos_steps(&pos);
	pos += (unsigned long)(pos_rel_mm * Incr_pro_mm + 0.5);
	if (pos < 0 || pos > MaxStep) {
		//MessageBox(NULL, "ungültige Position", "Fehler", MB_OK);
		return FALSE;
	}//if

	 //Positionsangabe umwandeln
	if (pos_rel_mm < 0) {
		pos = (unsigned long)(pos_rel_mm*(-1) * Incr_pro_mm + 0.5);
		sprintf_s(command, 128, "GR-%d", pos);
	}
	else {
		pos = (unsigned long)(pos_rel_mm * Incr_pro_mm + 0.5);
		sprintf_s(command, 128, "GR+%d", pos);
	}//if() else

	ComLen = GenCom(command, buf);
	COMDEV_Write(HCom, buf, ComLen);
	COMDEV_Read(HCom, buf, sizeof(buf));
	Analyse(buf, NULL, &status);
	if (status & 0x78) {
		//MessageBox(NULL, "Fehler", "Fehler", MB_OK);
		return FALSE;
	}//if()

	ComLen = GenCom("PC?", command);
	do {
		COMDEV_Write(HCom, command, ComLen);
		COMDEV_Read(HCom, buf, sizeof(buf));
		Analyse(buf, NULL, &status);
	} while (((status & 0x01) == 1) && ((status & 0x78) == 0));

	if ((status & 0x78) > 0) {
		ComLen = GenCom("IS?", buf);
		COMDEV_Write(HCom, buf, ComLen);
		COMDEV_Read(HCom, buf, sizeof(buf));
		//MessageBox(NULL, "Fehler sind aufgetreten", "Fehler", MB_OK);
		//MessageBox(NULL, buf, "Fehlercode", MB_OK);
		return FALSE;
	}//if

	return TRUE;

}//int POSITION_SetRelPos()

bool CEncoderGSP::setSpeed(double speed)
{
	unsigned long ulSpeed = 0;

	unsigned long   ComLen;
	char     command[128] = { 0 };
	char     buf[128] = { 0 };
	unsigned char     status = 0;

	if (0 > speed)
	{
		m_MoveDirection = true;
		ulSpeed = static_cast<unsigned long>(fabsf(static_cast<float>(speed)));
	}
	else
	{
		m_MoveDirection = false;
		ulSpeed = static_cast<unsigned long>(fabsf(static_cast<float>(speed)));

	}

	if ((ulSpeed > MaxFreq) || (ulSpeed < 10))
	{
		
		return FALSE;
	}

	sprintf_s(command, 128, "PF%d", ulSpeed);
	ComLen = GenCom(command, buf);
	COMDEV_Write(HCom, buf, ComLen);
	COMDEV_Read(HCom, buf, sizeof(buf));
	Analyse(buf, NULL, &status);
	if (status != 0x00)
	{
		
		return FALSE;
	}
	ActSpeed = speed;
	return TRUE;
}

bool CEncoderGSP::moveToPos(double pos)
{	
	return setRelPos(pos);
}
//int POSITION_SetSpeed()

bool CEncoderGSP::getPos_steps(unsigned long *pos)
{
	unsigned long   ComLen;
	char     data[128] = { 0 };
	char     buf[128] = { 0 };
	unsigned char     status = 0;

	ComLen = GenCom("PC?", buf);
	COMDEV_Write(HCom, buf, ComLen);
	COMDEV_Read(HCom, buf, sizeof(buf));
	Analyse(buf, data, &status);
	if (status != 0x00)
		return FALSE;

	*pos = atol(data);

	return TRUE;
}//int POSITION_GetPos_steps()

bool CEncoderGSP::getPos(unsigned long *pos)
{
	unsigned long   ComLen;
	char     data[128] = { 0 };
	char     buf[128] = { 0 };
	unsigned char     status = 0;

	ComLen = GenCom("PC?", buf);
	COMDEV_Write(HCom, buf, ComLen);
	COMDEV_Read(HCom, buf, sizeof(buf));
	Analyse(buf, data, &status);
	*pos = (unsigned long)(atol(data) / Incr_pro_mm + 0.5);
	if (status != 0x00)
		return FALSE;

	

	return TRUE;
}//int POSITION_GetPos()

bool CEncoderGSP::getPos(double *pos)
{
	unsigned long   ComLen;
	char     data[128] = { 0 };
	char     buf[128] = { 0 };
	unsigned char     status = 0;

	ComLen = GenCom("PC?", buf);
	COMDEV_Write(HCom, buf, ComLen);
	COMDEV_Read(HCom, buf, sizeof(buf));
	Analyse(buf, data, &status);
	*pos = (double)(atol(data) / Incr_pro_mm );
	if (status != 0x00)
		return FALSE;



	return TRUE;
}


bool CEncoderGSP::getMaxPos(unsigned long *pos)
{
	*pos = (unsigned long)(MaxStep / Incr_pro_mm + 0.5);

	return TRUE;
}//int POSITION_GetMaxPos()


unsigned char CEncoderGSP::CalcCheckSum(char *data, unsigned long len) 
{
	unsigned char  checksum = 0;
	WORD  i;

	for (i = 0; i < len; i++) {
		checksum ^= (unsigned char)data[i];
	}//for()

	return(checksum);
}//unsigned char CalcCheckSum()

unsigned long CEncoderGSP::GenCom(char *command, char *data) 
{
	int  comlen;
	unsigned char   cs;

	comlen = static_cast<int>(strlen(command));

	data[0] = 0x02;
	data[1] = axis;

	memcpy(data + 2, command, comlen);

	data[comlen + 2] = ':';
	data[comlen + 3] = 'X';
	data[comlen + 4] = 'X';
	data[comlen + 5] = 0x03;
	data[comlen + 6] = '\0';

	cs = CalcCheckSum(data + 1, comlen + 2);

	data[comlen + 3] = NibbleToChar(cs >> 4);
	data[comlen + 4] = NibbleToChar(cs & 0x0F);

	return(comlen + 6);
}//unsigned long GenCom()

char CEncoderGSP::NibbleToChar(unsigned char nibble)
{
	char tmp;

	if (nibble > 9)
		tmp = nibble + 0x37;
	else
		tmp = nibble + 0x30;

	return tmp;
}//char NibbleToChar()

unsigned char CEncoderGSP::CharToNibble(char c)
{
	if (c > 0x40)
		return c - 0x37;
	else
		return c - 0x30;
}//unsigned char CharToNibble1()

int CEncoderGSP::Analyse(char* answ, char* data, unsigned char* status) 
{
	char   axis = '1';

	unsigned long   start = 0xFFFF;
	unsigned long   end = 0;
	unsigned long i;
	unsigned char   cs = 0;

	*status = 0;
	//Antwortstring im Gesamtstring suchen
	for (i = 0; i < strlen(answ); i++)
		if (answ[i] == 0x02) {
			start = i + 1;
			break;
		}//if()
	if (start == 0xFFFF) return false;

	for (i = start; i < strlen(answ); i++)
		if (answ[i] == 0x03) {
			end = i - 1;
			break;
		}//if()

		 //Plausibilitätsprüfung
	if (end == 0) return FALSE;
	if (answ[start] != axis) return FALSE;
	if ((end - start) < 6) return FALSE;



	//Datenfeld kopieren
	if (data != NULL) {
		memset(data, 0, strlen(data));
		if (answ[start + 4] != ':')
			memcpy(data, answ + start + 4, end - start - 6);
	}//if()

	 //Checksumme extrahieren
	cs = CharToNibble(answ[end - 1]);
	cs = cs << 4;
	cs += CharToNibble(answ[end]);

	//Checksumme aus String errechnen und vergleichen
	if (cs != CalcCheckSum(answ + start, end - start - 1))
		return FALSE;

	//Kurz-Status extrahieren
	*status = CharToNibble(answ[start + 1]);
	*status = *status << 4;
	*status += CharToNibble(answ[start + 2]);

	return TRUE;
}//bool Analyse()
