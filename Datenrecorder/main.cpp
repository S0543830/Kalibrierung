#include "stdafx.h"
#include "Datenrecorder.h"
#include "CKalibrierung.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#define Filter



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


int writeASC(std::vector<std::list<std::list<StructGeradenPunkte>>> aktScan)
{
	std::fstream in;
	int j = 0;
	for (int i = 0; i < aktScan.size(); i++)
	{
		for (std::list<std::list<StructGeradenPunkte>>::iterator scan = aktScan.at(i).begin(); scan != aktScan.at(i).end(); ++scan)
		{
			std::string stdsa = std::to_string(j);
			in.open("Regressionsgerade_" + stdsa + ".asc", ios::app);
			in << "X Y Zeitstempel" << endl;
			for (std::list<StructGeradenPunkte>::iterator itPoints = scan->begin(); itPoints != scan->end(); ++itPoints)
			{
				for (std::list<SPunkt>::iterator points = itPoints->lcpGeradenPunkte.begin(); points != itPoints->lcpGeradenPunkte.end(); ++points)
				{
					in << points->x << " " << points->y << " " << itPoints->sgGerade.dSteigung << endl;
				}
			}
			in.close();
			j++;
		}
		
	}


	return 0;
}

int writeASC(std::vector<std::list<SLaserscan>*> aktScan)
{
	std::fstream in;

	for (int i = 0; i < aktScan.size(); i++)
	{
		std::string stdsa = std::to_string(i);
		in.open("Neu_Aufbau_" + stdsa + "_500.asc", ios::out);
		in << "X Y Z Distanz Orientierung Zeitstempel" << endl;
		for (std::list<SLaserscan>::iterator scan = aktScan.at(i)->begin(); scan != aktScan.at(i)->end(); ++scan)
		{
			for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
			{
				in << itPoints->x << " ";
				in << itPoints->y << " ";
				in << itPoints->z << " ";
				in << itPoints->dDistanz << " ";
				in << itPoints->dOrientierung << " ";
				in <<  scan->lScannerZeiten.front() << endl;
			}
		}
		in.close();
	}


	return 0;
}

int writeASC(std::vector<std::list<SLaserscan>> aktScan, int iZahl)
{
	std::fstream in;
	int i = 0;
	for (std::list<SLaserscan>::iterator scan = aktScan.at(0).begin(); scan != aktScan.at(0).end(); ++scan)
	{
		std::string stdsa = std::to_string(i);
		in.open("./rec/K_SET_1_Rohdaten_" + stdsa + ".asc", ios::app);

		for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
		{
			in << itPoints->x << " " << itPoints->y << " " << scan->lScannerZeiten.front() << endl;
		}
		i++;
		in.close();
	}
	return 0;
}

int writeASC(std::vector<std::list<SLaserscan>*> aktScan,int iZahl)
{
	std::fstream in;
	int i = 0;
	for (std::list<SLaserscan>::iterator scan = aktScan.at(0)->begin(); scan != aktScan.at(0)->end(); ++scan)
	{
		std::string stdsa = std::to_string(i);
		in.open("rec\K_SET_1_Rohdaten_" + stdsa + ".asc", ios::app);

		for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
		{
			in << itPoints->x << " " << itPoints->y << " " << scan->lScannerZeiten.front() << endl;
		}
		i++;
		in.close();
	}
	return 0;
}


int writeASC(SLaserscan aktScan)
{
	std::fstream in;
	in.open("SingleScan.asc", ios::app);
		in << "X Y Zeitstempel" << endl;
		for (std::list<SPunkt>::iterator itPoints = aktScan.lScannerDaten.begin(); itPoints != aktScan.lScannerDaten.end(); ++itPoints)
		{
			in << itPoints->x << " " << itPoints->y << " " << aktScan.lScannerZeiten.front() << endl;
		}
		in.close();
	


	return 0;
}

int writeASC(std::vector<std::list<SLaserscan>> aktScan)
{
	std::fstream in;

	for (int i = 0; i < aktScan.size(); i++)
	{
		std::string stdsa = std::to_string(i);
		in.open("Kalibrierung_Filter_" + stdsa + ".asc", ios::out);
		in << "X Y Zeitstempel" << endl;
		for (std::list<SLaserscan>::iterator scan = aktScan.at(i).begin(); scan != aktScan.at(i).end(); ++scan)
		{
			for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
			{
				in << itPoints->x << " " << itPoints->y << " " << scan->lScannerZeiten.front() << endl;
			}
		}
		in.close();
	}


	return 0;
}

int writeASC(std::vector<SLaserscan> aktScan)
{
	std::fstream in;

	for (int i = 0; i < aktScan.size(); i++)
	{
		std::string stdsa = std::to_string(i);
		in.open("Datenrecording_" + stdsa + ".asc", ios::app);
		in << "X Y Zeitstempel" << endl;

			for (std::list<SPunkt>::iterator itPoints = aktScan.at(i).lScannerDaten.begin(); itPoints != aktScan.at(i).lScannerDaten.end(); ++itPoints)
			{
				in << itPoints->x << " " << itPoints->y << " " << aktScan.at(i).lScannerZeiten.front() << endl;
			}
		
		in.close();
	}


	return 0;
}

int writeASC(SLaserscan aktScan,std::string sId)
{
	std::fstream in;

		in.open("Datenrecording_"+ sId+ ".asc", ios::app);
		in << "X Y Zeitstempel" << endl;

		for (std::list<SPunkt>::iterator itPoints = aktScan.lScannerDaten.begin(); itPoints != aktScan.lScannerDaten.end(); ++itPoints)
		{
			in << itPoints->x << " " << itPoints->y << " " << aktScan.lScannerZeiten.front() << endl;
		}

		in.close();
	


	return 0;
}

int writeASC(std::list<SEncoder> lEncorder)
{
	std::fstream in;
	in.open("Encoder.asc", ios::app);
	in << "Position Zeitstempel" << endl;
	for (std::list<SEncoder>::iterator scan = lEncorder.begin(); scan != lEncorder.end(); ++scan)
	{
		in << scan->dPosition << " " << scan->dZeitstempel << endl;
	}
	in.close();
	return 0;
}

int readASC(std::vector<std::list<SLaserscan>*>& vOut)
{
	std::fstream inm;
	std::string line;
	SPunkt a;
	SLaserscan strLaser;
	std::list < SLaserscan>* lstrLaser;
	std::list<SPunkt> lPunkte;
	std::list<double> lZeitstempel;
	double x = 0;
	double y = 0;
	double z = 0;
	int i = 0;
	int iReturn = -1;
	vOut.resize(3);
	for (int j = 0; j < 3; j++)
	{
		std::string stdsa = std::to_string(j);
		//inm.open("Kalibrierung_" + stdsa + ".asc");
		//inm.open("K_SET_1_Rohdaten_"+stdsa+".asc");
		inm.open("SET_3_" + stdsa + ".asc");
		lstrLaser = new std::list<SLaserscan>();
		if (inm.is_open())
		{
			std::getline(inm, line);
			while (std::getline(inm, line))
			{
				if (i != 700)
				{
					std::vector<std::string> elems;
					elems = split(line, " ");
					a.x = fromString<double>(elems[0]);
					a.y = fromString<double>(elems[1]);
					a.z = 0;
					lPunkte.push_back(a);
					lZeitstempel.push_back(fromString<double>(elems[2]));
					i++;
				}
				else
				{
					strLaser.dZeitstempel_Datenempfangen = lZeitstempel.front();
					strLaser.lScannerDaten = lPunkte;
					strLaser.lScannerZeiten = lZeitstempel;
					lstrLaser->push_back(strLaser);
					lPunkte.clear();
					lZeitstempel.clear();
					i = 0;
				}
			}
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


int readASC(SLaserscan& vOut)
{
	std::fstream inm;
	std::string line;
	SPunkt a;
	std::list<SPunkt> lPunkte;
	std::list<double> lZeitstempel;
	int iReturn = -1;
	inm.open("ScanOne.asc");
	if (inm.is_open())
	{
		std::getline(inm, line);
		while (std::getline(inm, line))
		{
			
			std::vector<std::string> elems;
			elems = split(line, " ");
			a.x = fromString<double>(elems[0]);
			a.y = fromString<double>(elems[1]);
			a.z = 0;
			lPunkte.push_back(a);
			lZeitstempel.push_back(fromString<double>(elems[2]));
		}
		
	}
	inm.close();
	vOut.lScannerDaten = lPunkte;
	vOut.dZeitstempel_Datenempfangen = lZeitstempel.front();
	vOut.lScannerZeiten = lZeitstempel;
	lPunkte.clear();
	lZeitstempel.clear();
	if (vOut.lScannerDaten.size() != 0)
	{
		iReturn = 0;
	}
	return iReturn;
}

int main()
{

	CDatenrecorder* objDatenrecorder = new CDatenrecorder();
	std::vector<std::list<SLaserscan>*> vScan;
	std::vector<std::list<SLaserscan>> vFilterScan,vFilterDistanz,vFilterOrient ;
	SLaserscan stScan,stOut;
	std::list<SEncoder> lEncoder;
	std::list<SLaserscan> liScan;
	std::vector<SLaserscan> vSingle;
	std::vector<SLaserscan> vFilterscan2;
	double dActPos=0;
	#ifdef _DEBUG
		objDatenrecorder->initSystemOverXML("../.config/Thuema_Config_Debug.xml");
	#else
		objDatenrecorder->initSystemOverXML("../.config/Thuema_Config_Release.xml");
	#endif

//	double dScanBegin = CTimeStamp::getTimeStamp();
	objDatenrecorder->StartRecording();
	while (dActPos <= 1485-350)
	{
		objDatenrecorder->getActEncorderPos(dActPos);
	}
	objDatenrecorder->StopRecording();
	//double dScanStop = CTimeStamp::getTimeStamp();
	objDatenrecorder->getRecEncoderData(lEncoder);
	objDatenrecorder->getRecAllScanData(vScan);
	writeASC(vScan);
	writeASC(lEncoder);

#if Zeitmessung
	double dbegin = vScan.at(2)->front().lScannerZeiten.front();
	double dend = vScan.at(2)->back().lScannerZeiten.back();
	double dAdSec = vScan.at(2)->size() / (dend - dbegin);
	std::cout << "Zeitstempel (ZS) des 1. Scans: " << dbegin << endl;
	std::cout << "Zeitstempel (ZS) des letzten Scans: " << dend << endl;
	std::cout << "Zeitdifferenz: " << dend - dbegin << endl;
	std::cout << "ZeitDiffScan: " << dScanStop - dScanBegin << endl;
	std::cout << "Anzahl der Scans: " << vScan.at(2)->size() << endl;
	std::cout << "Anzahl Scan / ( ZS vom letzten Scan - ZS vom 1. Scan ): " << dAdSec << endl;
	std::cin >> dAdSec;
#endif

	//objDatenrecorder->getActSingleScan(stScan,2);

#if Filter 0
	C2DFilter objFiler;
	double dMinDistanz = 0;
	double dMaxDistanz = 0;
	double dMinAngle = 0;
	double dMaxAngle = 0;
	vFilterScan.resize(vScan.size());
	vFilterDistanz.resize(vScan.size());
	vFilterOrient.resize(vScan.size());
	for (int i = 0; i < vScan.size(); i++)
	{
		if (i == 0) { dMinDistanz = 950; dMaxDistanz = 1450; dMinAngle = 60; dMaxAngle = 115; }
		if (i == 1) { dMinDistanz = 1000; dMaxDistanz = 1600; dMinAngle = 60; dMaxAngle = 100; }
		if (i == 2) { dMinDistanz = 300; dMaxDistanz = 600; dMinAngle = -120; dMaxAngle = 120; }
		objFiler.FilterMixPoints(*vScan.at(i), vFilterScan.at(i),15,2);
		objFiler.FilterByDistanz(vFilterScan.at(i), vFilterDistanz.at(i), dMinDistanz, dMaxDistanz);
		objFiler.FilterByAngle(vFilterDistanz.at(i), vFilterOrient.at(i), dMinAngle, dMaxAngle);
	}
#endif

#if Kalibrierung
	std::vector<std::list<SLaserscan>> vRegGeradeX, vRegeradeY;
	std::vector<std::list<structGeradengleichung>> vGeradengleichung;
	ermittleRegressionsPunkteXYEbene(vFilterOrient, vRegGeradeX, vRegeradeY);
	std::vector<double> vRotWinkelX;
	berechneRegGeraden(vRegGeradeX, vGeradengleichung);
	RotWinkelBerechnen(vGeradengleichung, 250, vRotWinkelX);
#endif


	//std::vector<std::list<SLaserscan>*> vScans;
	//std::vector<std::list<SLaserscan>> vFilteredScanOut;
	//SLaserscan strScan;
	//std::list<SLaserscan> asd;
	//if(readASC(vScans)==0)
	//{
	//	std::vector<std::list<std::list<StructGeradenPunkte>>> vlRegressionsGeraden;
	//	CKalibrierung objKali;
	//	/*objKali.filterBodyFromScan(vScans, vFilteredScanOut);
	//	writeASC(vFilteredScanOut);*/
	//	//writeASC(vlRegressionsGeraden);
	//	std::vector<std::list<SLaserscan>> lScan;
	//	std::vector<SKalibrierung> sKali;
	//	
	//	objKali.startKalibrierung(vScans, sKali, 'x');
	//	
	//}
	//else
	//{
	//	printf("Error Read ASC");
	//}
	

	return 0;
}