#ifndef __C3DCALC_H_INCLUDED__
#define __C3DCALC_H_INCLUDED__
#include "DataTypes.h"
#include "DebugOutput.h"



class C3DPointObj;

class C3DCalc
{
public:
	C3DCalc();
	~C3DCalc();
	/********************************************************************
	Erstellt:		Juli 2017   
	Autor:			M. Wolff
	Beschreibung: Umrechnung von Ebenenscans und Encoderwerte in 3D-Punkte
	Parameter:
	IN std::list<SLaserscan> liScans - voll qualifizierter Dateiname
	IN std::list<SEncoder> liPos -, wird der Datei entnommen
	IN double adHanConRot[3] -
	IN double adHanConTrans[3] -
	IN double adScannRot[3] -
	IN double adScannTrans[3] -
	Rueckgabe:
	list<SPunkt> - gefüllt mit berechneten 3D-Punkten
	*********************************************************************/
	std::list<SPunkt> Calc3DCPunkt(	std::list<SLaserscan> liScans,
								std::list<SEncoder> liPos,
								double adHanConRot[3],
								double adHanConTrans[3],
								double adScannRot[3],
								double adScannTrans[3] );

	/********************************************************************
	Erstellt:		Juli 2017
	Autor:			M. Wolff
	Beschreibung: Umrechnung von Ebenenscans und Encoderwerte in 3D-Objekt
	Parameter:
	IN std::list<SLaserscan> liScans - voll qualifizierter Dateiname
	IN std::list<SEncoder> liPos -, wird der Datei entnommen
	IN double adHanConRot[3] -
	IN double adHanConTrans[3] -
	IN double adScannRot[3] -
	IN double adScannTrans[3] -
	Rueckgabe:
	C3DPointObj* - 3D-Objekt gefüllt mit berechneten 3D-Punkten
	*********************************************************************/
	C3DPointObj* Calc3DObj(	std::list<SLaserscan> liScans,
							std::list<SEncoder> liPos,
							double adHanConRot[3],
							double adHanConTrans[3],
							double adScannRot[3],
							double adScannTrans[3]);

	/********************************************************************
	Erstellt:		Juli 2017
	Autor:			M. Wolff
	Beschreibung: Umrechnung von Ebenenscans und Encoderwerte in 3D-Punkte
	Parameter:
	IN std::list<SPunkt> liPunkte - voll qualifizierter Dateiname
	IN string sDatName -, wird der Datei entnommen
	Rueckgabe:
	int - gefüllt mit berechneten 3D-Punkten
	*********************************************************************/
	int Save3D(std::list<SPunkt> liPunkte, string sDatName);


private:
	/********************************************************************
	Erstellt:		Juli 2017
	Autor:			M. Wolff
	Beschreibung: Umrechnung von Ebenenscans und Encoderwerte in 3D-Punkte
	Parameter:
	IN std::list<SEncoder>::iterator itAkt - voll qualifizierter Dateiname
	IN std::list<SEncoder>::iterator itBefor 
	In double dZeitpunkt
	Rueckgabe:
	double - gefüllt mit berechneten 3D-Punkten
	*********************************************************************/
	double interpolationPosition(	std::list<SEncoder>::iterator itAkt,
									std::list<SEncoder>::iterator itBefor,
									double dZeitpunkt );
private:

	double m_dFaktor;
	CDebugOutput* m_objDebug;

};




#endif