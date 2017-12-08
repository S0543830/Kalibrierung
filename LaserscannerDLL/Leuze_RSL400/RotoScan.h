// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren 
// aus einer DLL vereinfachen. Alle Dateien in dieser DLL werden mit dem ROTOSCAN_EXPORTS-Symbol
// kompiliert, das in der Befehlszeile definiert wurde. Das Symbol darf nicht für ein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen 
// ROTOSCAN_API-Funktionen als aus einer DLL importiert, während die DLL
// mit diesem Makro definierte Symbole als exportiert ansieht.

#ifdef ROTOSCAN_EXPORTS
#define ROTOSCAN_API __declspec( dllexport )
#else
#define ROTOSCAN_API __declspec( dllimport )
#endif

// includes
#include <list>
#include "DataTypes.h"

extern "C" {
	ROTOSCAN_API int initRotoScan(unsigned int* iHandle, const char* szHostName, unsigned int uiPortNumber);
	ROTOSCAN_API int exitRotoScan(unsigned int iHandle);
	ROTOSCAN_API int setAngleFilterLimits(unsigned int iHandle, double dMinAngle, double dMaxAngle);
	ROTOSCAN_API int setDistanceFilterLimits(unsigned int iHandle, double dMinDist, double dMaxDist);
	ROTOSCAN_API int getRotoScanStatus(unsigned int iHandle);
	ROTOSCAN_API int getSingleScan(unsigned int iHandle, std::list <CPunkt>& rlstclPoints, std::list <double>& rlstdTimeStamps);
	ROTOSCAN_API int startScanRecording(unsigned int iHandle);
	ROTOSCAN_API int stopScanRecording(unsigned int iHandle);
	ROTOSCAN_API int getRecordedScanPoints(unsigned int iHandle, std::list <CPunkt>& rlstclPoints, std::list <double>& rlstdTimeStamps, bool bInterpoleateTimeStamps);
	ROTOSCAN_API double getTimeStamp(unsigned int iHandle);
}
