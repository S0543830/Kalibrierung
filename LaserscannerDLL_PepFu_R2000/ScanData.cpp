#include "ScanData.h"
#include <algorithm>


bool PepperlSort(const  SPolar_Pep &i, const  SPolar_Pep &j) { return i.dWinkel > j.dWinkel; }

ScanData::ScanData()
{
	m_objDebugOutput = new CDebugOutput();
	m_objDebugOutput->initDebugOutput("ScanData");
	m_cTimeStamp = new CTimeStamp();
	m_iDatensätze = 0;
}

int ScanData::parseTelegramm(std::string strPaket, double dTime)
{
	int p_iVal = -1;
	unsigned short p_usTmpScanNr = 0;
	unsigned short p_usTmpPaketNr = 0;
	unsigned short p_usNumData = 0;
	unsigned int p_uiStatus = 0;

	if (60 > strPaket.size())
	{
		return p_iVal;
	}

	// holt Paket- und Scannummer
	memcpy_s(&p_usTmpPaketNr, sizeof(unsigned short), &strPaket[12], 2);
	memcpy_s(&p_usTmpScanNr, sizeof(unsigned short), &strPaket[10], 2);

	// entpacke Teil des Headers
	memcpy_s(&m_usPacketType, sizeof(unsigned short), &strPaket[2], 2);
	memcpy_s(&m_uiScanFrequency, sizeof(unsigned int), &strPaket[34], 4);
	memcpy_s(&m_iFirstAngle, sizeof(int), &strPaket[44], 4);
	memcpy_s(&m_iAngularIncrement, sizeof(int), &strPaket[48], 4);
	memcpy_s(&m_usNumPoiScan, sizeof(unsigned short), &strPaket[38], 2);

	//Wenn erstes Paket
	if (1 == p_usTmpPaketNr)
	{
		m_usLastPackNr = p_usTmpPaketNr;
		m_usScanNumber = p_usTmpScanNr;
		p_iVal = 0;
	}
	else if ((this->m_usLastPackNr + 1) == p_usTmpPaketNr)
	{
		// Wenn Scannummern nicht uebereinstimmen, p_iVal = -2
		if (this->m_usScanNumber != p_usTmpScanNr)
		{
			p_iVal = -2;
		}
		else
		{
			m_usLastPackNr = p_usTmpPaketNr;
			p_iVal = 0;
		}
	}
	// Wenn Paket und Scannummer korrekt sind, entpacke den rest
	if (0 == p_iVal)
	{
		memcpy_s(&p_uiStatus, sizeof(unsigned int), &strPaket[30], 4);
		memcpy_s(&p_usNumData, sizeof(unsigned short), &strPaket[40], 2);

		if (0 == p_uiStatus)
		{
			unsigned short p_usAmplitude = 0;
			unsigned int p_uiDist = 0;
			int p_iIndex = 60;

			double dWinkelIncrement = m_iAngularIncrement / 10000.0f;
			double dWinkel = m_iFirstAngle / 10000.0f;
			// Unterscheidet nach Paket Typ
			switch (m_usPacketType)
			{
				// Paket Typ A
			case 65:
				for (int i = 0; i < p_usNumData; i++)
				{
					memcpy_s(&p_uiDist, sizeof(unsigned int), &strPaket[p_iIndex], 4);

					SPolar_Pep sTmp;
					sTmp.dVecDistanz.push_back(static_cast<double>(p_uiDist));
					sTmp.dWinkel = dWinkel;
					sTmp.iAnz = 1;
					sTmp.dVecTime.push_back(dTime);
					m_listMesswerte.push_back(sTmp);

					dWinkel += dWinkelIncrement;
					p_iIndex += 4;

				}
				break;
				//Paket Typ B
			case 66:
				for (int i = 0; i < p_usNumData; i++)
				{
					memcpy_s(&p_uiDist, sizeof(unsigned int), &strPaket[p_iIndex], 4);
					p_iIndex += 4;
					memcpy_s(&p_usAmplitude, sizeof(unsigned int), &strPaket[p_iIndex], 2);
					p_iIndex += 2;

					SPolar_Pep sTmp;
					sTmp.dVecDistanz.push_back(static_cast<double>(p_uiDist));
					sTmp.dWinkel = dWinkel;
					sTmp.iAnz = 1;
					sTmp.dVecTime.push_back(dTime);
					m_listMesswerte.push_back(sTmp);
					listAmplitude.push_back(p_usAmplitude);

				}
				break;
			default:
				m_objDebugOutput->debug(0, "Wrong Paket Type, Choose Type A or B ");
				p_iVal = -1;
				break;
			} //switch (m_usPacketType)
		} //if (0 == p_uiStatus)
		else
		{
			// Status ErrorCode Behandlung
		}
	} //if (0 == p_iVal)

	if (0 == this->m_usNumPoiScan)
	{
		p_iVal = -1;
	}
	else if (this->m_usNumPoiScan == this->m_listMesswerte.size())
	{
		p_iVal = 1;
	}
	// return 1 = succes, -1 = Error,-2 Error Scannummer
	return p_iVal;
}

// ermittelt CPunkte mittels Distance 
int ScanData::getPunkteUndZeit(	double dMinAngle,
										double dMaxAngle,
										double dMinDist,
										double dMaxDist,
										std::list<SPunkt> *liPunkte,
										std::list<double> *liTimes,
										int iMode,
										int iWinkelFilter)
{
	SPunkt sPunkt = { 0,0,0 };

	double dDist = 0;
	double dTime = 0;
	double dWinkel = 0;
	int iCounter = 0;
	int iWinkelCounter = 0;


	for (std::list<SPolar_Pep>::iterator it = this->m_listMesswerte.begin(); it != this->m_listMesswerte.end(); it++)
	{
		double dDistF = 0;
		double dTimeF = 0;
		double dWinkelF = 0;

		switch (iMode)
		{
			case 0: this->calcNormal(*it, &dDistF, &dWinkelF, &dTimeF); break;
			case 1: this->calcMittel(*it, &dDistF, &dWinkelF, &dTimeF); break;
			case 2: this->calcMedian(*it, &dDistF, &dWinkelF, &dTimeF); break;
			default: this->calcNormal(*it, &dDistF, &dWinkelF, &dTimeF);
		}


		dDist += dDistF;
		dTime += dTimeF;
		dWinkel += dWinkelF;
		iWinkelCounter++;

		if (iWinkelFilter == iWinkelCounter)
		{
			dDist /= iWinkelFilter;
			dTime /= iWinkelFilter;
			dWinkel /= iWinkelFilter;

			if (dDist == 0xffffffff)
			{
				dDist = 0;
			}

			if ((dWinkel >= dMinAngle) && (dWinkel <= dMaxAngle))
			{
				if ((dDist >= dMinDist) && (dDist <= dMaxDist))
				{
					sPunkt = this->calcPoint(dDist, dWinkel);
					liPunkte->push_back(sPunkt);
					liTimes->push_back(dTime);
					iCounter++;
				}
			}
			dDist = 0;
			dTime = 0;
			dWinkel = 0;
			iWinkelCounter = 0;
		}
	}
	return iCounter;
}

int ScanData::addPunkte(ScanData objScan)
{
	m_objDebugOutput->debug(0, "AddPunkte");

	this->m_listMesswerte.sort(PepperlSort);
	objScan.m_listMesswerte.sort(PepperlSort);

	int iCounter = 0;
	std::list<SPolar_Pep>::iterator it_ = this->m_listMesswerte.begin();
	std::list<SPolar_Pep>::iterator it_obj = objScan.m_listMesswerte.begin();


	while (it_obj != objScan.m_listMesswerte.end())
	{
		if (it_ == this->m_listMesswerte.end())
		{
			break;
		}

		if (it_->dWinkel == it_obj->dWinkel)
		{
			it_->dVecDistanz.push_back(it_obj->dVecDistanz.front());
			it_->dVecTime.push_back(it_obj->dVecTime.front() );
			it_->iAnz++;
			iCounter++;
			++it_;
			++it_obj;
		}
		else
		{ 
			if (it_->dWinkel < it_obj->dWinkel)
			{
				this->m_listMesswerte.push_back(*it_obj);
			}

			if( it_ != this->m_listMesswerte.end() )
			{
				it_++;
			}
			else
			{
				return 0;
			}
		}
	}

	return iCounter;
}

SPunkt ScanData::calcPoint(double dDist, double dWinkel)
{
	SPunkt sPunkt;
	sPunkt.x = dDist * cos(dWinkel * M_DEG2RAD);
	sPunkt.y = dDist * sin(dWinkel  * M_DEG2RAD);
	sPunkt.z = 0;
	sPunkt.dDistanz = dDist;
	sPunkt.dOrientierung = dWinkel;
	return sPunkt;
}

void ScanData::calcMedian(SPolar_Pep sIn, double * dDist, double * dWinkel, double * dTime)
{
	if (0 != (sIn.dVecDistanz.size() % 2))
	{
		*dDist = sIn.dVecDistanz.at((sIn.dVecDistanz.size() + 1) / 2);
		*dTime = sIn.dVecTime.at((sIn.dVecTime.size() + 1) / 2);
	}
	else
	{
		*dDist = sIn.dVecDistanz.at(sIn.dVecDistanz.size() / 2);
		*dDist += sIn.dVecDistanz.at((sIn.dVecDistanz.size() / 2) + 1);
		*dDist /= 2;

		*dTime = sIn.dVecTime.at(sIn.dVecTime.size() / 2);
		*dTime += sIn.dVecTime.at((sIn.dVecTime.size() / 2) + 1);
		*dTime /= 2;
	}

	*dWinkel = sIn.dWinkel;
}

void ScanData::calcMittel(SPolar_Pep sIn, double * dDist, double * dWinkel, double * dTime)
{
	for (int i = 0; i < sIn.dVecDistanz.size(); i++)
	{
		*dDist += sIn.dVecDistanz.at(i);
		*dTime += sIn.dVecTime.at(i);
	}

	*dDist /= sIn.dVecDistanz.size();
	*dTime /= sIn.dVecTime.size();

	*dWinkel = sIn.dWinkel;
}

void ScanData::calcNormal(SPolar_Pep sIn, double * dDist, double * dWinkel, double * dTime)
{
	if (0 < sIn.dVecDistanz.size())
	{
		*dDist = sIn.dVecDistanz.front();
	}
	else
	{
		*dDist = -1;
	}

	if (0 < sIn.dVecTime.size())
	{
		*dTime = sIn.dVecTime.front();
	}
	else
	{
		*dTime = -1;
	}

	*dWinkel = sIn.dWinkel;
}

ScanData::~ScanData(void)
{

}
