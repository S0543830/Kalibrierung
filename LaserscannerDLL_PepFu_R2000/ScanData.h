#ifndef __SCANDATA_HEADER__
#define __SCANDATA_HEADER__

#include "DebugOutput.h"
#include "TimeStamp.h"
#include "DataTypes.h"
#include <list>
#include <vector>

#ifndef M_PI_LONG
#define M_PI_LONG 3.1415926535897932384626433832795028841971693993751
#endif

#ifndef M_DEG2RAD
#define M_DEG2RAD ( M_PI_LONG/180.0 )
#endif

#ifndef M_RAD2DEG
#define M_RAD2DEG ( 180.0/M_PI_LONG )
#endif

typedef void(*pt2ParaFuncs)(SPolar_Pep, double *, double *, double *);

class ScanData
{
public:
	ScanData();
	~ScanData(void);

	/********************************************************************
	Erstellt:		08.2017
	Autor:			C.Armas Martinez
	Beschreibung: Entpackt Header und prueft zugehoerigkeit der Pakete zum Scan
				  Daten werden in Liste geschrieben = FIFO
	Parameter:
	IN string strPaket – enthaelt Paketdaten eines Scanns
	IN double dTime - enthaelt einen zur Scanzeit erstellten Zeitstempel
	OUT list m_listDistance - enthält alle Distanzen 
	OUT list m_listZeitstempel - enthölt alle Zeitstempel
	Rueckgabe:
	int – erfolgreich mit 1, -1 = Error,-2 Error Scannummer
	*********************************************************************/
	int parseTelegramm(std::string strPaket, double dTime);


	/********************************************************************
	Erstellt:		08.2017
	Autor:			C.Armas Martinez
	Beschreibung: erstellt mittels Distance ueber die Berechnung 
				  mit u.a. sin und cos die CPunkte
	Parameter:
	OUT list m_listZeitstempelSortiert
	Rueckgabe:
	list - liste mit C-Punkten
	*********************************************************************/

	int getPunkteUndZeit(	double dMinAngle,
							double dMaxAngle,
							double dMinDist,
							double dMaxDist,
							std::list<SPunkt> *liPunkte,
							std::list<double> *liTimes,
							int iMode = 0,
							int iWinkelFilter = 1 );

	/********************************************************************
	Erstellt:		08.2017
	Autor:			C.Armas Martinez
	Beschreibung: gibt m_listZeitstempelSortiert zurueck
	Parameter:
	Rueckgabe:
	list – liste mit Zeitstempeln
	*********************************************************************/
	std::list<double> getTimestamp();

	/********************************************************************
	Erstellt:		10.2017
	Autor:			M. Wolff
	Adieren von Messwerten aus einem anderen Scan Objekt
	Parameter:
	Rueckgabe:
	int Anzahl aus wievielen Objekte ddie Messwerten nun besteht
	*********************************************************************/
	int addPunkte(ScanData objScan);

	/********************************************************************
	Erstellt:		11.2017
	Autor:			M. Wolff

	Parameter:
	Rueckgabe:
	int Anz
	*********************************************************************/
	SPunkt calcPoint(double dDist, double dWinkel);

	void calcMedian(SPolar_Pep sIn, double *dDist, double *dWinkel, double *dTime);

	void calcMittel(SPolar_Pep sIn, double *dDist, double *dWinkel, double *dTime);

	void calcNormal(SPolar_Pep sIn, double *dDist, double *dWinkel, double *dTime);

private:
	CDebugOutput* m_objDebugOutput;
	CTimeStamp* m_cTimeStamp;

	std::list<unsigned short> listAmplitude;
	std::list<SPolar_Pep> m_listMesswerte;

	std::list<std::vector<SPolar_Pep>> m_liVecMessdata;

	unsigned short m_usScanNumber;
	unsigned short m_usPacketType;
	unsigned short m_usNumPoiScan;
	unsigned short m_usLastPackNr;
	unsigned int m_uiScanFrequency;
	int m_iAngularIncrement;
	int m_iFirstAngle;
	int m_iDatensätze;
};

#endif