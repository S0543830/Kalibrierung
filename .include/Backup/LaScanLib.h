// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren 
// aus einer DLL vereinfachen. Alle Dateien in dieser DLL werden mit dem ROTOSCAN_EXPORTS-Symbol
// kompiliert, das in der Befehlszeile definiert wurde. Das Symbol darf nicht für ein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen 
// LASCANLIB_API-Funktionen als aus einer DLL importiert, während die DLL
// mit diesem Makro definierte Symbole als exportiert ansieht.

#ifdef LASCANLIB_EXPORTS
#define LASCANLIB_API __declspec( dllexport )
#else
#define LASCANLIB_API __declspec( dllimport )
#endif

// includes
#include <list>
#include "DataTypes.h"

extern "C" {
	LASCANLIB_API int initLaScanLib( unsigned int* iHandle, const char* szHostName, unsigned int uiPortNumber);
	LASCANLIB_API int exitLaScanLib(unsigned int iHandle);
	LASCANLIB_API int setAngleFilterLimits(unsigned int iHandle, double dMinAngle, double dMaxAngle);
	LASCANLIB_API int setDistanceFilterLimits(unsigned int iHandle, double dMinDist, double dMaxDist);
	LASCANLIB_API int getLaScanLibStatus(unsigned int iHandle);
	LASCANLIB_API int getSingleScan(unsigned int iHandle, std::list <SPunkt>& rlstclPoints, std::list <double>& rlstdTimeStamps);
	LASCANLIB_API int startScanRecording(unsigned int iHandle);
	LASCANLIB_API int stopScanRecording(unsigned int iHandle);
	LASCANLIB_API int getRecordedScanPoints(unsigned int iHandle, std::list <SPunkt>& rlstclPoints, std::list <double>& rlstdTimeStamps, bool bInterpoleateTimeStamps);
	LASCANLIB_API int getFilterRecordPoints(unsigned int iHandle, std::list <SPunkt>& rlstclPoints, std::list <double>& rlstdTimeStamps, unsigned int iAnzahl);
	LASCANLIB_API double getTimeStamp( );
}
