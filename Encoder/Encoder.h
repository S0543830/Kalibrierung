// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren 
// aus einer DLL vereinfachen. Alle Dateien in dieser DLL werden mit dem ENCODER_EXPORTS-Symbol
// (in der Befehlszeile definiert) kompiliert. Dieses Symbol darf f�r kein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen 
// ENCODER_API-Funktionen als aus einer DLL importiert, w�hrend die DLL
// mit diesem Makro definierte Symbole als exportiert ansieht.
#ifdef ENCODER_EXPORTS
#define ENCODER_API __declspec(dllexport)
#else
#define ENCODER_API __declspec(dllimport)
#endif


struct InitParameter
{
	double dMaxPos;
	double dMinPos;
	double dVel;
	double dAcc;
};


extern "C" {
	ENCODER_API bool init( InitParameter const * pInitParam );
	ENCODER_API void close( void );
	ENCODER_API bool startMovement( void );
	ENCODER_API bool stopMovement( void );
	ENCODER_API bool setVel( double dVel );
	ENCODER_API bool setAcc( double dAcc );
	ENCODER_API bool getPos( double * pdPos );
}
