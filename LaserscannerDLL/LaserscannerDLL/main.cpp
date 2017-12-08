#include "RotoScan.h"
#include <iostream>
#include <fstream>
#include <list>

int main()
{
	std::list<CPunkt> liPoints;
	std::list<double> liTimes;
	unsigned int uiID;
	std::cout << initRotoScan(&uiID,"192.168.0.31", 9090) << std::endl;

	startScanRecording(uiID);
	getRecordedScanPoints(uiID,liPoints, liTimes, false);
	stopScanRecording(uiID);
	//std::cout << getSingleScan( liPoints, liTimes ) << std::endl;


	std::fstream fOut;
	fOut.open("test2.asc", ios::out);

	if (fOut.is_open())
	{
		for (std::list<CPunkt>::iterator it = liPoints.begin();
			it != liPoints.end();
			++it)
		{
			fOut << it->x << " ";
			fOut << it->y << " ";
			fOut << it->z << std::endl;
		}
	}


	return 0;
}