#include <math.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "CKalibrierung.h"

int main()
{
	
	CKalibrierung objKali;
	std::vector<std::list<SLaserscan>*> vScans;
	std::vector<SKalibrierung> sKali;
	objKali.startKalibrierung(vScans, sKali,'x', 10);
	return 0;
}