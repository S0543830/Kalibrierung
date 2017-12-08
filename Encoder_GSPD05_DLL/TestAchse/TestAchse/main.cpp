#include "EncoderGSP.h"
#include <Windows.h>
#include <iostream>

int main()
{
	if (true == init("COM7",9600,'N',8,1))
	{
		double dSpeed = 500;
		double dSpeedB = -500;
		double dPosOld = -1;
		double dPosNew = 0;
		for( int i = 0; i < 1; i++ )
		{
			setVel(dSpeed);
			getPos(&dPosNew);
			std::cout <<dPosNew<< std::endl;
			startMovement();
			Sleep(500);
			do
			{
				std::cout << dPosOld << "-->"<<dPosNew << std::endl;
				dPosOld = dPosNew;
				getPos(&dPosNew);
				Sleep(1);

			} while (dPosOld != dPosNew);
		
			Sleep(100);
			setVel(dSpeedB);
			getPos(&dPosNew);
			startMovement();
			Sleep(100);
			do
			{
				std::cout << dPosOld << "-->"<<dPosNew << std::endl;
				dPosOld = dPosNew;
				getPos(&dPosNew);
				Sleep(1);

			} while (dPosOld != dPosNew);
		}
		
	}


	Sleep(10000);
	return 0;
}