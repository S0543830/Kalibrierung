// TestSickDLL.cpp: Hauptprojektdatei.

#include "stdafx.h"
#include "LaScanLib.h"
#include "DataTypes.h"
using namespace System;

int main()
{
	std::list<SPunkt> lTmpPunkte;
	std::list<double> lZeiten;
	SLaserscan tmlstruct;
	std::list<SLaserscan> lTmpStruct;
	unsigned int iandle;
	int iReturn = -1;
	iReturn = initLaScanLib(&iandle,"192.168.0.4", 2111);
	for (int i = 0; i < 10000; i++)
	{
		iReturn = startScanRecording(iandle);
		iReturn = getFilterRecordPoints(iandle,lTmpPunkte, lZeiten, 8);
		iReturn = stopScanRecording(iandle);
		tmlstruct.dZeitstempel_Datenempfangen = lZeiten.front();
		tmlstruct.lScannerDaten = lTmpPunkte;
		tmlstruct.lScannerZeiten = lZeiten;
		lTmpPunkte.clear();
		lZeiten.clear();
		lTmpStruct.push_back(tmlstruct);
	}
    return 0;
}
