#ifndef __CPROGCONTROL_H_INCLUDED__
#define __CPROGCONTROL_H_INCLUDED__
#include "Datenrecorder.h"
class C3DPointObj;

enum CProgControlReturnValue
{
	PROGCONTROL_OK = 0LU,	// no error
	ERR_PROGCONTROL_ERROR = 3LU,	// 
};

class CProgControl
{
public:
	CProgControl();
	~CProgControl();

	int initSystem(std::string sXMLDatei);
	int startRecord();
	int stopRecord();

	C3DPointObj* getScanData(int iScannerID);

private:

	CDatenrecorder* objDatRec;

	std::list<SEncoder> liEncDat;


};

#endif
