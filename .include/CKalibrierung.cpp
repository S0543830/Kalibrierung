#include "CKalibrierung.h"
#include "2DFilter.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <conio.h> 
#include <Convert.h>
#include "TimeStamp.h"
#include "XMLParameter.h"

CKalibrierung::CKalibrierung():
	PI(3.14159265359),
	m_objEncoder(nullptr),
	iAnzahlScanner(0), 
	bIsInitScanner(false), 
	bEncoderRec(false), 
	moveToPos(0)
{
}

CKalibrierung::~CKalibrierung()
{
}

template<class T> T fromString(const std::string& s)
{
	std::istringstream stream(s);
	T t;
	stream >> t;
	return t;
}

// Trennt die Zahlen nach dem Symikolon
inline std::vector<std::string> split(const std::string& s, const std::string& f)
{
	std::vector<std::string> temp;

	//if no seperator than return string
	if (f.empty()) {
		temp.push_back(s);
		return temp;
	}

	typedef std::string::const_iterator iter;
	const iter::difference_type f_size(distance(f.begin(), f.end()));
	iter i(s.begin());

	for (iter pos; (pos = search(i, s.end(), f.begin(), f.end())) != s.end(); )
	{
		temp.push_back(std::string(i, pos));
		advance(pos, f_size);
		i = pos;
	}

	temp.push_back(std::string(i, s.end()));

	return temp;
}


int readASC(std::vector<std::list<SLaserscan>*>& vOut,bool isObject)
{
	std::fstream inm;
	std::string line;
	SPunkt a;
	SLaserscan strLaser;
	std::list<SPunkt> lPunkte;
	std::list<double> lZeitstempel;
	int iReturn = -1;
	double dZeitstempel = 0;
	vOut.resize(1);
	for (int j = 0; j < 1; j++)
	{
		std::string stdsa = std::to_string(j);
		//inm.open("Kalibrierung_" + stdsa + ".asc");
		//inm.open("K_SET_1_Rohdaten_"+stdsa+".asc");
		if(true == isObject)
		{ 
			//inm.open("Kalibrierung_NurObjekt_15grad.asc");
			//inm.open("Kalibrierung_NurObjekt_0Grad.asc");
			inm.open("Neu_Aufbau_NachEinerStunde_median.asc");
			//inm.open("objektlinie.asc");
		}
		else
		{
			inm.open("Kalibrierung_Scan_Ohne_Objekt_0_Mittelwert_8.asc");
		}
		std::list < SLaserscan> * lstrLaser = new std::list<SLaserscan>();
		if (inm.is_open())
		{
			std::getline(inm, line);
			while (std::getline(inm, line))
			{
				std::vector<std::string> elems = split(line, " ");
				if (dZeitstempel == 0)
				{
					a.x = fromString<double>(elems[0]);
					a.y = fromString<double>(elems[1]);
					a.z = 0;
					lPunkte.push_back(a);
					lZeitstempel.push_back(fromString<double>(elems[2]));
					dZeitstempel = fromString<double>(elems[2]);
				}
				else if (dZeitstempel == fromString<double>(elems[2]))
				{
					a.x = fromString<double>(elems[0]);
					a.y = fromString<double>(elems[1]);
					a.z = 0;
					lPunkte.push_back(a);
					lZeitstempel.push_back(fromString<double>(elems[2]));
					dZeitstempel = fromString<double>(elems[2]);
				}
				else if (dZeitstempel != fromString<double>(elems[2]))
				{
					strLaser.dZeitstempel_Datenempfangen = lZeitstempel.front();
					strLaser.lScannerDaten = lPunkte;
					strLaser.lScannerZeiten = lZeitstempel;
					lstrLaser->push_back(strLaser);
					lPunkte.clear();
					lZeitstempel.clear();
					a.x = fromString<double>(elems[0]);
					a.y = fromString<double>(elems[1]);
					a.z = 0;
					lPunkte.push_back(a);
					lZeitstempel.push_back(fromString<double>(elems[2]));
					dZeitstempel = fromString<double>(elems[2]);
				}
			}
			strLaser.dZeitstempel_Datenempfangen = lZeitstempel.front();
			strLaser.lScannerDaten = lPunkte;
			strLaser.lScannerZeiten = lZeitstempel;
			lstrLaser->push_back(strLaser);
			lPunkte.clear();
			lZeitstempel.clear();
		}
		inm.close();
		vOut.at(j) = lstrLaser;
	}
	if (vOut.at(0)->size() != 0)
	{
		iReturn = 0;
	}
	return iReturn;
}

int CKalibrierung::startKalibrierung(std::vector<std::list<SLaserscan>*> vlScan, std::vector<SKalibrierung>& vSKalibrierungOut, char cScanRichtung = 'x', int EncoderSteps = 10)
{
	int iReturn = -1;
//	std::string sCOM;
//	std::string sParity;
//	std::string sDLL_Pfad;
//	int iBaud;
//	int iData;
//	int iStop;
//	int iVal;
//	CScanner* objScanner;
//	iReturn = getEncorderDataFromXML("../../.config/Thuema_Config_Debug.xml", sDLL_Pfad, sCOM, iVal, iBaud, sParity, iData, iStop);
//	std::vector<ScannerInitParameter> vScannerConfigData;
//	int iAnzahlScanner = 0;
//	if (iReturn == 0)
//	{
//		iReturn = initEncoder((char*)sDLL_Pfad.c_str(), iVal, (char*)sCOM.c_str(), iBaud, sParity[0], iData, iStop);
//		iReturn = getScannerDataFromXML("../../.config/Thuema_Config_Debug.xml", vScannerConfigData);
//		for (unsigned int i = 0; i < vScannerConfigData.size(); i++)
//		{
//			//Initialisierung des Scanners
//			//	iReturn = initScanner(vScannerConfigData.at(i).ccDLLPfad.c_str(), vScannerConfigData.at(i).sIP.c_str(), vScannerConfigData.at(i).iPort, i);
//			this->iAnzahlScanner++;
//			objScanner = new CScanner();
//			iReturn = objScanner->initScanner(vScannerConfigData.at(i).ccDLLPfad.c_str(), vScannerConfigData.at(i).sIP.c_str(), vScannerConfigData.at(i).iPort);
//			if (0 != iReturn)
//			{
//				objScanner->exitScanner();
//				this->bIsInitScanner = false;
//				iReturn = -1;
//				delete objScanner;
//			}
//			else
//			{
//				this->vobjScanner.push_back(objScanner);
//			}
//		}
//	}
////#pragma region Aufnahme_Ohne_Objekt
////	std::cout << "Bitte entfernen Sie das Objekt." << std::endl;
////	std::cout << "Drücken Sie eine beliebige Taste . . ." << std::endl;
////	std::cin.ignore();
////	SEncoder tmpEncoder;
////	std::list<SEncoder> lEncoderWithoutObject;
////	std::vector<std::list<SLaserscan>>* vScanWithoutObject = new std::vector<std::list<SLaserscan>>();
////	vScanWithoutObject->resize(this->iAnzahlScanner);
////	double pos = 0;
////	while ((getPosEncoder() >= 0) && (getPosEncoder()<400))
////	{
////		pos = getPosEncoder(),
////		moveEncoder(EncoderSteps);
////		Sleep(1000);
////		getFilteredPoitns(vScanWithoutObject, 8);
////		tmpEncoder.dZeitstempel = CTimeStamp::getTimeStamp();
////		tmpEncoder.dPosition = getPosEncoder();
////		lEncoderWithoutObject.push_back(tmpEncoder);
////	}
////#pragma endregion Aufnahme_Ohne_Objekt
////	moveEncoder(-getPosEncoder());
//
//#pragma region Aufnahme_Mit_Objekt
//	SEncoder tmpEncoder;
//	std::cout << "Bitte setzen Sie das Objekt wieder ein." << std::endl;
//	std::cout << "Drücken Sie eine beliebige Taste . . ." << std::endl;
//	std::cin.ignore();
//	std::list<SEncoder> lEncoderWithObject;
//	std::vector<std::list<SLaserscan>>* vScanWithObject = new std::vector<std::list<SLaserscan>>();
//	vScanWithObject->resize(this->iAnzahlScanner);
//	while ((getPosEncoder() >= 0) && (getPosEncoder()<50))
//	{
//		moveEncoder(EncoderSteps);
//		Sleep(5000);
//		getFilteredPoitns(vScanWithObject, 8,4);
//		tmpEncoder.dZeitstempel = CTimeStamp::getTimeStamp();
//		tmpEncoder.dPosition = getPosEncoder();
//		lEncoderWithObject.push_back(tmpEncoder);
//	}
//#pragma endregion Aufnahme_Mit_Objekt
//	moveEncoder(-getPosEncoder());
//	std::fstream in;
//#pragma region Speichern in ASC
//	//Speichern Scans it Objekt
//	for (int i = 0; i < vScanWithObject->size(); i++)
//	{
//		std::string stdsa = std::to_string(i);
//		in.open("Kalibrierung_Scan_Mit_Objekt_" + stdsa + "_Mittelwert_8.asc", std::ios::out);
//		in << "X Y Zeitstempel" << std::endl;
//		for (std::list<SLaserscan>::iterator scan = vScanWithObject->at(i).begin(); scan != vScanWithObject->at(i).end(); ++scan)
//		{
//			for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
//			{
//				in << itPoints->x << " " << itPoints->y << " " << scan->lScannerZeiten.front() << std::endl;
//			}
//		}
//		in.close();
//	}
//
//	//Speichern Scans ohne Objekt
//	/*for (int i = 0; i < vScanWithoutObject->size(); i++)
//	{
//		std::string stdsa = std::to_string(i);
//		in.open("Kalibrierung_Scan_Ohne_Objekt_" + stdsa + "_Mittelwert_8.asc", std::ios::out);
//		in << "X Y Zeitstempel" << std::endl;
//		for (std::list<SLaserscan>::iterator scan = vScanWithoutObject->at(i).begin(); scan != vScanWithoutObject->at(i).end(); ++scan)
//		{
//			for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
//			{
//				in << itPoints->x << " " << itPoints->y << " " << scan->lScannerZeiten.front() << std::endl;
//			}
//		}
//		in.close();
//	}*/
//
//	//Speichern Encoder mit Objekt
//	in.open("Kalibrierung_Encoder_MitObjekt_Mittelwert8.asc", std::ios::out);
//	in << "Position Zeitstempel" << std::endl;
//	for (std::list<SEncoder>::iterator lEncoder = lEncoderWithObject.begin(); lEncoder != lEncoderWithObject.end(); ++lEncoder)
//	{
//		in << lEncoder->dPosition << " " << lEncoder->dZeitstempel << std::endl;
//	}
//	in.close();
//
//	////Speichern Encoder mit Objekt
//	//in.open("Kalibrierung_Encoder_OhneObjekt_Mittelwert8.asc", std::ios::out);
//	//in << "Position Zeitstempel" << std::endl;
//	//for (std::list<SEncoder>::iterator lEncoder = lEncoderWithoutObject.begin(); lEncoder != lEncoderWithoutObject.end(); ++lEncoder)
//	//{
//	//	in << lEncoder->dPosition << " " << lEncoder->dZeitstempel << std::endl;
//	//}
//	//in.close();
//#pragma endregion Speichern in ASC


	cParaPfad = "../.config/RobaLackiPara.xml";
	std::vector<std::list<SLaserscan>*> vScanWithObject;
	std::vector<std::list<SLaserscan>*> vScanWithoutObject;
	readASC(vScanWithObject,true);
//	readASC(vScanWithoutObject, false);
//
//#pragma region Filterung_vom_Scan
//	std::cout << "Filterung wird gestartert." << endl;
//	std::vector<std::list<SLaserscan>> vlFiltered;
//	iReturn = filterObject(vScanWithObject, vScanWithoutObject/*, lEncoderWithoutObject, lEncoderWithObject*/, vlFiltered);
//#pragma endregion Filterung_vom_Scan



	/*fstream in;
	for (int i = 0; i < vlFiltered.size(); i++)
	{
		std::string stdsa = std::to_string(i);
		in.open("Kalibrierung_Filter_" + stdsa + ".asc", ios::out);
		in << "X Y Zeitstempel" << endl;
		for (std::list<SLaserscan>::iterator scan = vlFiltered.at(i).begin(); scan != vlFiltered.at(i).end(); ++scan)
		{
			for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
			{
				in << itPoints->x << " " << itPoints->y << " " << scan->lScannerZeiten.front() << endl;
			}
		}
		in.close();
	}*/

	//SEncoder tmpEncoder;
	//std::list<SEncoder> lEncoder;
	//int aktPos = getPosEncoder();
	////Der Encoder wird bewegt und es werden Messpunkte aufgenommen
	//while ((getPosEncoder()>= 0) && (getPosEncoder()<300))
	//{
	//	moveEncoder(EncoderSteps);
	//	Sleep(3000);
	//	getFilteredPoitns(scanOut, 12);
	//	tmpEncoder.dZeitstempel = CTimeStamp::getTimeStamp();
	//	tmpEncoder.dPosition = getPosEncoder();
	//	lEncoder.push_back(tmpEncoder);
	//}
	//moveEncoder(-300);
	//std::fstream in;

	//for (int i = 0; i < scanOut->size(); i++)
	//{
	//	std::string stdsa = std::to_string(i);
	//	in.open("Pepperl_u_Fuchs_Kalibrierkoerper_12Scan.asc", ios::out);
	//	in << "X Y Z Distanz Orientierung Zeitstempel" << endl;
	//	for (std::list<SLaserscan>::iterator scan = scanOut->at(i).begin(); scan != scanOut->at(i).end(); ++scan)
	//	{
	//		for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
	//		{
	//			in << itPoints->x << " ";
	//			in << itPoints->y << " ";
	//			in << itPoints->z << " ";
	//			in << itPoints->dDistanz << " ";
	//			in << itPoints->dOrientierung << " ";
	//			in << scan->lScannerZeiten.front() << endl;
	//		}
	//	}
	//	in.close();
	//}
	//in.open("Encoder_Pepperl_u_Fuchs_Kalibrierkoerper_12Scan.asc", ios::out);
	//in << "Position Zeitstempel" << endl;
	//for (std::list<SEncoder>::iterator scan = lEncoder.begin(); scan != lEncoder.end(); ++scan)
	//{
	//	in << scan->dPosition << " ";
	//	in << scan->dZeitstempel << endl;
	//}
	//in.close();
	this->m_OriginComponentSystem.resize(vlScan.size());
	//----------------Filtere Umgebung Begin-----------------//
	//iReturn = filterBodyFromScan(vlScan, vlFiltered);
	vlScan.clear();
	
	//----------------Filtere Umgebung Ende-----------------//

	/*********************************************************/

	//----------------Berechne Rotationen Begin--------------//
	std::vector<std::list<std::list<StructGeradenPunkte>>> vlRegressionsGeraden;
	iReturn = ermittleRegressionsGeraden(vScanWithObject, vlRegressionsGeraden);
	std::vector<std::list<double>> vRotWinkel;
	iReturn = rotWinkelBerechnen(vlRegressionsGeraden, vSKalibrierungOut);
	vlRegressionsGeraden.clear();
	//----------------Berechne Rotationen Ende--------------//

	/********************************************************/

	//--------------Berechne Translation Begin--------------//
	//iReturn = setComponentSystem(vlFiltered,cScanRichtung);
	iReturn = setReferenceSystem();
	//--------------Berechne Translation Ende--------------//
	return iReturn;
}

int CKalibrierung::setReferenceSystem()
{
	return 0;
}

//Setzt den Ursprung des Werkstückkoordinatensystems
int CKalibrierung::setComponentSystem(std::vector<std::list<SLaserscan>> vScanIn, char cScanRichtung = 'x')
{
	int iReturn = -1;
	int iMaxPointSize = 0;
	std::list<SLaserscan>::iterator maxScan;
	std::list<SLaserscan> lScanOut;
	for (int i = 0; i < vScanIn.size(); i++)
	{
		for (std::list<SLaserscan>::iterator scan = vScanIn.at(i).begin(); scan != vScanIn.at(i).end(); ++scan)
		{
			if (iMaxPointSize < scan->lScannerDaten.size())
			{
				iMaxPointSize = scan->lScannerDaten.size();
				maxScan = scan;
			}
		}
		this->ermittleRegressionsGeraden(maxScan, lScanOut, iMaxPointSize);
		iMaxPointSize = 0;
		//Finde den größten Scan
		for (std::list<SLaserscan>::iterator scan = lScanOut.begin(); scan != lScanOut.end(); ++scan)
		{
			if (iMaxPointSize < scan->lScannerDaten.size())
			{
				iMaxPointSize = scan->lScannerDaten.size();
				maxScan = scan;
			}
		}
		lScanOut.clear();
		std::list<SPunkt>::iterator myPoints = maxScan->lScannerDaten.begin();
		//Bestimme den Ursprung des Werkstückkoordinatensystem
		for (int j = 0; j < iMaxPointSize ; j++)
		{
			if (j <iMaxPointSize / 2)
			{
				++myPoints;
				j = j + 1;
			}
			else
			{
				m_OriginComponentSystem.at(i).dDistanz = myPoints->dDistanz;
				m_OriginComponentSystem.at(i).dOrientierung = myPoints->dOrientierung;
				m_OriginComponentSystem.at(i).x = myPoints->x;
				m_OriginComponentSystem.at(i).y = myPoints->y;
				m_OriginComponentSystem.at(i).z = myPoints->z;
				iReturn = 0;
				break;
			}
		}
		iMaxPointSize = 0;
	}
	return iReturn;
}
//Punkte werden zwischen zwei Punkten interpoliert
int CKalibrierung::interpolation(SPunkt A, SPunkt B, int iAnzahl, std::list<SPunkt>& lOut)
{
	std::vector<double> vWertX;
	float dStart;
	double schrittMenge = std::abs(A.x - B.x) / (iAnzahl +1);
	
	if (A.x > B.x)
	{
		dStart = B.x;
	}
	else
	{
		dStart = A.x;
	}
	for (unsigned int i = 0; i < iAnzahl+1; i++)
	{
		dStart = dStart + schrittMenge;
		vWertX.push_back(dStart);
	}
	//Lineare Interpolation
	SPunkt tmp;
	for (unsigned int z = 0; z < vWertX.size(); z++)
	{
		//Y = ( ( X - X1 )( Y2 - Y1) / ( X2 - X1) ) + Y1
		tmp.x = vWertX.at(z);
		tmp.y = ((vWertX.at(z) - A.x)*(B.y - A.y) / (B.x - A.x)) + A.y;
		tmp.z = 0;
		if (z != vWertX.size() - 1)
		{
			lOut.push_back(tmp);
		}
		
	}
	vWertX.clear();
	return 0;
}

bool CKalibrierung::isPointInList(std::list<SPunkt> lListIn, SPunkt A)
{
	SPunkt B;
	double dist = 0;
	for (std::list<SPunkt>::iterator myPoints = lListIn.begin(); myPoints != lListIn.end(); ++myPoints)
	{
		B.x = myPoints->x;
		B.y = myPoints->y;
		B.z = 0;
		dist = berechnerDistanz(A, B);
		if (dist < 30)
		{
			return true;
		}
	}
	return false;
}

bool CKalibrierung::isPointInList(std::list<SLaserscan> lListIn, SPunkt A)
{
	SPunkt B;
	double dist = 0;
	
	for (std::list<SLaserscan>::iterator myPoints = lListIn.begin(); myPoints != lListIn.end(); ++myPoints)
	{
		for (std::list<SPunkt>::iterator points = myPoints->lScannerDaten.begin(); points != myPoints->lScannerDaten.end(); ++points)
		{
			B.x = points->x;
			B.y = points->y;
			B.z = 0;
			dist = berechnerDistanz(A, B);
			if (dist < 30)
			{
				return true;
			}
		}
	}
	return false;
}

int CKalibrierung::filterBody(std::list<SLaserscan> lScanIn,std::list<SLaserscan>& lScanOut)
{ 
	int iReturn = -1;
	int i = 0;
	SPunkt A;
	SPunkt B;
	SPunkt C;
	double CZeit = 0;
	std::list<SPunkt> lObjScanOne;
	std::list<double> lObjZeitOne;
	std::list<SPunkt> lObjScanTwo;
	std::list<double> lObjZeitTwo;
	SLaserscan structOne;
	SLaserscan structTwo;
	std::list<SLaserscan> lStructOne;
	std::list<SLaserscan> lStructTwo;
	int maxEntfernung = 400;
	double dActDis = 0;
	int two = 0;
	int one = 0;
	double minXA = 0;
	double maxXA = 0;
	double minYA = 0;
	double maxYA = 0;
	double minXB = 0;
	double maxXB = 0;
	double minYB = 0;
	double maxYB = 0;
	//Objekte separieren
	for (std::list<SLaserscan>::iterator scan = lScanIn.begin(); scan != lScanIn.end(); ++scan)
	{
		for(std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end();++itPoints)
		{
			A.dDistanz = itPoints->dDistanz;
			A.dOrientierung = itPoints->dOrientierung;
			A.x = itPoints->x;
			A.y = itPoints->y;
			A.z = 0;
			/*if ((minXA == 0) && (minYA == 0) && (maxYA == 0) && (maxXA == 0))
			{
				one++;
				lObjScanOne.push_back(A);
				lObjZeitOne.push_back(scan->lScannerZeiten.front());
				minXA = A.x;
				minYA = A.y;
			}*/
			if ((minXA == 0) && (maxXA == 0))
			{
				one++;
				lObjScanOne.push_back(A);
				lObjZeitOne.push_back(scan->lScannerZeiten.front());
				minXA = A.x;
				minYA = A.y;
				
			}
			else if ((minXA != 0) && (maxXA == 0))
			{
				one++;
				lObjScanOne.push_back(A);
				lObjZeitOne.push_back(scan->lScannerZeiten.front());
				if (A.x < minXA)
				{
					maxXA = minXA;
					minXA = A.x;
				}
				else
				{
					maxXA = A.x;
				}
				if (minYA > A.y)
				{
					maxYA = minYA;
					minYA = A.y;
				}
				else
				{
					maxYA = A.y;
				}
			}
			else
			{
				if ((A.x < maxXA) && (A.x > minXA))//Punkt liegt in A X
				{
					if ((A.y < maxYA) && (A.y > minYA))//Punkt liegt in A Y
					{
						one++;
						lObjScanOne.push_back(A);
						lObjZeitOne.push_back(scan->lScannerZeiten.front());
					}
					else if((abs(A.y - maxYA)<maxEntfernung ) && ( abs(A.y - minYA)< maxEntfernung)) //Punkt liegt in der näher von A Y
					{
						if (A.y < maxYA)
						{
							maxYA = A.y;
						}
						if (A.y > minYA)
						{
							minYA = A.y;
						}
						one++;
						lObjScanOne.push_back(A);
						lObjZeitOne.push_back(scan->lScannerZeiten.front());
					}
					//Gehört nicht zu Objekt A
					else if ((minXB == 0) && (maxXB == 0))
					{
						two++;
						lObjScanTwo.push_back(A);
						lObjZeitTwo.push_back(scan->lScannerZeiten.front());
						if (A.x < minXB)
						{
							minXB = A.x;
						}
						else
						{
							maxXB = A.x;
						}
						if (A.y < minYB)
						{
							minYB = A.y;
						}
						else
						{
							maxYB = A.y;
						}
					}
					else if ((minXB != 0) && (maxXB == 0))
					{
						two++;
						lObjScanTwo.push_back(A);
						lObjZeitTwo.push_back(scan->lScannerZeiten.front());
						if (minXB > A.x)
						{
							maxXB = minXB;
							minXB = A.x;
						}
						else
						{
							maxXB = A.x;
						}
						if (minYB > A.y)
						{
							maxYB = minYB;
							minYB = A.y;
						}
						else
						{
							maxYB = A.y;
						}
					}
					else
					{
						if ((A.x < maxXB) && (A.x > minXB)) //liegt B X
						{
							if ((A.y < maxYB) && (A.y > minYB)) // liegt in B Y
							{
								two++;
								lObjScanTwo.push_back(A);
								lObjZeitTwo.push_back(scan->lScannerZeiten.front());
							}
							else if ((abs(A.y - maxYB) < maxEntfernung) && (abs(A.y - minYB) < maxEntfernung)) //liegt in der nähe B
							{
								if (A.y > maxYB)
								{
									maxYB = A.y;
								}
								if (A.y < minYB)
								{
									minYB = A.y;
								}
								two++;
								lObjScanTwo.push_back(A);
								lObjZeitTwo.push_back(scan->lScannerZeiten.front());
							}
						}
						else if((abs(A.x- maxXB)<maxEntfernung) && (abs(A.x-minXB)<maxEntfernung)) // liegt in der nähe von  B X
						{ 
							if ((A.y < maxYB) && (A.y > minYB)) // liegt in B Y
							{
								two++;
								lObjScanTwo.push_back(A);
								lObjZeitTwo.push_back(scan->lScannerZeiten.front());
							}
							else if ((abs(A.y - maxYB) < maxEntfernung) && (abs(A.y - minYB) < maxEntfernung)) //liegt in der nähe B
							{
								if (A.y > maxYB)
								{
									maxYB = A.y;
								}
								if (A.y < minYB)
								{
									minYB = A.y;
								}
								if (A.x > maxXB)
								{
									maxXB = A.x;
								}
								if (A.x < minXB)
								{
									minXB = A.x;
								}
								two++;
								lObjScanTwo.push_back(A);
								lObjZeitTwo.push_back(scan->lScannerZeiten.front());
							}
							else
							{
								//cout << "Separieren von Objekte fehlgeschlagen: 3. Objekt erkannt";
							}
						}
						else
						{
					//		cout << "Speparieren fehlerhaft: 3.Objekt gefunden";
						}
					}
				}
				else if ((abs(A.x - maxXA) < maxEntfernung) && (abs(A.x - minXA) < maxEntfernung))// liegt in der näher von A X
				{
					if ((A.y < maxYA) && (A.y > minYA)) //liegt in A Y
					{
						if (A.x > maxXA)
						{
							maxXA = A.x;
						}
						if (A.x < minXA)
						{
							minXA = A.x;
						}
						one++;
						lObjScanOne.push_back(A);
						lObjZeitOne.push_back(scan->lScannerZeiten.front());
					}
					else if ((abs(A.y - maxYA) < maxEntfernung) && (abs(A.y - minYA) < maxEntfernung))//liet in der näher von A Y
					{
						if (A.y > maxYA)
						{
							maxYA = A.y;
						}
						if (A.y < minYA)
						{
							minYA = A.y;
						}
						if (A.x > maxXA)
						{
							maxXA = A.x;
						}
						if (A.x < minXA)
						{
							minXA = A.x;
						}
						one++;
						lObjScanOne.push_back(A);
						lObjZeitOne.push_back(scan->lScannerZeiten.front());
					}
					//Gehört zu B
					else if ((minXB == 0) && (maxXB == 0))
					{
						two++;
						lObjScanTwo.push_back(A);
						lObjZeitTwo.push_back(scan->lScannerZeiten.front());
						if (A.x < minXB)
						{
							minXB = A.x;
						}
						else
						{
							maxXB = A.x;
						}
						if (A.y < minYB)
						{
							minYB = A.y;
						}
						else
						{
							maxYB = A.y;
						}
					}
					else if ((minXB != 0) && (maxXB == 0))
					{
						two++;
						lObjScanTwo.push_back(A);
						lObjZeitTwo.push_back(scan->lScannerZeiten.front());
						if (minXB > A.x)
						{
							maxXB = minXB;
							minXB = A.x;
						}
						else
						{
							maxXB = A.x;
						}
						if (minYB > A.y)
						{
							maxYB = minYB;
							minYB = A.y;
						}
						else
						{
							maxYB = A.y;
						}
					}
					else
					{
						if ((A.x < maxXB) && (A.x > minXB)) //liegt in B X
						{
							if ((A.y < maxYB) && (A.y > minYB)) //liegt in B Y
							{
								two++;
								lObjScanTwo.push_back(A);
								lObjZeitTwo.push_back(scan->lScannerZeiten.front());
							}
							else if ((abs(A.y - maxYB) < maxEntfernung) && (abs(A.y - minYB) < maxEntfernung)) //liegt in der näher von B Y
							{
								if (A.y > maxYB)
								{
									maxYB = A.y;
								}
								if (A.y < minYB)
								{
									minYB = A.y;
								}
								two++;
								lObjScanTwo.push_back(A);
								lObjZeitTwo.push_back(scan->lScannerZeiten.front());
							}
						}
						else if( (abs(A.x-maxXB)<maxEntfernung)&& (abs(A.x-minXB)<maxEntfernung))// in der nähe von B X
						{
							if ((A.y < maxYB) && (A.y > minYB))//liegt in B Y
							{
								if (A.x > maxXB)
								{
									maxXB = A.x;
								}
								if(A.x < minXB)
								{
									minXB = A.x;
								}
								two++;
								lObjScanTwo.push_back(A);
								lObjZeitTwo.push_back(scan->lScannerZeiten.front());
							}
							else if ((abs(A.y - maxYB) < maxEntfernung) && (abs(A.y - minYB) < maxEntfernung))//in der nähe von B Y
							{
								if (A.x > maxXB)
								{
									maxXB = A.x;
								}
								if (A.x < minXB)
								{
									minXB = A.x;
								}
								if (A.y > maxYB)
								{
									maxYB = A.y;
								}
								if (A.y < minYB)
								{
									minYB = A.y;
								}
								two++;
								lObjScanTwo.push_back(A);
								lObjZeitTwo.push_back(scan->lScannerZeiten.front());
							}
							else // gehört weder zu A noch zu B
							{ 
							//	std::cout << "Separieren von Objekt fehlerhaft: 3. Objekt gefunden";
							}
						}
					}
				}
				//Liegt in B
				else if ((minXB == 0) && (maxXB == 0))
				{
					two++;
					lObjScanTwo.push_back(A);
					lObjZeitTwo.push_back(scan->lScannerZeiten.front());
					minXB = A.x;
					minYB = A.y;
				}
				else if ((minXB != 0) && (maxXB == 0))
				{
					two++;
					lObjScanTwo.push_back(A);
					lObjZeitTwo.push_back(scan->lScannerZeiten.front());
					if (minXB > A.x)
					{
						maxXB = minXB;
						minXB = A.x;
					}
					else
					{
						maxXB = A.x;
					}
					if (minYB > A.y)
					{
						maxYB = minYB;
						minYB = A.y;
					}
					else
					{
						maxYB = A.y;
					}
				}
				else
				{
					if ((A.x < maxXB) && (A.x > minXB)) //liegt in B X
					{
						if ((A.y < maxYB) && (A.y > minYB)) //liegt in B Y
						{
							two++;
							lObjScanTwo.push_back(A);
							lObjZeitTwo.push_back(scan->lScannerZeiten.front());
						}
						else if ((abs(A.y - maxYB) < maxEntfernung) && (abs(A.y - minYB) < maxEntfernung)) //liegt in der näher von B Y
						{
							if (A.y > maxYB)
							{
								maxYB = A.y;
							}
							if (A.y < minYB)
							{
								minYB = A.y;
							}
							two++;
							lObjScanTwo.push_back(A);
							lObjZeitTwo.push_back(scan->lScannerZeiten.front());
						}
						else
						{
							//cout << "Separieren fehlerhaft: 3. Objekt gefunden";
						}
					}
					else if ((abs(A.x - maxXB)<maxEntfernung) && (abs(A.x - minXB)<maxEntfernung))// in der nähe von B X
					{
						if ((A.y < maxYB) && (A.y > minYB))//liegt in B Y
						{
							if (A.x > maxXB)
							{
								maxXB = A.x;
							}
							if (A.x < minXB)
							{
								minXB = A.x;
							}
							two++;
							lObjScanTwo.push_back(A);
							lObjZeitTwo.push_back(scan->lScannerZeiten.front());
						}
						else if ((abs(A.y - maxYB) < maxEntfernung) && (abs(A.y - minYB) < maxEntfernung))//in der nähe von B Y
						{
							if (A.x > maxXB)
							{
								maxXB = A.x;
							}
							if (A.x < minXB)
							{
								minXB = A.x;
							}
							if (A.y > maxYB)
							{
								maxYB = A.y;
							}
							if (A.y < minYB)
							{
								minYB = A.y;
							}
							two++;
							lObjScanTwo.push_back(A);
							lObjZeitTwo.push_back(scan->lScannerZeiten.front());
						}
						else // gehört weder zu A noch zu B
						{
					//		std::cout << "Separieren von Objekt fehlerhaft: 3. Objekt gefunden";
						}
					}
					else
					{
					//	cout << "Spearieren fehlerhaft: 3. Objekt gefunden";
					}
				}
			}

			/*if ((lObjScanTwo.size() == 0) && (lStructTwo.size() == 0))
			{
				lObjScanTwo.push_back(A);
				lObjZeitTwo.push_back(scan->lScannerZeiten.front());
				two++;
			}
			else if ((lObjScanTwo.size() == 0) && (lStructTwo.size() != 0))
			{
				if (true == isPointInList(lStructTwo, A))
				{
					lObjScanTwo.push_back(A);
					lObjZeitTwo.push_back(scan->lScannerZeiten.front());
					two++;
				}
				else
				{
					one++;
					lObjScanOne.push_back(A);
					lObjZeitOne.push_back(scan->lScannerZeiten.front());
				}
			}
			else if ((lObjScanTwo.size() != 0))
			{
				if (true == isPointInList(lObjScanTwo, A))
				{
					two++;
					lObjScanTwo.push_back(A);
					lObjZeitTwo.push_back(scan->lScannerZeiten.front());
				}
				else
				{
					one++;
					lObjScanOne.push_back(A);
					lObjZeitOne.push_back(scan->lScannerZeiten.front());
				}
			}*/
		}
		if (lObjScanOne.size() != 0)
		{
			structOne.lScannerDaten = lObjScanOne;
			structOne.lScannerZeiten = lObjZeitOne;
			structOne.dZeitstempel_Datenempfangen = scan->dZeitstempel_Datenempfangen;
			lStructOne.push_back(structOne);
			lObjScanOne.clear();
			lObjZeitOne.clear();
		}
		if (lObjScanTwo.size() != 0)
		{
			structTwo.lScannerDaten = lObjScanTwo;
			structTwo.lScannerZeiten = lObjZeitTwo;
			structTwo.dZeitstempel_Datenempfangen = scan->dZeitstempel_Datenempfangen;
			lStructTwo.push_back(structTwo);
			lObjScanTwo.clear(),
			lObjZeitTwo.clear();
		}
	}

	if (lStructOne.size() == 0)
	{
		lScanOut = lStructTwo;
		iReturn = 0;
	}
	else if(lStructTwo.size() == 0)
	{
		lScanOut = lStructOne;
		iReturn = 0;
	}
	else
	{
		if (one >two)
		{
			if (((one - two) / (two * 100)) < 10)
			{
				lScanOut = lStructOne;
				iReturn = 0;
			}
		}
		else
		{
			if (((two - one) / (one * 100)) < 10)
			{
				lScanOut = lStructTwo;
				iReturn = 0;
			}
		}
		//Objekt erkennen im separierten Scan
		if (iReturn != 0)
		{
			for (std::list<SLaserscan>::iterator lScanOne = lStructOne.begin(); lScanOne != lStructOne.end(); ++lScanOne)
			{
				for (std::list<SPunkt>::iterator pointsFromOne = lScanOne->lScannerDaten.begin(); pointsFromOne != lScanOne->lScannerDaten.end(); ++pointsFromOne)
				{
					if (i == 0)
					{
						A.x = pointsFromOne->x;
						A.y = pointsFromOne->y;
						i++;
					}
					else
					{
						B.x = pointsFromOne->x;
						B.y = pointsFromOne->y;
						if (15 > berechnerDistanz(A, B))
						{
							iReturn = 0;
							lScanOut = lStructOne;
							break;
						}
						A = B;
					}
				}
				i = 0;
			}
			if (iReturn != 0)
			{
				for (std::list<SLaserscan>::iterator lScanTwo = lStructTwo.begin(); lScanTwo != lStructTwo.end(); ++lScanTwo)
				{
					for (std::list<SPunkt>::iterator pointsFromOne = lScanTwo->lScannerDaten.begin(); pointsFromOne != lScanTwo->lScannerDaten.end(); ++pointsFromOne)
					{
						if (i == 0)
						{
							A.x = pointsFromOne->x;
							A.y = pointsFromOne->y;
							i++;
						}
						else
						{
							B.x = pointsFromOne->x;
							B.y = pointsFromOne->y;
							if (15 > berechnerDistanz(A, B))
							{
								iReturn = 0;
								lScanOut = lStructTwo;
								break;
							}
							A = B;
						}
					}
					i = 0;
				}
			}
		}
	}
	return iReturn;
}

//Berechnet den Schnittwinkel zweier Geraden
double CKalibrierung::calculateAngleOfIntersection(StructGeradengleichung streightLineA, StructGeradengleichung streightLineB)
{
	double zaehler = 0;
	zaehler = streightLineA.dSteigung - streightLineB.dSteigung;
	double nenner = 0;
	nenner = 1 + (streightLineA.dSteigung * streightLineB.dSteigung);
	return atan(abs(zaehler / nenner))/PI*180.0;
}

//Berechnet den Schnittwinkel zweier Geraden durch den Steigungen der Geraden
double CKalibrierung::calculateAngleOfIntersection(double dSteigungA, double dSteigungB)
{
	double zaehler = 0;
	zaehler = dSteigungA - dSteigungB;
	double nenner = 0;
	nenner = 1 + (dSteigungA * dSteigungB);
	return atan(abs(zaehler / nenner)) * (180/PI);
}

//Berechnet Schnittpunkt zweier Geraden
SPunkt CKalibrierung::calculateIntersectionPoint(StructGeradengleichung gerade1, StructGeradengleichung gerade2)
{
	/*y1 = m1*x + b1
	y2 = m2*x + b2

	Gleichsetzen :

	m1*x + b1 = m2*x + b2
	m1*x - m2*x = b2 - b1

	Ausklammern :

	x * (m1 - m2) = b2 - b1

	Ergebnis :
	x = (b2 - b1) / (m1 - m2)
	y = m1* ((b2 - b1 / (m1 - m2)) + b1*/
	SPunkt Schnittpunkt;
	Schnittpunkt.x = (gerade2.dKorreleationskoeffiezient - gerade1.dKorreleationskoeffiezient) / (gerade1.dSteigung - gerade2.dSteigung);
	Schnittpunkt.y = gerade1.dSteigung* ((gerade2.dKorreleationskoeffiezient - gerade1.dKorreleationskoeffiezient) / (gerade1.dSteigung - gerade2.dSteigung)) + gerade1.dKorreleationskoeffiezient;
	return Schnittpunkt;
}

int CKalibrierung::filterObject(std::vector<std::list<SLaserscan>*> vlScanWithObject, std::vector<std::list<SLaserscan>*> vlScanWithoutObject,
	/*std::list<SEncoder> lEncoderWithoutObject , std::list<SEncoder> lEncoderWithObject = NULL*/std::vector<std::list<SLaserscan>>& vlFilteredScan)
{
	int iReturn = -1;
	int iMinSize = 0;
	SPunkt tmp;
	std::list<SPunkt> ltmpScanPoints;
	std::list<double> ltmpTimeStamp;
	SLaserscan structScan;
	std::list<SLaserscan> lScanObjekt;
	bool bIsBody = true;
	vlFilteredScan.resize(vlScanWithObject.size());
	for (int i = 0; i < vlScanWithObject.size(); i++)
	{
		std::list<SLaserscan>::iterator lWithObjekt = vlScanWithObject.at(i)->begin();
		std::list<SLaserscan>::iterator lWithoutObjekt = vlScanWithoutObject.at(i)->begin();
		while (lWithObjekt != vlScanWithObject.at(i)->end())
		{
			for (std::list<SPunkt>::iterator myPointsWO = lWithObjekt->lScannerDaten.begin(); myPointsWO != lWithObjekt->lScannerDaten.end(); ++myPointsWO)
			{
				if ((myPointsWO->x != 0) && (myPointsWO->y != 0))
				{
					for (std::list<SPunkt>::iterator myPoints = lWithoutObjekt->lScannerDaten.begin(); myPoints != lWithoutObjekt->lScannerDaten.end(); ++myPoints)
					{
						//Vergleiche die Punkte der jeweiligen Scans
						//Wenn die Bedingung erfüllt ist, gehört dieser Punkt zum Objekt
						if ((abs(myPoints->x - myPointsWO->x) < 40) && (abs(myPoints->y - myPointsWO->y) < 40))
						{
							bIsBody = false;
							break;
						}
					}
				
					if (true == bIsBody)
					{
						tmp.dDistanz = myPointsWO->dDistanz;
						tmp.dOrientierung = myPointsWO->dOrientierung;
						tmp.x = myPointsWO->x;
						tmp.y = myPointsWO->y;
						tmp.z = myPointsWO->z;
						ltmpTimeStamp.push_back(lWithObjekt->lScannerZeiten.front());
						ltmpScanPoints.push_back(tmp);
					}
					bIsBody = true;
				}
			}
			if (ltmpScanPoints.size() != 0)
			{
				structScan.dZeitstempel_Datenempfangen = lWithObjekt->dZeitstempel_Datenempfangen;
				structScan.lScannerDaten = ltmpScanPoints;
				structScan.lScannerZeiten = ltmpTimeStamp;
				lScanObjekt.push_back(structScan);
				ltmpScanPoints.clear();
				ltmpTimeStamp.clear();
				iReturn = 0;
			}
			++lWithObjekt;
			++lWithoutObjekt;
		}
		vlFilteredScan.at(i) = lScanObjekt;
		lScanObjekt.clear();
	}
	return iReturn;
}

int CKalibrierung::filterBodyFromScan(std::vector<std::list<SLaserscan>*> vlScan, std::vector<std::list<SLaserscan>>& vlFilteredScan)
{
	int iReturn = -1;
	int j = 0;
	std::list<SPunkt> lFirstScan;
	std::list<SPunkt>::iterator pointsFromFirstScan;
	std::list<SPunkt> ltmpScanPoints;
	std::list<double> ltmpTimeStamp;
	SPunkt tmp;
	SPunkt A;
	SPunkt B;
	std::list<SPunkt> lFördererScan;
	std::vector<std::list<SLaserscan>> vlstrScan;
	vlstrScan.resize(15);
	SLaserscan structScan;
	bool bIsPointOfBody = true;
	vlFilteredScan.resize(vlScan.size());
	SLaserscan structScanFilterMixPoints;
	std::list<SLaserscan> lStructFilteredScans;
	std::list<SLaserscan> lStructFilteredMixPoints;
	C2DFilter objFilter;
	for (int i = 0; i < vlScan.size(); i++)
	{
		//Die Umgebung wird raus gefiltert
		for (std::list<SLaserscan>::iterator scan = vlScan.at(i)->begin(); scan != vlScan.at(i)->end(); ++scan)
		{
			if (j != 0)
			{
				pointsFromFirstScan = lFirstScan.begin();
				for (std::list<SPunkt>::iterator myPoints = scan->lScannerDaten.begin(); myPoints != scan->lScannerDaten.end(); ++myPoints)
				{
					if( (myPoints->x != 0) && (myPoints->y != 0))
					{
						//Scanvergleich 1. Scan mit nte Scan
						if((abs(myPoints->x - pointsFromFirstScan->x)>100) && (abs(myPoints->y - pointsFromFirstScan->y)>100) )
						{
							//Rauschuntersuchung
							for (std::list<SPunkt>::iterator lFirstScanRausch = lFirstScan.begin(); lFirstScanRausch != lFirstScan.end(); ++lFirstScanRausch)
							{
								if ((abs(lFirstScanRausch->x - myPoints->x)<80) && 
									(abs(lFirstScanRausch->y - myPoints->y)<80))
								{
									bIsPointOfBody = false;
								}
							}
							if (bIsPointOfBody == true)
							{
								tmp.dDistanz = myPoints->dDistanz;
								tmp.dOrientierung = myPoints->dOrientierung;
								tmp.x = myPoints->x;
								tmp.y = myPoints->y;
								tmp.z = myPoints->z;
								ltmpScanPoints.push_back(tmp);
								ltmpTimeStamp.push_back(scan->lScannerZeiten.front());
							}
							else
							{
								bIsPointOfBody = true;
							}
						}
						++pointsFromFirstScan;
					}
				}
				if (ltmpScanPoints.size() != 0)
				{
					structScan.dZeitstempel_Datenempfangen = scan->dZeitstempel_Datenempfangen;
					structScan.lScannerDaten = ltmpScanPoints;
					structScan.lScannerZeiten = ltmpTimeStamp;
					lStructFilteredScans.push_back(structScan);
					ltmpScanPoints.clear();
					ltmpTimeStamp.clear();
					iReturn = 0;
				}
			}
			else
			{
				lFirstScan = scan->lScannerDaten;
				j++;
			}

		}
		double dminAngle = 0;
		double dmaxAngle = 0;
		double dminDistanz = 0;
		double dmaxDistanz = 0;
		if (i == 2)
		{
			std::fstream in;
			int i = 0;
			in.open("./rec/K_SET_1_Rohdaten_vorher.asc", std::ios::out);
			for (std::list<SLaserscan>::iterator scan = lStructFilteredScans.begin(); scan != lStructFilteredScans.end(); ++scan)
			{
				for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
				{
					in << itPoints->x << " " << itPoints->y << " " << itPoints->z << " " << itPoints->dOrientierung << std::endl;
				}
			}
			in.close();
		}
		objFilter.filterMixPoints(lStructFilteredScans, lStructFilteredMixPoints, 15, 2);
		//Suche die min. u. max Entfernung sowie Orientierungswinkel
		for (std::list<SLaserscan>::iterator mixPoints = lStructFilteredMixPoints.begin(); mixPoints != lStructFilteredMixPoints.end(); ++mixPoints)
		{
			for(std::list<SPunkt>::iterator points = mixPoints->lScannerDaten.begin();points != mixPoints->lScannerDaten.end();++points)
			{
				if (dminAngle > points->dOrientierung)
				{
					dminAngle = points->dOrientierung;
				}
				if (dmaxAngle < points->dOrientierung)
				{
					dmaxAngle = points->dOrientierung;
				}
				if (dminDistanz > points->dDistanz)
				{
					dminDistanz = points->dDistanz;
				}
				if (dmaxDistanz < points->dDistanz)
				{
					dmaxDistanz = points->dDistanz;
				}
			}
			
		}
		std::list<SLaserscan> lFilteredByAngleAndDistance;
		objFilter.filterByDistanzAndAngle(lStructFilteredMixPoints, lFilteredByAngleAndDistance, dminDistanz, dmaxDistanz, dminAngle, dmaxAngle);

		if (i == 2)
		{
			std::fstream in;
			int i = 0;
			in.open("./rec/K_SET_1_Rohdaten_nachher.asc", std::ios::out);
			for (std::list<SLaserscan>::iterator scan = lFilteredByAngleAndDistance.begin(); scan != lFilteredByAngleAndDistance.end(); ++scan)
			{
				for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
				{
					in << itPoints->x << " " << itPoints->y << " " << itPoints->z << " " << itPoints->dOrientierung << std::endl;
				}
			}
			in.close();
		}

		filterBody(lFilteredByAngleAndDistance, vlFilteredScan.at(i));
		//vlFilteredScan.at(i) = lStructFilteredMixPoints;
		lStructFilteredScans.clear();
		lStructFilteredMixPoints.clear();
		lFilteredByAngleAndDistance.clear();
		lFirstScan.clear();
		j = 0;
		A.dDistanz = NULL;
	}
	if (vlFilteredScan.at(0).size() != 0)
	{
		iReturn = 0;
	}


	return iReturn;
}

//Ermittelt die Werte aus der XML für den Encoder
int CKalibrierung::getEncorderDataFromXML(char* cPfad, std::string& ccDLLPfad, std::string& sCOM, int& iVal, int& iBaud, std::string& sParity, int& iData, int& iStop)
{
	try
	{
		int iReturn = -1;
		iReturn = CXMLParameter::GetWertAusXML_Str(cPfad, &ccDLLPfad, 4, "Thuema", "Sensoren", "Encoder", "DLL_Name");
		iReturn = CXMLParameter::GetWertAusXML_Str(cPfad, &sCOM, 4, "Thuema", "Sensoren", "Encoder", "COM");
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iBaud, 4, "Thuema", "Sensoren", "Encoder", "Baud");
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iVal, 4, "Thuema", "Sensoren", "Encoder", "Val");
		iReturn = CXMLParameter::GetWertAusXML_Str(cPfad, &sParity, 4, "Thuema", "Sensoren", "Encoder", "Parity");
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iData, 4, "Thuema", "Sensoren", "Encoder", "Data");
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iStop, 4, "Thuema", "Sensoren", "Encoder", "Stop");
		return iReturn;
	}
	catch (std::exception)
	{
		return -1;
	}

}

//Ermittelt die Werte aus der XML für die Laserscanner
int CKalibrierung::getScannerDataFromXML(char* cPfad, std::vector<ScannerInitParameter>& vScannerConfigData)
{
	try
	{
		int iReturn = -1;
		int iScannerAnzahl = 0;
		std::string sScannerKnoten;
		ScannerInitParameter tmp;
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iScannerAnzahl, 4, "Thuema", "Sensoren", "Scanner", "Anzahl");
		if (iReturn == 0)
		{
			for (int i = 0; i < iScannerAnzahl; i++)
			{
				sScannerKnoten = "Scanner_" + std::to_string(i);
				iReturn = CXMLParameter::GetWertAusXML_Str(cPfad, &tmp.ccDLLPfad, 5, "Thuema", "Sensoren", "Scanner", sScannerKnoten.c_str(), "DLL_Name");
				iReturn = CXMLParameter::GetWertAusXML_Str(cPfad, &tmp.sIP, 5, "Thuema", "Sensoren", "Scanner", sScannerKnoten.c_str(), "IP");
				iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &tmp.iPort, 5, "Thuema", "Sensoren", "Scanner", sScannerKnoten.c_str(), "Port");
				iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &tmp.iScannerID, 5, "Thuema", "Sensoren", "Scanner", sScannerKnoten.c_str(), "ScannerID");
				tmp.iAnzahlScanner = iScannerAnzahl;
				vScannerConfigData.push_back(tmp);
				tmp.ccDLLPfad.clear();
				tmp.sIP.clear();
			}
		}

		return iReturn;
	}
	catch (std::exception)
	{
		return -1;
	}
}

//Der Encoder wird initialisiert
int CKalibrierung::initEncoder(const char* ccPPfadEncoderDLL, double dVel, char* cCOMPort, int iBaud, char cParity, int iData, int iStop)
{
	int iReturn = -1;
	this->m_objEncoder = new CEncoder();
	if (true == this->m_objEncoder->initEncoder(ccPPfadEncoderDLL, cCOMPort, iBaud, cParity, iData, iStop))
	{
		iReturn = 0;
		this->m_objEncoder->setSpeed(dVel);
	}
	else
	{
		iReturn = -2;
		delete this->m_objEncoder;
	}
	return iReturn;
}

int CKalibrierung::initScanner(const char * ccPPfadScannerDLL, const char * ccIp, unsigned int uiPort, unsigned int & uiScannerID)
{
	int iReturn = -1;
	this->iAnzahlScanner++;
	CScanner* objScanner = new CScanner();
	iReturn = objScanner->initScanner(ccPPfadScannerDLL, ccIp, uiPort);
	if (0 != iReturn)
	{
		objScanner->exitScanner();
		this->bIsInitScanner = false;
		iReturn = -1;
		delete objScanner;
	}
	else
	{
		this->vobjScanner.push_back(objScanner);
	}
	return iReturn;
}

int CKalibrierung::getFilteredPoitns(std::vector<std::list<SLaserscan>>* scanOut, unsigned int filter, unsigned int iMode)
{
	SLaserscan tmp;
	int iReturn = -1;
	
	for (int i = 0; i < this->iAnzahlScanner; i++)
	{
		tmp.lScannerDaten.clear();
		tmp.lScannerZeiten.clear();
		iReturn = this->vobjScanner.at(i)->startRecording();
		if (0 == iReturn)
		{

			iReturn = this->vobjScanner.at(i)->getFilterRecordedPoints(tmp.lScannerDaten, tmp.lScannerZeiten,filter, iMode);
			tmp.dZeitstempel_Datenempfangen = CTimeStamp::getTimeStamp();
			iReturn = this->vobjScanner.at(i)->stopRecording();

		}
		else
		{
			std::cout << "Error" << std::endl;
		}
		scanOut->at(i).push_back(tmp);
	}
	
	return iReturn;
}

bool CKalibrierung::moveEncoder(double pos)
{
	return this->m_objEncoder->moveToPos(pos);
}

double CKalibrierung::getPosEncoder()
{
	double pos;
	this->m_objEncoder->getPosition(pos);
	return pos;
}



int CKalibrierung::ermittleRegressionsGeraden(std::vector<std::list<SLaserscan>*> vlslScanIn, std::vector<std::list<std::list<StructGeradenPunkte>>>& vlRegOut)
{
	int iReturn = -1;
	int iZaehler = 1;
	int iPunkteProRegGerade = 0;
	int punkteProRegGerade = 0;
	SPunkt tmp;
	std::list<SPunkt> lTmp;
	StructGeradengleichung stmp;
	StructGeradenPunkte gerade;
	std::list<StructGeradenPunkte> lGeraden;
	std::list<std::list<StructGeradenPunkte>> llGeraden;
	vlRegOut.resize(vlslScanIn.size());
	//Liste mit Regressionsgeraden
	for (int i = 0; i < vlslScanIn.size(); i++)
	{
		//std::list<SPunkt> interpoliertePunkte;
		//std::list<SLaserscan> lInterpolierterScan;
		//SLaserscan interpolierterScan;
		//std::list<double> lInterpoliertZeit;
		////Werte zwischen den Punkten interpolieren
		//for (std::list<SLaserscan>::iterator lstScan = vlslScanIn.at(i)->begin(); lstScan != vlslScanIn.at(i)->end(); ++lstScan)
		//{
		//	std::list<SPunkt>::iterator mySecondPoints = lstScan->lScannerDaten.begin();
		//	++mySecondPoints;
		//	for (std::list<SPunkt>::iterator myPoints = lstScan->lScannerDaten.begin(); myPoints != lstScan->lScannerDaten.end(); ++myPoints)
		//	{
		//		if (mySecondPoints != lstScan->lScannerDaten.end())
		//		{
		//			this->interpolation(*myPoints, *mySecondPoints, 1, interpoliertePunkte);
		//			interpoliertePunkte.push_back(*myPoints);
		//			for (unsigned int i = 0; i < 1+1; i++)
		//			{
		//				lInterpoliertZeit.push_back(lstScan->lScannerZeiten.front());
		//			}
		//			++mySecondPoints;
		//		}
		//	}
		//	interpolierterScan.dZeitstempel_Datenempfangen = lstScan->dZeitstempel_Datenempfangen;
		//	interpolierterScan.lScannerDaten = interpoliertePunkte;
		//	interpolierterScan.lScannerZeiten = lInterpoliertZeit;
		//	lInterpolierterScan.push_back(interpolierterScan);
		//	interpoliertePunkte.clear();
		//	lInterpoliertZeit.clear();
		//}
		//std::fstream interpol;
		//interpol.open("InterpolierterScan.asc", std::ios::out);
		//for (std::list<SLaserscan>::iterator lstScan = lInterpolierterScan.begin(); lstScan != lInterpolierterScan.end(); ++lstScan)
		//{
		//	for (std::list<SPunkt>::iterator myPoints = lstScan->lScannerDaten.begin(); myPoints != lstScan->lScannerDaten.end(); ++myPoints)
		//	{
		//		interpol << myPoints->x << " " << myPoints->y << " " << myPoints->z << " " << std::endl;
		//	}
		//}
		//interpol.close();

		//Berechne für jeden Scan X Regressionsgeraden und speichere die in llGeraden
		//for (std::list<SLaserscan>::iterator lstScan = lInterpolierterScan.begin(); lstScan != lInterpolierterScan.end(); ++lstScan)
		for (std::list<SLaserscan>::iterator lstScan = vlslScanIn.at(i)->begin(); lstScan != vlslScanIn.at(i)->end(); ++lstScan)
		{
			punkteProRegGerade = 4;//lstScan->lScannerDaten.size() / 12;
			for (std::list<SPunkt>::iterator myPoints = lstScan->lScannerDaten.begin();myPoints != lstScan->lScannerDaten.end();++myPoints)
			{
				if (iZaehler != lstScan->lScannerDaten.size())
				{
					if (iPunkteProRegGerade < punkteProRegGerade)
					{
						tmp.x = myPoints->x;
						tmp.y = myPoints->y;
						tmp.z = myPoints->z;
						tmp.dDistanz = myPoints->dDistanz;
						tmp.dOrientierung = myPoints->dOrientierung;
						lTmp.push_back(tmp);
						iPunkteProRegGerade++;
					}
					else
					{
						this->berechneRegGeraden(lTmp, stmp);
						gerade.lcpGeradenPunkte = lTmp;
						gerade.sgGerade = stmp;
						lTmp.clear();
						lGeraden.push_back(gerade);
						iPunkteProRegGerade = 1;
						tmp.x = myPoints->x;
						tmp.y = myPoints->y;
						tmp.z = myPoints->z;
						tmp.dDistanz = myPoints->dDistanz;
						tmp.dOrientierung = myPoints->dOrientierung;
						lTmp.push_back(tmp);
						gerade.lcpGeradenPunkte.clear();
					}
				}
				else
				{
					if (lTmp.size() != 0)
					{
						tmp.x = myPoints->x;
						tmp.y = myPoints->y;
						tmp.z = myPoints->z;
						tmp.dDistanz = myPoints->dDistanz;
						tmp.dOrientierung = myPoints->dOrientierung;
						lTmp.push_back(tmp);
						this->berechneRegGeraden(lTmp, stmp);
						gerade.lcpGeradenPunkte = lTmp;
						gerade.sgGerade = stmp;
						lTmp.clear();
						lGeraden.push_back(gerade);
						gerade.lcpGeradenPunkte.clear();
					}
				}
				iZaehler++;
			}
			iZaehler = 0;
			iPunkteProRegGerade = 0;
			llGeraden.push_back(lGeraden);
			lGeraden.clear();
		}
#ifdef _DEBUG
		std::fstream reg;
		int rotr = 0;
		int gruenr = 0;
		int blaur = 0;
		reg.open("Alle_Regressionsgeraden.asc", std::ios::out);
		for (std::list<std::list<StructGeradenPunkte>>::iterator scan = llGeraden.begin(); scan != llGeraden.end(); ++scan)
		{
			for (std::list<StructGeradenPunkte>::iterator myLine = scan->begin(); myLine != scan->end(); ++myLine)
			{
				rotr = std::rand() % 255;
				gruenr = std::rand() % 255;
				blaur = std::rand() % 255;
				for (std::list<SPunkt>::iterator punkte = myLine->lcpGeradenPunkte.begin(); punkte != myLine->lcpGeradenPunkte.end(); ++punkte)
				{
					reg << punkte->x << " " << punkte->y << " 0 " << rotr << " " << gruenr << " " << blaur << std::endl;
				}
			}
		}
		reg.close();
#endif
		std::list<std::list<std::list<StructGeradenPunkte>>> lllGeraden;
		//Schnittwinkel zwischen den Geraden berechnen
		//Füge die einzelnen Regressionsgeraden in Abhängigkeit des Schnittwinkels zusammen
		for(std::list<std::list<StructGeradenPunkte>>::iterator lScan = llGeraden.begin(); lScan != llGeraden.end();++lScan)
		{
			std::list<StructGeradenPunkte>::iterator myLineF = lScan->begin();
			myLineF++;
			std::list<StructGeradenPunkte> lGerade;
			std::list<std::list<StructGeradenPunkte>> llGerade;
			for (std::list<StructGeradenPunkte>::iterator myLine = lScan->begin();myLine != lScan->end();++myLine)
			{
				double angle = calculateAngleOfIntersection(myLine->sgGerade.dSteigung, myLineF->sgGerade.dSteigung);
				if (angle < 15)
				{
					lGerade.push_back(*myLine);
				}
				else
				{
					lGerade.push_back(*myLine);
					llGerade.push_back(lGerade);
					lGerade.clear();
				}
				if (myLineF->sgGerade.dSteigung != lScan->back().sgGerade.dSteigung)
				{
					myLineF++;
				}
		
			}
			if (lGerade.size() != 0)
			{
				llGerade.push_back(lGerade);
				lGerade.clear();
			}
			lllGeraden.push_back(llGerade);
			llGerade.clear();
		}
		llGeraden.clear();
#ifdef _DEBUG
		reg.open("RegressionTeilen_Neu_Aufbau_NachEinerStunde_median.asc", std::ios::out);
		for (std::list<std::list<std::list<StructGeradenPunkte>>>::iterator lScanGeraden = lllGeraden.begin(); lScanGeraden != lllGeraden.end(); ++lScanGeraden)
		{
			for (std::list<std::list<StructGeradenPunkte>>::iterator myMainReg = lScanGeraden->begin(); myMainReg != lScanGeraden->end(); ++myMainReg)
			{
				rotr = std::rand() % 255;
				gruenr = std::rand() % 255;
				blaur = std::rand() % 255;
				for (std::list<StructGeradenPunkte>::iterator myDisReg = myMainReg->begin(); myDisReg != myMainReg->end(); ++myDisReg)
				{
					for (std::list<SPunkt>::iterator myPoints = myDisReg->lcpGeradenPunkte.begin(); myPoints != myDisReg->lcpGeradenPunkte.end(); ++myPoints)
					{
						reg << myPoints->x << " " << myPoints->y << " 0 " << rotr << " " << gruenr << " " << blaur << std::endl;
					}
				}
			}
		}
		reg.close();
#endif
		//Berechne eine neue Regressionsgerade mit den jeweiligen Punkten der Hauptregressionsgeraden
		for(std::list<std::list<std::list<StructGeradenPunkte>>>::iterator lScanGeraden = lllGeraden.begin();lScanGeraden != lllGeraden.end();++lScanGeraden)
		{
			for (std::list<std::list<StructGeradenPunkte>>::iterator myMainReg = lScanGeraden->begin();myMainReg != lScanGeraden->end();++myMainReg)
			{
				for (std::list<StructGeradenPunkte>::iterator myDisReg = myMainReg->begin();myDisReg != myMainReg->end();++myDisReg)
				{
					for (std::list<SPunkt>::iterator myPoints = myDisReg->lcpGeradenPunkte.begin();myPoints != myDisReg->lcpGeradenPunkte.end();++myPoints)
					{
						tmp.x = myPoints->x;
						tmp.y = myPoints->y;
						tmp.z = myPoints->z;
						tmp.dDistanz = myPoints->dDistanz;
						tmp.dOrientierung = myPoints->dOrientierung;
						lTmp.push_back(tmp);
					}
				}
				this->berechneRegGeraden(lTmp, stmp);
				gerade.lcpGeradenPunkte = lTmp;
				gerade.sgGerade = stmp;
				lTmp.clear();
				lGeraden.push_back(gerade);
			}
			llGeraden.push_back(lGeraden);
			lGeraden.clear();
		}
		lGeraden.clear();
#ifdef _DEBUG
		std::fstream in,excel;
		in.open("Regresionsgeraden_Neu_Aufbau_NachEinerStunde_median.asc", std::ios::out);
		excel.open("Regression_Neu_Aufbau_NachEinerStunde_median.csv", std::ios::out);
		excel << "X;Y;Z;ROT;GRüN;BLAU;STEIGUNG" << std::endl;
		in << "X Y Z RGB-R RGB-G RGB-B" << std::endl;
		int rot = 0;
		int gruen = 0;
		int blau = 0;
		for (std::list<std::list<StructGeradenPunkte>>::iterator scan = llGeraden.begin(); scan != llGeraden.end(); ++scan)
		{
			for(std::list<StructGeradenPunkte>::iterator regression = scan->begin();regression != scan->end();++regression)
			{
				rot = std::rand() % 255;
				gruen = std::rand() % 255;
				blau = std::rand() % 255;
				for(std::list<SPunkt>::iterator points = regression->lcpGeradenPunkte.begin(); points != regression->lcpGeradenPunkte.end(); ++points)
				{
					in << points->x << " " << points->y << " " << points->z << " " 
						<< rot << " " << gruen << " " << blau << std::endl;
					std::ostringstream strs,stry,steigung;
					strs << points->x;
					stry << points->y;
					steigung << regression->sgGerade.dSteigung;
					std::string x = strs.str();
					std::string y = stry.str();
					std::string st = steigung.str();
					std::replace(x.begin(), x.end(), '.', ',');
					std::replace(y.begin(), y.end(), '.', ',');
					std::replace(st.begin(), st.end(), '.', ',');
					excel << x << ";" << y << ";" << points->z << ";"
						<< rot << ";" << gruen << ";" << blau << ";" << st << std::endl;
				}
			}
		}
		in.close();
		excel.close();
#endif
		if(llGeraden.size()!=0)
		{
			vlRegOut.at(i) = llGeraden;
			llGeraden.clear();
			iReturn = 0;
		}
		
	}
	return iReturn;
}

int CKalibrierung::calculateZenith(structParameterGerade paraIn, structScheidelpunkt& paraOut)
{
	//Scheitelpunktformel f(x)= a(x-d)^2+e 
	//Scheitelpunkt S(d/e)
	int iReturn = -1;
	//Berechne d und e 
	//Koeffizient von x^2 aus x^2 und x ausklammern
	//f(x) = a ( x2 - d/a) +e 
	double d = 0; //d == d/a
	d = paraIn.zahl2 / paraIn.zahl1;
	//Quadratische Ergänzung hinzufügen
	double a2 = pow(d / 2, 2);
	//Parameter e bestimmen
	double e = paraIn.zahl3 + (paraIn.zahl1*(-a2)); // e

	//Binomische Formel
	double de = 0;
	de = (d / 2)*(-1);
	paraOut.x = de;
	paraOut.y = e;
	if (paraOut.x != NULL)
	{
		iReturn = 0;
	}
	return iReturn;
}

int CKalibrierung::berechneRegGeraden(std::list<SPunkt> lPointsIn, StructGeradengleichung& sgGeradeOut) const
{
	int iReturn = -1;
	double dSummeX = 0;
	double dSummeY = 0;
	int iZaehler = 0;
	double dSummeYMalX = 0;
	double dSummeXQuadrad = 0;
	//Summiere alle X Werte sowie Y Werte
	for (std::list<SPunkt>::iterator lScani = lPointsIn.begin(); lScani != lPointsIn.end(); ++lScani)
	{
		dSummeX = dSummeX + lScani->x;
		dSummeY = dSummeY + lScani->y;
		iZaehler = iZaehler + 1;
		dSummeYMalX = dSummeYMalX + (lScani->x*lScani->y);
		dSummeXQuadrad = dSummeXQuadrad + pow(lScani->x, 2);
	}
	double steigung = ((iZaehler*dSummeYMalX) - dSummeX*dSummeY) / (iZaehler*dSummeXQuadrad - pow(dSummeX, 2));
	double korrel = dSummeY / iZaehler - steigung*(dSummeX / iZaehler);

	sgGeradeOut.dKorreleationskoeffiezient = korrel;
	sgGeradeOut.dSteigung = steigung;
	sgGeradeOut.dStartwertX = lPointsIn.front().x;
	sgGeradeOut.dStartWertY = lPointsIn.front().y;
	sgGeradeOut.dEndWertX = lPointsIn.back().x;
	sgGeradeOut.dEndWertY = lPointsIn.back().y;
	if (INFINITE != sgGeradeOut.dKorreleationskoeffiezient)
	{
		iReturn = 0;
	}
	return iReturn;

}

int CKalibrierung::berechneRegGeraden(std::list<SPunkt> lPointsIn, StructGeradengleichung& sgGeradeOut, char cScanRichtung)
{
	int iReturn = -1;
	double dMittelWertX = 0;
	double dMittelWertY = 0;
	int iZaehler = 0;
	//Summiere alle X Werte sowie Y Werte
	for (std::list<SPunkt>::iterator lScani = lPointsIn.begin(); lScani != lPointsIn.end(); ++lScani)
	{
		dMittelWertX = dMittelWertX + lScani->x;
		dMittelWertY = dMittelWertY + lScani->y;
		iZaehler = iZaehler + 1;
	}
	//Berechnung der Mittelwerte
	dMittelWertX = dMittelWertX / iZaehler;
	dMittelWertY = dMittelWertY / iZaehler;

	//Steigung m bestimmen, wo Summe aller quadratischen Abweichungen minimal ist
	//Summe vertikale Entferung zum Quadrat
	//f(x) = m x + y = m(x -Mittelwert(x)) + (Mittelwert(y)
	structParameterGerade p;
	std::list<structParameterGerade> lParameter;
	for (std::list<SPunkt>::iterator lScani = lPointsIn.begin(); lScani != lPointsIn.end(); ++lScani)
	{
		if (cScanRichtung == 'x')
		{
			p.zahl1 = pow(lScani->x - dMittelWertX, 2);
			p.zahl3 = pow(lScani->y - dMittelWertY, 2);
		}
		else
		{
			p.zahl3 = pow(lScani->x - dMittelWertX, 2);
			p.zahl1 = pow(lScani->y - dMittelWertY, 2);
		}
		p.zahl2 = ((lScani->x - dMittelWertX)* (lScani->y - dMittelWertY)) * 2;
		lParameter.push_back(p);
	}
	double SummeX2 = 0;
	double SummeX = 0;
	double SummeB = 0;
	for (std::list<structParameterGerade>::iterator para = lParameter.begin(); para != lParameter.end(); ++para)
	{
		if (cScanRichtung == 'x')
		{
			SummeX2 = SummeX2 + para->zahl1;
			SummeX = SummeX + para->zahl2;
			SummeB = SummeB + para->zahl3;
		}
		else
		{
			SummeX2 = SummeX2 + para->zahl3;
			SummeX = SummeX + para->zahl2;
			SummeB = SummeB + para->zahl1;
		}
	}
	if (cScanRichtung == 'x')
	{
		p.zahl1 = SummeX2;
		p.zahl2 = SummeX;
		p.zahl3 = SummeB;
	}
	else
	{
		p.zahl3 = SummeX2;
		p.zahl2 = SummeX;
		p.zahl1 = SummeB;
	}

	//Berechne Scheidelpunkt
	structScheidelpunkt tmpScheidelpunkt;
	iReturn = calculateZenith(p, tmpScheidelpunkt);
	//Scheitelpunkt in f(x) einsetzen
	double steigung = 0;
	steigung = tmpScheidelpunkt.x;
	double koeff = 0;
	if (cScanRichtung == 'x')
	{
		koeff = (tmpScheidelpunkt.x*dMittelWertX) + dMittelWertY;
	}
	else
	{
		koeff = (tmpScheidelpunkt.x*dMittelWertY) + dMittelWertX;
	}

	sgGeradeOut.dKorreleationskoeffiezient = koeff;
	sgGeradeOut.dSteigung = steigung;
	sgGeradeOut.dStartwertX = lPointsIn.front().x;
	sgGeradeOut.dStartWertY = lPointsIn.front().y;
	sgGeradeOut.dEndWertX = lPointsIn.back().x;
	sgGeradeOut.dEndWertY = lPointsIn.back().y;
	if (INFINITE != sgGeradeOut.dKorreleationskoeffiezient)
	{
		iReturn = 0;
	}
	return iReturn;



}

int CKalibrierung::ermittleRegressionsGeraden(std::list<SLaserscan>::iterator lScanIn, std::list<SLaserscan> lOut, int iMaxSize)
{
	int iReturn = -1;
	SPunkt tmp;
	std::list<SPunkt> lcpGerade;
	std::list<SPunkt> lcpZwischenGerade;
	std::list<SPunkt> lcpRegressionsPunkte;
	StructGeradenPunkte gerade;
	std::list<StructGeradenPunkte> lGerade;
	SLaserscan tmpLaserScan;
	std::list<double> lTimeStamp;
	int iZaehler = 0;
	StructGeradengleichung tmpGerade;
	StructGeradengleichung tmpGeradeZwischen;
	StructGeradengleichung RegressionsGerade;
	for (std::list<SPunkt>::iterator lcpPoints = lScanIn->lScannerDaten.begin(); lcpPoints != lScanIn->lScannerDaten.end(); ++lcpPoints)
			{
				tmp.dDistanz = lcpPoints->dDistanz;
				tmp.dOrientierung = lcpPoints->dOrientierung;
				tmp.x = lcpPoints->x;
				tmp.y = lcpPoints->y;
				tmp.z = lcpPoints->z;
				if (lcpPoints == lScanIn->lScannerDaten.end())
				{
					lcpGerade.push_back(tmp);
					gerade.lcpGeradenPunkte = lcpGerade;
					gerade.sgGerade = tmpGerade;
					lGerade.push_back(gerade);
					lcpGerade.clear();
					//----------------- Speichere alle Punkte der ersten Regressionsgeraden-------------//
					for (std::list<StructGeradenPunkte>::iterator geraden = lGerade.begin(); geraden != lGerade.end(); ++geraden)
					{
						for (std::list<SPunkt>::iterator geradenPunkte = geraden->lcpGeradenPunkte.begin(); geradenPunkte != geraden->lcpGeradenPunkte.end(); ++geradenPunkte)
						{
							tmp.dDistanz = geradenPunkte->dDistanz;
							tmp.dOrientierung = geradenPunkte->dOrientierung;
							tmp.x = geradenPunkte->x;
							tmp.y = geradenPunkte->y;
							tmp.z = geradenPunkte->z;
							lcpRegressionsPunkte.push_back(tmp);
							lTimeStamp.push_back(lScanIn->lScannerZeiten.front());
						}
					}
					lGerade.clear();
					//---------------- Speichern Ende----------------------//
					//---------------- Berechne Regressionsgerade----------//
					tmpLaserScan.dZeitstempel_Datenempfangen = lScanIn->dZeitstempel_Datenempfangen;
					tmpLaserScan.lScannerDaten = lcpRegressionsPunkte;
					tmpLaserScan.lScannerZeiten = lTimeStamp;
					lOut.push_back(tmpLaserScan);
					lcpRegressionsPunkte.clear();
					lTimeStamp.clear();
					//---------------- Berechnen Ende ---------------------//
				}
				else
				{
					iZaehler = iZaehler + 1;
					if (10 != iZaehler)
					{
						lcpGerade.push_back(tmp);
					}
					else
					{
						iZaehler = 0;
						this->berechneRegGeraden(lcpGerade, tmpGerade);

						if (0 != lGerade.size())
						{
							if (abs(lGerade.back().sgGerade.dSteigung - tmpGerade.dSteigung) < 5) // Ist die Steigung der
																								  //vorigen Geraden um 5 GE größer oder kleiner als die Steigung der neuen Gerade?
							{
								gerade.lcpGeradenPunkte = lcpGerade;
								gerade.sgGerade = tmpGerade;
								lGerade.push_back(gerade);
								lcpGerade.clear();
							}
							//Betrachtung der nächsten Gerade
							else
							{
								tmpGeradeZwischen = tmpGerade;
								for (int j = 0; j < 10; j++)
								{
									if (lcpPoints != lScanIn->lScannerDaten.end())
									{
										++lcpPoints;
										tmp.dDistanz = lcpPoints->dDistanz;
										tmp.dOrientierung = lcpPoints->dOrientierung;
										tmp.x = lcpPoints->x;
										tmp.y = lcpPoints->y;
										tmp.z = lcpPoints->z;
										lcpZwischenGerade.push_back(tmp);
									}
								}
								this->berechneRegGeraden(lcpZwischenGerade, tmpGerade);
								if (abs(tmpGeradeZwischen.dSteigung - tmpGerade.dSteigung) < 5)//Vergleich 2. und 3. Gerade
								{
									if (abs(lGerade.back().sgGerade.dSteigung - tmpGerade.dSteigung < 5))//Vergleich 1. und 3. Gerade
									{
										//Wenn beide passen, dann gehören alle Punkte zur einer Geraden
										gerade.lcpGeradenPunkte = lcpGerade;
										gerade.sgGerade = tmpGeradeZwischen;
										lGerade.push_back(gerade);
										lcpGerade.clear();
										gerade.lcpGeradenPunkte = lcpZwischenGerade;
										gerade.sgGerade = tmpGerade;
										lGerade.push_back(gerade);
										lcpZwischenGerade.clear();

									}
									else //1. u. 3. passt nicht // 1. u. 2 passt nicht  2. u. 3. passt
										 //1. gehöhrt zur ersten Regressionsgerade // 2. u. 3 gehören zur nächsten Gerade
									{
										//----------------- Speichere alle Punkte der ersten Regressionsgeraden-------------//
										for (std::list<StructGeradenPunkte>::iterator geraden = lGerade.begin(); geraden != lGerade.end(); ++geraden)
										{
											for (std::list<SPunkt>::iterator geradenPunkte = geraden->lcpGeradenPunkte.begin(); geradenPunkte != geraden->lcpGeradenPunkte.end(); ++geradenPunkte)
											{
												tmp.dDistanz = geradenPunkte->dDistanz;
												tmp.dOrientierung = geradenPunkte->dOrientierung;
												tmp.x = geradenPunkte->x;
												tmp.y = geradenPunkte->y;
												tmp.z = geradenPunkte->z;
												lcpRegressionsPunkte.push_back(tmp);
												lTimeStamp.push_back(lScanIn->lScannerZeiten.front());
											}
										}
										lGerade.clear();
										//---------------- Speichern Ende----------------------//
										//---------------- Berechne Regressionsgerade----------//
										tmpLaserScan.dZeitstempel_Datenempfangen = lScanIn->dZeitstempel_Datenempfangen;
										tmpLaserScan.lScannerDaten = lcpRegressionsPunkte;
										tmpLaserScan.lScannerZeiten = lTimeStamp;
										lOut.push_back(tmpLaserScan);
										lcpRegressionsPunkte.clear();
										lTimeStamp.clear();
										iReturn = 0;
										//---------------- Berechnen Ende ---------------------//
										//Speichere 2. Gerade
										gerade.lcpGeradenPunkte = lcpGerade;
										gerade.sgGerade = tmpGeradeZwischen;
										lGerade.push_back(gerade);
										lcpGerade.clear();
										//Speichere 3. Gerade
										gerade.lcpGeradenPunkte = lcpZwischenGerade;
										gerade.sgGerade = tmpGerade;
										lGerade.push_back(gerade);
										lcpZwischenGerade.clear();
									}
								}
								else
								{
									if (abs(lGerade.back().sgGerade.dSteigung - tmpGerade.dSteigung < 5))//1. u. 3. 
									{

										gerade.lcpGeradenPunkte = lcpZwischenGerade;
										gerade.sgGerade = tmpGerade;
										lGerade.push_back(gerade);
										lcpZwischenGerade.clear();
										gerade.lcpGeradenPunkte = lcpGerade;
										gerade.sgGerade = tmpGeradeZwischen;
										lGerade.push_back(gerade);
										lcpGerade.clear();
									}
									else
									{
										iReturn = -1;
										return iReturn;
									}

								}
							}
						}
						else
						{
							gerade.lcpGeradenPunkte = lcpGerade;
							gerade.sgGerade = tmpGerade;
							lGerade.push_back(gerade);
							lcpGerade.clear();
						}
				}
		}
	}
	return iReturn;
}

double CKalibrierung::calculateGradeOfLine(SPunkt A, SPunkt B)
{
	//Steigung einer Geraden aus zwei Punkten
	//(y2 - y1) / (x2 - x1)
	return (B.y - A.y) / (B.x + A.x);
	
}

int CKalibrierung::rotWinkelBerechnen(std::vector<std::list<std::list<StructGeradenPunkte>>> vRegIn, std::vector<SKalibrierung>& vRotWinkel)
{
	int iReturn = -1;
	//Lese geometrische Parameter aus XML
	int iTiefe = 0;
	int iHoehe = 0;
	int iBreite = 0;
	int iAnzahl = 0;
	iReturn = getParaDataFromXML(cParaPfad, iTiefe, iHoehe, iBreite, iAnzahl);
	if (iReturn != 0)
	{
		std::cout << "Kalibrierung: Der Pfad: " + std::string(cParaPfad) + " zur XML Datei ist ungültig." << std::endl;
	}
	else
	{
		vRotWinkel.resize(vRegIn.size());
		SPunkt startpunkt;
		SPunkt endpunkt;
		SPunkt C;
		double steigungGeradeC = 0;
		int iZaehler = 0;
		std::list<double> dRotX;
		std::list<double> dRotZ;
		std::list<double> dRotWinkel;
		std::list<double> ldLaenge;
		std::list<std::list<double>> lldLaenge;
		std::list<SPunkt> lSchnittpunkt;
		std::list<std::list<SPunkt>> llSchnittpunkt;
		//Gehe jeden Scanner durch
		for (int i = 0; i < vRegIn.size(); i++)
		{
			//Berechne die Schnittpunkte der (Haupt-)Regressionsgeraden
			for (std::list<std::list<StructGeradenPunkte>>::iterator lScan = vRegIn.at(i).begin(); lScan != vRegIn.at(i).end(); ++lScan)
			{
				if (lScan->size() > 1)
				{
					std::list<StructGeradenPunkte>::iterator myLineiter = lScan->begin();
					myLineiter++;
					for (std::list<StructGeradenPunkte>::iterator myLine = lScan->begin(); myLine != lScan->end(); ++myLine)
					{
						if (myLineiter != lScan->end())
						{
							lSchnittpunkt.push_back(this->calculateIntersectionPoint(myLine->sgGerade, myLineiter->sgGerade));
							++myLineiter;
						}
						else
						{
							break;
						}
					}
				}
				llSchnittpunkt.push_back(lSchnittpunkt);
				lSchnittpunkt.clear();
			}


			//Berechne die Länge zwischen den Schnittpunkten
			for (std::list<std::list<SPunkt>>::iterator Scan = llSchnittpunkt.begin(); Scan != llSchnittpunkt.end(); ++Scan)
			{
				std::list<SPunkt>::iterator SchnittpunktIter = Scan->begin();
				if (Scan->size() > 0)
				{
					SchnittpunktIter++;
				}
				for (std::list<SPunkt>::iterator Schnittpunkt = Scan->begin(); Schnittpunkt != Scan->end(); ++Schnittpunkt)
				{
					if (SchnittpunktIter != Scan->end())
					{
						ldLaenge.push_back(this->berechnerDistanz(*Schnittpunkt, *SchnittpunktIter));
						SchnittpunktIter++;
					}
				}
				lldLaenge.push_back(ldLaenge);
				ldLaenge.clear();
			}


			//Gehe jede Lange durch und finde heraus wo auf dem Körper gescannt wurde
			/*std::list<double> lTiefe;
			std::list<double> lHoehe;
			std::list<double> lBreite;
			double minTiefe = 10000;
			double minHoehe = 10000;
			for (std::list<std::list<double>>::iterator Scan = lldLaenge.begin(); Scan != lldLaenge.end(); ++Scan)
			{
				for (std::list<double>::iterator dLaengeIter = Scan->begin(); dLaengeIter != Scan->end(); ++dLaengeIter)
				{
					if (*dLaengeIter >= iTiefe   &&  *dLaengeIter < iHoehe)
					{
						lTiefe.push_back(*dLaengeIter);
						if (minTiefe > *dLaengeIter)
						{
							minTiefe = *dLaengeIter;
						}
					}
					if (*dLaengeIter >= iHoehe && *dLaengeIter >=250)
					{
						lHoehe.push_back(*dLaengeIter);
						if (minHoehe > *dLaengeIter)
						{
							minHoehe = *dLaengeIter;
						}
					}
				}
			}
			for (std::list<double>::iterator lRotX = lTiefe.begin(); lRotX != lTiefe.end(); ++lRotX)
			{
				double winkel = acos(iTiefe / *lRotX)*(180 / PI);
				if (std::isfinite(winkel))
				{
					dRotX.push_back(winkel);
				}
				
			}
			for (std::list<double>::iterator lRotX = lHoehe.begin(); lRotX != lHoehe.end(); ++lRotX)
			{
				double winkel = acos(iHoehe / *lRotX)*(180 / PI);
				if (std::isfinite(winkel))
				{
					dRotZ.push_back(winkel);
				}
			}*/
			int x = 0;
			for (std::list<std::list<double>>::iterator myLaenge = lldLaenge.begin(); myLaenge != lldLaenge.end(); ++myLaenge)
			{
				for (std::list<double>::iterator l = myLaenge->begin(); l != myLaenge->end(); ++l)
				{
					if (x == 0)
					{
						double winkel = std::acos(iTiefe / *l)*(180 / PI);
						dRotX.push_back(winkel);
					}
					else
					{
						double winkel = std::acos(iHoehe / *l)*(180 / PI);
						dRotZ.push_back(winkel);
					}
					x++;
				}
				i = 0;
			}

			double rotX = 0;
			for (std::list<double>::iterator lRotX = dRotX.begin(); lRotX != dRotX.end(); ++lRotX)
			{
				rotX = rotX + *lRotX;
			}
			vRotWinkel.at(i).dRotX = rotX / dRotX.size();
			vRotWinkel.at(i).dRotY = 0;

			double rotZ = 0;
			for (std::list<double>::iterator lRotZ = dRotZ.begin(); lRotZ != dRotZ.end(); ++lRotZ)
			{
				rotZ = rotZ + *lRotZ;
			}
			vRotWinkel.at(i).dRotZ = rotZ / dRotZ.size();


			std::fstream in;

			in.open("winkel.asc", std::ios::out);
			for (std::list<double>::iterator itPoints = dRotWinkel.begin(); itPoints != dRotWinkel.end(); ++itPoints)
			{
				in << *itPoints << std::endl;
			}
			in.close();
			std::string stdsa = std::to_string(i);
			in.open("Pepperl_u_Fuchs_Winkel.asc", std::ios::out);
			in << "MittelwertX=" + std::to_string(rotX / dRotX.size()) +
				"MittelwertY=0" +
				" MittelwertZ=" + std::to_string(rotZ / dRotZ.size()) << std::endl;
			in << std::endl;
			in << "X Y Z" << std::endl;

			std::list<double>::iterator lRotX = dRotX.begin();
			for (std::list<double>::iterator itPoints = dRotZ.begin(); itPoints != dRotZ.end(); ++itPoints)
			{
				if (lRotX != dRotX.end())
				{
					in << *lRotX << " ";
					in << 0 << " ";
					in << *itPoints << " ";
					in << std::endl;
					++lRotX;
				}
				else
				{
					in << " " << " ";
					in << 0 << " ";
					in << *itPoints << " ";
					in << std::endl;
				}
			}

			in.close();


		}
		return 0;
	}
	
}

//Berechnet den Schnittpunkt 
int CKalibrierung::berechneLotfussppunkt(StructGeradengleichung gerade, SPunkt& Start, SPunkt& End) const
{
	Start.x = (((gerade.dStartwertX + (gerade.dSteigung*gerade.dStartWertY)) - (gerade.dSteigung*gerade.dSchnittPunkt)) / (pow(gerade.dSteigung, 2) + 1));
	Start.y = gerade.dSchnittPunkt + (gerade.dSteigung * (Start.x));

	End.x = (((gerade.dEndWertX + (gerade.dSteigung*gerade.dEndWertY)) - (gerade.dSteigung*gerade.dSchnittPunkt)) / (pow(gerade.dSteigung, 2) + 1));
	End.y = gerade.dSchnittPunkt + (gerade.dSteigung * (End.x));


	return 0;
}

//Berechnet den euklidischen Abstand zwischen zwei Punkten
double CKalibrierung::berechnerDistanz(SPunkt A, SPunkt B) const
{
	return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2) + pow(A.z - B.z,2));
}


int CKalibrierung::getParaDataFromXML(char* cPfad, int& iTiefe, int& iHoehe, int& iBreite, int& iAnzahl)
{
	try
	{
		int iReturn = -1;
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iTiefe, 2, "Parameter", "Tiefe");
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iHoehe, 2, "Parameter", "Hoehe");
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iBreite, 2, "Parameter", "Breite");
		iReturn = CXMLParameter::GetWertAusXML_Int(cPfad, &iAnzahl, 2, "Parameter", "Anzahl");
		return iReturn;
	}
	catch (std::exception)
	{
		return -1;
	}
}