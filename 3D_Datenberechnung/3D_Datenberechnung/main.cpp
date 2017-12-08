#include "stdafx.h"
#include "3DCalculation.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "DebugOutput.h"

//#define _USE_3DLIB

int main()
{
	std::string sEncoder("Kalibrierung_Encoder_MitObjekt_Mittelwert8.asc");
	//std::string sScannerA("Kalibrierung_0.asc");
	std::string sScannerA("Kalibrierung_Scan_Mit_Objekt_0_Mittelwert_8.asc");

/************************************************/
	CDebugOutput objDebug;

	std::fstream inEnc;
	std::stringstream ss;
	std::string line;
	std::list<SEncoder> liEncoder;
	SEncoder tmpEnc;
	
	inEnc.open(sEncoder.c_str(), std::ios::in);
	if (inEnc.is_open())
	{
		std::getline(inEnc, line);
		
		while (std::getline(inEnc, line))
		{
			ss << line;
			ss >> tmpEnc.dPosition >> tmpEnc.dZeitstempel;
			liEncoder.push_back(tmpEnc);
			ss.clear();
		}
	}
	inEnc.close();
	tmpEnc = liEncoder.front();
	tmpEnc.dPosition = 0;
	tmpEnc.dZeitstempel -= 0.2;
	liEncoder.push_front(tmpEnc);

	std::cout << "Encoder gelesen" << std::endl;

	/*************************************************************/

	std::fstream inScan;
	std::list<SLaserscan> liScanner;
	SLaserscan tmpSca;
	std::list<SPunkt> liPoints;
	std::list<double> liTime;
	SPunkt tmpPoint;
	double tmpTime;

		
	inScan.open(sScannerA.c_str(), std::ios::in);
	std::cout << "Lade Datei: " << sScannerA.c_str() << std::endl;

	if (inScan.is_open())
	{
		std::getline(inScan, line);
		std::getline(inScan, line);

		ss << line;
		ss >> tmpPoint.x >> tmpPoint.y >> /*tmpPoint.z >> tmpPoint.dDistanz >> tmpPoint.dOrientierung >>*/tmpTime;
		liPoints.push_back(tmpPoint);
		liTime.push_back(tmpTime);
		ss.clear();
		while (std::getline(inScan, line))
		{
			ss << line;
			ss >> tmpPoint.x >> tmpPoint.y >> /*tmpPoint.z >> tmpPoint.dDistanz >> tmpPoint.dOrientierung >>*/ tmpTime;

			if (tmpTime != liTime.back())
			{
				tmpSca.lScannerDaten = liPoints;
				tmpSca.lScannerZeiten = liTime;
				tmpSca.dZeitstempel_Datenempfangen = liTime.back();
				liScanner.push_back(tmpSca);
				tmpSca.lScannerDaten.clear();
				tmpSca.lScannerZeiten.clear();
				liPoints.clear();
				liTime.clear();
			}

			liPoints.push_back(tmpPoint);
			liTime.push_back(tmpTime);

			ss.clear();
		}
	}
	tmpSca.lScannerDaten = liPoints;
	tmpSca.lScannerZeiten = liTime;
	tmpSca.dZeitstempel_Datenempfangen = liTime.back();
	liScanner.push_back(tmpSca);


	inScan.close();

	std::list<SPunkt> liOut;
	C3DCalc obj3d;
	double dEncRot[3] = { 0,0, 0 };
	double dEncTran[3] = {0, 0, 0 };

	//double dScaRot[3] = {  18, 0,45 }; // SET_2
	//double dScaRot[3] = { 18, 0, 39.88 }; //Kalibrierung_0
	//double dScaRot[3] = { 27,0,38}; //SET_1_0
	//double dScaRot[3] = { 24.43,0,17.58  }; // SET_3_0
	double dScaRot[3] = { 90,0,  90 }; // SET_3_2
	double dScaTran[3] = { 0, 0, 0 };

	liOut = obj3d.Calc3DCPunkt(liScanner, liEncoder, dEncRot, dEncTran, dScaRot, dScaTran);

	sScannerA.insert(sScannerA.size() - 4, "_3D");
	std::string s = "ConvertK_30_31_7_.asc";
	std::cout << "Schreibe Datei: " << s.c_str() << std::endl;
	obj3d.Save3D(liOut, s);

	return 0;
}