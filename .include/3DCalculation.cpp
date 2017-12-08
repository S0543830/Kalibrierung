#include "3DCalculation.h"
#include "math_na.h"
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _USE_3DLIB
	#include <3DLibCpp\3DBasic\3Dtypes.h>
	#include <3DLibCpp\DATA_OBJ\C3DObjectTypes.h>
	#include <3DLibCpp\DATA_OBJ\C3DObjectIO.h>
	#include <3DLibCpp/DATA_OBJ/C3DPointObj.h>
#endif


/************************************************************************/
/*                                                                      */
/************************************************************************/
C3DCalc::C3DCalc()
{
	m_objDebug = new CDebugOutput("C3DCalc");
	m_objDebug->setInitTime();
	this->m_dFaktor = 1;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
C3DCalc::~C3DCalc()
{
	
}
#define ROH
/************************************************************************/
/*                                                                      */
/************************************************************************/
std::list<SPunkt> C3DCalc::Calc3DCPunkt(	std::list<SLaserscan> liScans,
									std::list<SEncoder> liPos,
									double adHanConRot[3],
									double adHanConTrans[3],
									double adScannRot[3],
									double adScannTrans[3] )
{
	std::list<SPunkt> liPointOut;

	if (false == liScans.empty())
	{
		if (false == liPos.empty())
		{
			std::stringstream* strstmTextualOutput = new std::stringstream();
			*strstmTextualOutput << "HanCon Rot ";
			*strstmTextualOutput << adHanConRot[0] << " ";
			*strstmTextualOutput << adHanConRot[1] << " ";
			*strstmTextualOutput << adHanConRot[2] << std::endl;
			*strstmTextualOutput << "HanCon Trans ";
			*strstmTextualOutput << adHanConTrans[0] << " ";
			*strstmTextualOutput << adHanConTrans[1] << " ";
			*strstmTextualOutput << adHanConTrans[2] << std::endl;
			*strstmTextualOutput << "Scanner Rot ";
			*strstmTextualOutput << adScannRot[0] << " ";
			*strstmTextualOutput << adScannRot[1] << " ";
			*strstmTextualOutput << adScannRot[2] << std::endl;
			*strstmTextualOutput << "Scanner Trans ";
			*strstmTextualOutput << adScannTrans[0] << " ";
			*strstmTextualOutput << adScannTrans[1] << " ";
			*strstmTextualOutput << adScannTrans[2] << std::endl;
			this->m_objDebug->debug(0, strstmTextualOutput->str().c_str());
			delete(strstmTextualOutput);

			SPunkt cTmpPoint;
			double adEncoRotMat[3][3] = { { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };
			double adScanRotMat[3][3] = { { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };

			double adMessvektor[3]	  = { 0, 0, 0 };
			double adHanConMessvek[3] = { 0, 0, 0 };

			std::list<SEncoder>::iterator itEncBe = liPos.begin();
			std::list<SEncoder>::iterator itEnc = liPos.begin();

			double dEncOld = (itEnc->dPosition * this->m_dFaktor);
			double dAktPos = (itEnc->dPosition * this->m_dFaktor);

			for(	std::list<SLaserscan>::iterator itScans = liScans.begin();
					itScans != liScans.end();
					++itScans)
			{
				//Encoder Position suchen
				while (itEnc != liPos.end())
				{

					if (itScans->dZeitstempel_Datenempfangen <= itEnc->dZeitstempel)
					{
						if (liPos.begin() != itEnc)
						{
							itEncBe = itEnc;
							dEncOld = dAktPos;
							dAktPos = this->interpolationPosition(itEnc, --itEncBe, itScans->dZeitstempel_Datenempfangen);
							break;
						}
						else
						{
							itEncBe = itEnc;
							dEncOld = dAktPos;
							dAktPos = itEnc->dPosition;
							break;
						}

						

					}
					else
					{
						itEnc++;
					}
				}

				std::cout << dAktPos << std::endl;

				//Foerderer Bewegung hinzurechnen
				adHanConMessvek[0] = abs(dAktPos);

				//Foerderer Verschiebung hinzufügen
				adHanConMessvek[0] += adHanConTrans[0];
				adHanConMessvek[1] += adHanConTrans[1];
				adHanConMessvek[2] += adHanConTrans[2];
				
				//Rotations Matrix Foerderer
				Drei_Winkel_Matrix("xyz", adHanConRot[0], adHanConRot[1], adHanConRot[2], adEncoRotMat);
				Mult_Matrix_Vektor(adEncoRotMat, adHanConMessvek, adHanConMessvek);


				//Transformation über alle Scann Punkte der Ebene
				for(	std::list<SPunkt>::iterator itPoint = itScans->lScannerDaten.begin();
						itPoint != itScans->lScannerDaten.end();
						++itPoint)
				{
//#ifdef ROH 
//					adMessvektor[0] = itPoint->dDistanz * sin(itPoint->dOrientierung * M_DEG2RAD); //itPoint->x;
//					adMessvektor[1] = itPoint->dDistanz * cos(itPoint->dOrientierung * M_DEG2RAD); //itPoint->y;
//					adMessvektor[2] = 0; //itPoint->z;
//#else
					adMessvektor[0] = itPoint->x;
					adMessvektor[1] = itPoint->y;
					adMessvektor[2] = itPoint->z;
//#endif
					Drei_Winkel_Matrix("zyx", -adScannRot[2], -adScannRot[1], -adScannRot[0], adScanRotMat);

					Mult_Matrix_Vektor(adScanRotMat, adMessvektor, adMessvektor);
					adMessvektor[0] += adScannTrans[0];
					adMessvektor[1] += adScannTrans[1];
					adMessvektor[2] += adScannTrans[2];

					adMessvektor[0] += adHanConMessvek[0];
					adMessvektor[1] += adHanConMessvek[1];
					adMessvektor[2] += adHanConMessvek[2];

					// Punkt zurückschreiben
					cTmpPoint.x = adMessvektor[0];
					cTmpPoint.y = adMessvektor[1];
					cTmpPoint.z = adMessvektor[2];

					cTmpPoint.dOrientierung = itPoint->dOrientierung;
					cTmpPoint.dDistanz = itPoint->dDistanz;

					liPointOut.push_back(cTmpPoint);

				}
			}

		}
	}

	return liPointOut;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
#ifdef _USE_3DLIB
C3DPointObj* C3DCalc::Calc3DObj(	std::list<SLaserscan> liScans,
									std::list<SEncoder> liPos,
									double adHanConRot[3],
									double adHanConTrans[3],
									double adScannRot[3],
									double adScannTrans[3]	)
{
	auto obj3D = new C3DPointObj();
	std::stringstream sDeb;
	if (0 < liScans.size())
	{
		if (nullptr != obj3D)
		{
			T3DVECF_ARRAY & rPointsArray = obj3D->GetPoints();
			T3DVECF tmpPoint;

			std::list<SPunkt> liTmp = this->Calc3DCPunkt( liScans, liPos, adHanConRot, adHanConTrans, adScannRot, adScannTrans );
			sDeb << "Liste: " << liTmp.size() << std::endl;
			for (std::list<SPunkt>::iterator it = liTmp.begin(); it != liTmp.end(); ++it)
			{
				tmpPoint[0] = it->x;
				tmpPoint[1] = it->y;
				tmpPoint[2] = it->z;
				rPointsArray.Append(tmpPoint, 1);
			}
		}
		
		sDeb << "Object: " << obj3D->GetPointCount() << std::endl;
		
		// Nur wenn Punkte gelesen wurden, wird ein Objekt zurueckgegeben
		if (0 < obj3D->GetPointCount())
		{
			OutputDebugStringA(sDeb.str().c_str());
			return obj3D;
		}
		else
		{
			delete obj3D;
			obj3D = nullptr;
		}

	}//if scan size
		

	return obj3D;	
}
#endif

double C3DCalc::interpolationPosition(	std::list<SEncoder>::iterator itAkt,
										std::list<SEncoder>::iterator itBefor,
										double dZeitpunkt)
{
	double dTimeDiff = itAkt->dZeitstempel - itBefor->dZeitstempel;
	double dPosDiff = itAkt->dPosition - itBefor->dPosition;
	dPosDiff = (dPosDiff / dTimeDiff) * (itAkt->dZeitstempel - dZeitpunkt);

	return ((itBefor->dPosition + dPosDiff) * this->m_dFaktor);
}

int C3DCalc::Save3D(std::list<SPunkt> liPunkte, std::string sDatName)
{
	int iReturn = -1;
	std::fstream outScan;
	outScan.open(sDatName.c_str(), std::ios::out);
	if (outScan.is_open())
	{
		for (std::list<SPunkt>::iterator it = liPunkte.begin();
			it != liPunkte.end();
			++it)
		{
			outScan << it->x << " ";
			outScan << it->y << " ";
			outScan << it->z << " ";
			outScan << std::endl;
		}
		iReturn = 0;
		outScan.close();
	}
	return iReturn;
}

