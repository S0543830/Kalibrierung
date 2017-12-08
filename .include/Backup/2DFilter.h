#ifndef _2DFILTER_H_
#define _2DFILTER_H_

#include "DataTypes.h"

class C2DFilter
{
public:
	int filterByDistanzAndAngle(std::list<SLaserscan> lScan, std::list<SLaserscan>& lOut, double dMinDistanz, double dMaxDistanz, double dMinAngle, double dMaxAngle);
	int filterByDistanz(std::list<SLaserscan> lScan,std::list<SLaserscan>& lOut,double dMinDistanz, double dMaxDistanz);
	int filterByAngle(std::list<SLaserscan> lScan,std::list<SLaserscan>& lOut,double dMinAngle, double dMaxAngle);
	int filterMixPoints(std::list<SLaserscan> lScan, std::list<SLaserscan>& lOut, double dDistanz, int iAnzahlPunkte);
	int filterMixPoints(SLaserscan slScan, SLaserscan& slOut, double dDistanz, int iAnzahlPunkte);

private:
	double GetDistanz(SPunkt A, SPunkt B);
};

#endif _2DFILTER_H_