#include "2DFilter.h"
#include <vector>
#include <list>
#include <string>
using namespace std;

//Filtere nach Ditanz in mm und Winkel in Grad
int C2DFilter::filterByDistanzAndAngle(std::list<SLaserscan> lScan,std::list<SLaserscan>& lOut,double dMinDistanz, double dMaxDistanz, double dMinAngle, double dMaxAngle)
{
	SPunkt tmp;
	std::list<SPunkt> lTmpScan;
	std::list<double> lTmpZeit;
	SLaserscan stScan;
	//Gehe jeden Scan durch
	for (std::list<SLaserscan>::iterator scan = lScan.begin(); scan != lScan.end(); ++scan)
	{
		//Gehe jeden Punkt des Scans durch
		for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
		{
			//Wenn der Punkt innerhalb der Toleranz liegt, wird er zur Liste hinzugefügt
			if( (dMinDistanz<= itPoints->dDistanz) && (dMaxDistanz >= itPoints->dDistanz) && (dMinAngle <= itPoints->dOrientierung) && (dMaxAngle >= itPoints->dOrientierung))
			{
				tmp.dDistanz = itPoints->dDistanz;
				tmp.dOrientierung = itPoints->dOrientierung;
				tmp.x = itPoints->x;
				tmp.y = itPoints->y;
				lTmpZeit.push_back(scan->lScannerZeiten.front());
				lTmpScan.push_back(tmp);
			}
		}
		if (0 != lTmpScan.size())
		{
			stScan.dZeitstempel_Datenempfangen = scan->dZeitstempel_Datenempfangen;
			stScan.lScannerDaten = lTmpScan;
			stScan.lScannerZeiten = lTmpZeit;
			lOut.push_back(stScan);
			stScan.lScannerDaten.clear();
			stScan.lScannerZeiten.clear();
			lTmpZeit.clear();
			lTmpScan.clear();
		}
		
	}
	return 0;
}

//Filtere nach Distanz in mm
int C2DFilter::filterByDistanz(std::list<SLaserscan> lScan,std::list<SLaserscan>& lOut,double dMinDistanz, double dMaxDistanz)
{
	SPunkt tmp;
	std::list<SPunkt> lTmpScan;
	std::list<double> lTmpZeit;
	SLaserscan stScan;
	//Gehe jeden Scan durch
	for (std::list<SLaserscan>::iterator scan = lScan.begin(); scan != lScan.end(); ++scan)
	{
		//Gehe jeden Punkt des Scans durch
		for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
		{
			//Wenn der Punkt innerhalb der Toleranz liegt, wird er zur Liste hinzugefügt
			if( (dMinDistanz<= itPoints->dDistanz) && (dMaxDistanz >= itPoints->dDistanz))
			{
				tmp.dDistanz = itPoints->dDistanz;
				tmp.dOrientierung = itPoints->dOrientierung;
				tmp.x = itPoints->x;
				tmp.y = itPoints->y;
				lTmpZeit.push_back(scan->lScannerZeiten.front());
				lTmpScan.push_back(tmp);
			}
		}
		if (0 != lTmpScan.size())
		{
			stScan.dZeitstempel_Datenempfangen = scan->dZeitstempel_Datenempfangen;
			stScan.lScannerDaten = lTmpScan;
			stScan.lScannerZeiten = lTmpZeit;
			lOut.push_back(stScan);
			stScan.lScannerDaten.clear();
			stScan.lScannerZeiten.clear();
			lTmpZeit.clear();
			lTmpScan.clear();
		}
	}
	return 0;
}

//Filtere nach Winkel in Grad
int C2DFilter::filterByAngle(std::list<SLaserscan> lScan,std::list<SLaserscan>& lOut,double dMinAngle, double dMaxAngle)
{
	SPunkt tmp;
	std::list<SPunkt> lTmpScan;
	std::list<double> lTmpZeit;
	SLaserscan stScan;
	//Gehe jeden Scan durch
	for (std::list<SLaserscan>::iterator scan = lScan.begin(); scan != lScan.end(); ++scan)
	{
		//Gehe jeden Punkt des Scans durch
		for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
		{
			//Wenn der Punkt innerhalb der Toleranz liegt, wird er zur Liste hinzugefügt
			if( (dMinAngle<= itPoints->dOrientierung) && (dMaxAngle >= itPoints->dOrientierung))
			{
				tmp.dDistanz = itPoints->dDistanz;
				tmp.dOrientierung = itPoints->dOrientierung;
				tmp.x = itPoints->x;
				tmp.y = itPoints->y;
				lTmpZeit.push_back(scan->lScannerZeiten.front());
				lTmpScan.push_back(tmp);
			}
		}
		if (0 != lTmpScan.size())
		{
			stScan.dZeitstempel_Datenempfangen = scan->dZeitstempel_Datenempfangen;
			stScan.lScannerDaten = lTmpScan;
			stScan.lScannerZeiten = lTmpZeit;
			lOut.push_back(stScan);
			stScan.lScannerDaten.clear();
			stScan.lScannerZeiten.clear();
			lTmpZeit.clear();
			lTmpScan.clear();
		}
		
	}
	return 0;
}

//Filter von Mix Punkten eines Scans
//Distanz in mm
int C2DFilter::filterMixPoints(std::list<SLaserscan> lScan, std::list<SLaserscan>& lOut, double dDistanz = 15, int iAnzahlPunkte = 2)
{
	SPunkt A;
	SPunkt B;
	SPunkt C;
	std::list<SPunkt> lTmpScan;
	std::list<SPunkt> lEbenenScan;
	vector<SPunkt> vMixPoints;
	std::list<double> lTmpZeit;
	SLaserscan stScan;
	int i = 0;
	double dActDis = 0;
	bool bSeeNeightbar = false;
	//Gehe jeden Scan durch
		for (std::list<SLaserscan>::iterator scan = lScan.begin(); scan != lScan.end(); ++scan)
		{
			i = 0; //Zähler zurücksetzen

			//Gehe jeden Punkt des Scans durch
			for (std::list<SPunkt>::iterator itPoints = scan->lScannerDaten.begin(); itPoints != scan->lScannerDaten.end(); ++itPoints)
			{
				//Wenn es der erste Punkt ist, dann speichere ihn in A
				if (0 == i)//Wenn es der erste Punkt der Scanebene ist, speichere den Punkt
				{
					A.dDistanz = itPoints->dDistanz;
					A.dOrientierung = itPoints->dOrientierung;
					A.x = itPoints->x;
					A.y = itPoints->y;
					A.z = itPoints->z;
				}
				else //Ansonsten vergleiche Punkt A mit dem nächsten Punkt (B)
				{
					B.dDistanz = itPoints->dDistanz;
					B.dOrientierung = itPoints->dOrientierung;
					B.x = itPoints->x;
					B.y = itPoints->y;
					B.z = itPoints->z;
					dActDis = GetDistanz(A, B);
					if (dActDis >= dDistanz)
					{
						if (true == bSeeNeightbar)
						{
							if (lTmpScan.size() <= iAnzahlPunkte)
							{
								lTmpScan.clear();
								//lTmpScan.push_back(B);
								bSeeNeightbar = false;
								A = B;
							}
							else
							{
								A = B;
								bSeeNeightbar = false;
								for (std::list<SPunkt>::iterator liEbenenScan = lTmpScan.begin(); liEbenenScan != lTmpScan.end(); ++liEbenenScan)
								{
									C.dDistanz = liEbenenScan->dDistanz;
									C.dOrientierung = liEbenenScan->dOrientierung;
									C.x = liEbenenScan->x;
									C.y = liEbenenScan->y;
									lEbenenScan.push_back(C);
									lTmpZeit.push_back(scan->lScannerZeiten.front());
								}
								lTmpScan.clear();
							}
						}
						else
						{
							lTmpScan.push_back(A);
							bSeeNeightbar = true;
							A = B;
							if (lTmpScan.size() <= iAnzahlPunkte)
							{
								lTmpScan.clear();
							}
							else
							{
								for (std::list<SPunkt>::iterator liEbenenScan = lTmpScan.begin(); liEbenenScan != lTmpScan.end(); ++liEbenenScan)
								{
									C.dDistanz = liEbenenScan->dDistanz;
									C.dOrientierung = liEbenenScan->dOrientierung;
									C.x = liEbenenScan->x;
									C.y = liEbenenScan->y;
									lEbenenScan.push_back(C);
									lTmpZeit.push_back(scan->lScannerZeiten.front());
								}
								lTmpScan.clear();
							}
						}
					}
					else
					{
						if (true == bSeeNeightbar)
						{
							if (lTmpScan.size() <= iAnzahlPunkte)
							{
								lTmpScan.clear();
								lTmpScan.push_back(A);
								bSeeNeightbar = false;
							}
							else
							{
								for (std::list<SPunkt>::iterator liEbenenScan = lTmpScan.begin(); liEbenenScan != lTmpScan.end(); ++liEbenenScan)
								{
									C.dDistanz = liEbenenScan->dDistanz;
									C.dOrientierung = liEbenenScan->dOrientierung;
									C.x = liEbenenScan->x;
									C.y = liEbenenScan->y;
									lEbenenScan.push_back(C);
									lTmpZeit.push_back(scan->lScannerZeiten.front());
								}
								bSeeNeightbar = false;
								lTmpScan.clear();
								lTmpScan.push_back(A);
								A = B;
							}
						}
						else
						{
							lTmpScan.push_back(A);
							A = B;
						}
					}
				}
				i++;
			}

			for (std::list<SPunkt>::iterator liEbenenScan = lTmpScan.begin(); liEbenenScan != lTmpScan.end(); ++liEbenenScan)
			{
				C.dDistanz = liEbenenScan->dDistanz;
				C.dOrientierung = liEbenenScan->dOrientierung;
				C.x = liEbenenScan->x;
				C.y = liEbenenScan->y;
				lEbenenScan.push_back(C);
				lTmpZeit.push_back(scan->lScannerZeiten.front());
			}
			bSeeNeightbar = false;
			if (0 != lEbenenScan.size())
			{
				lTmpScan.clear();
				stScan.dZeitstempel_Datenempfangen = scan->dZeitstempel_Datenempfangen;
				stScan.lScannerDaten = lEbenenScan;
				stScan.lScannerZeiten = lTmpZeit;
				lOut.push_back(stScan);
				stScan.lScannerDaten.clear();
				stScan.lScannerZeiten.clear();
				lEbenenScan.clear();
				lTmpZeit.clear();
				lTmpScan.clear();
			}
			
		}
	return 0;
}

//Filter von Mix Punkten einer einzigen Scanebene
//Distanz in mm
int C2DFilter::filterMixPoints(SLaserscan slScan, SLaserscan& slOut, double dDistanz, int iAnzahlPunkte)
{
	SPunkt A;
	SPunkt B;
	SPunkt C;
	std::list<SPunkt> lTmpScan;
	std::list<SPunkt> lEbenenScan;
	std::vector<SPunkt> vMixPoints;
	std::list<double> lTmpZeit;
	SLaserscan stScan;
	int i = 0;
	double dActDis = 0;
	bool bSeeNeightbar = false;
	
	
			   //Gehe jeden Punkt des Scans durch
		for (std::list<SPunkt>::iterator itPoints = slScan.lScannerDaten.begin(); itPoints != slScan.lScannerDaten.end(); ++itPoints)
		{
			if (0 == i)//Wenn es der erste Punkt der Scanebene ist, speichere den Punkt
			{
				A.dDistanz = itPoints->dDistanz;
				A.dOrientierung = itPoints->dOrientierung;
				A.x = itPoints->x;
				A.y = itPoints->y;
			}
			else //Ansonsten vergleiche Punkt A mit dem nächsten Punkt (B)
			{
				B.dDistanz = itPoints->dDistanz;
				B.dOrientierung = itPoints->dOrientierung;
				B.x = itPoints->x;
				B.y = itPoints->y;
				dActDis = GetDistanz(A, B);
				if (dActDis >= dDistanz)
				{
					if (true == bSeeNeightbar)
					{
						if (lTmpScan.size() <= iAnzahlPunkte)
						{
							lTmpScan.clear();
							lTmpScan.push_back(B);
							bSeeNeightbar = false;
							A = B;
						}
						else
						{
							A = B;
							for (std::list<SPunkt>::iterator liEbenenScan = lTmpScan.begin(); liEbenenScan != lTmpScan.end(); ++liEbenenScan)
							{
								C.dDistanz = liEbenenScan->dDistanz;
								C.dOrientierung = liEbenenScan->dOrientierung;
								C.x = liEbenenScan->x;
								C.y = liEbenenScan->y;
								lEbenenScan.push_back(C);
								lTmpZeit.push_back(slScan.lScannerZeiten.front());
							}
							bSeeNeightbar = false;
							lTmpScan.clear();
						}
					}
					else
					{
						lTmpScan.push_back(A);
						bSeeNeightbar = true;
						A = B;
					}
				}
				else
				{
					if (true == bSeeNeightbar)
					{
						if (lTmpScan.size() <= iAnzahlPunkte)
						{
							lTmpScan.clear();
							lTmpScan.push_back(A);
							bSeeNeightbar = false;
						}
						else
						{
							A = B;
							for (std::list<SPunkt>::iterator liEbenenScan = lTmpScan.begin(); liEbenenScan != lTmpScan.end(); ++liEbenenScan)
							{
								C.dDistanz = liEbenenScan->dDistanz;
								C.dOrientierung = liEbenenScan->dOrientierung;
								C.x = liEbenenScan->x;
								C.y = liEbenenScan->y;
								lEbenenScan.push_back(C);
								lTmpZeit.push_back(slScan.lScannerZeiten.front());
							}
							bSeeNeightbar = false;
							lTmpScan.clear();
						}
					}
					else
					{
						lTmpScan.push_back(A);
						A = B;
					}
				}
			}
			i++;
		}
		for (std::list<SPunkt>::iterator liEbenenScan = lTmpScan.begin(); liEbenenScan != lTmpScan.end(); ++liEbenenScan)
		{
			C.dDistanz = liEbenenScan->dDistanz;
			C.dOrientierung = liEbenenScan->dOrientierung;
			C.x = liEbenenScan->x;
			C.y = liEbenenScan->y;
			lEbenenScan.push_back(C);
			lTmpZeit.push_back(slScan.lScannerZeiten.front());
		}
		bSeeNeightbar = false;
		if (0 != lEbenenScan.size())
		{
			lTmpScan.clear();
			stScan.dZeitstempel_Datenempfangen = slScan.dZeitstempel_Datenempfangen;
			stScan.lScannerDaten = lEbenenScan;
			stScan.lScannerZeiten = lTmpZeit;
			slOut = stScan;
			stScan.lScannerDaten.clear();
			stScan.lScannerZeiten.clear();
			lTmpScan.clear();
			lEbenenScan.clear();
			lTmpScan.clear();
		}
		
	
	return 0;
}

//Distanz zwischen zwei Punkten in Millimeter
double C2DFilter::GetDistanz(SPunkt A, SPunkt B)
{
	return sqrt(    pow(A.x - B.x,2) +   pow(A.y - B.y,2)    );//in mm
}