// Test.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "..\PaFR2000Scanner.h"
#include "..\..\.include\DataTypes.h"
#include <fstream>
int main()
{
	PaFR2000Scanner obj;
	SLaserscan tmp;
	std::list<SLaserscan> ltmp;
	unsigned int iHandle;
	int iReturn = -1;
	std::fstream in;
	iReturn = obj.initScanner("192.168.0.22",80, "A");
	for (unsigned int i = 0; i <10; i++)
	{
 		iReturn = obj.startScanOutput();
		if (0 != iReturn)
		{
			std::cout << "Break" << std::endl;
		}
		//Sleep(30000);
		iReturn = obj.getFilterRecordPoints(tmp.lScannerDaten, tmp.lScannerZeiten, 8,4);
		ltmp.push_back(tmp);
		tmp.lScannerDaten.clear();
		tmp.lScannerZeiten.clear();
		iReturn = obj.stopScanOutput();
		Sleep(1000);
	}
	obj.closeConnection();
	in.open("Neu_Aufbau_NachEinerStunde.asc", std::ios::out);
	in << "X Y Z Zeitstempel" << std::endl;
	for (std::list<SLaserscan>::iterator scan = ltmp.begin(); scan != ltmp.end(); ++scan)
	{
		for (std::list<SPunkt>::iterator Points = scan->lScannerDaten.begin(); Points != scan->lScannerDaten.end(); ++Points)
		{
			in << Points->x << " ";
			in << Points->y << " ";
			in << Points->z << " ";
			in << scan->lScannerZeiten.front() << std::endl;
		}
	}
	
	in.close();

    return 0;
}

