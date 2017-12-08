#include "stdafx.h"
#include "CProgControl.h"

#include "3DCalculation.h"

#include <3DLibCpp\3DBasic\3Dtypes.h>
#include <3DLibCpp\DATA_OBJ\C3DObjectTypes.h>
#include <3DLibCpp\DATA_OBJ\C3DObjectIO.h>
#include <3DLibCpp/DATA_OBJ/C3DPointObj.h>

CProgControl::CProgControl()
{
	this->objDatRec = new CDatenrecorder();
}

CProgControl::~CProgControl()
{

}

int CProgControl::initSystem(std::string sXMLDatei)
{
	int iVal = 0;
	iVal = this->objDatRec->initSystemOverXML(((char*)sXMLDatei.c_str()));
	if (0 != iVal)
	{
		return ERR_PROGCONTROL_ERROR;
	}
	return PROGCONTROL_OK;
}

int CProgControl::startRecord()
{
	int iVal = 0; 

	this->objDatRec->StartRecording();
	//this->objDatRec->getScannerStatus(0);

	return PROGCONTROL_OK;
}

int CProgControl::stopRecord()
{
	this->objDatRec->StopRecording();
	double dEncRot[3] = { 0, 0, 0 };
	double dEncTran[3] = { -1500, 0, 0 };
	double dScaRot[3] = { 0, 90, 0 };
	double dScaTran[3] = { 0, 0, 0 };
	//C3DCalc obj3D;
	//std::list<SPunkt> liOut = obj3D.Calc3DCPunkt(liScan,liPos, dEncRot, dEncTran, dScaRot, dScaTran);
	
	return PROGCONTROL_OK;
}

C3DPointObj* CProgControl::getScanData(int iScannerID)
{
	C3DCalc obj3D;
	std::list<SLaserscan> liScan;

	//EncoderDaten dieses Scans holen fall keine da sind
	if (true == this->liEncDat.empty() )
	{
		this->objDatRec->getRecEncoderData(this->liEncDat);
	}

	this->objDatRec->getRecScanData(&liScan, 0);

//Temporär Transformationsparameter
	double daEnR[3] = { 0, 0, 0 };
	double daEnT[3] = { -1500, 0, 0 };
	double daScR[3] = { 0, 90, 0 };
	double daScT[3] = { 0, 0, 0 };

	stringstream sDeb;
	sDeb << "Enc: " << liEncDat.size() << std::endl;
	sDeb << "Sca: " << liScan.size() << std::endl;
	OutputDebugStringA(sDeb.str().c_str());

	return  obj3D.Calc3DObj(liScan, this->liEncDat, daEnR, daEnT, daScR, daScT);
}