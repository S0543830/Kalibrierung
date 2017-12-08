// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren 
// aus einer DLL vereinfachen. Alle Dateien in dieser DLL werden mit dem ENCODER_EXPORTS-Symbol
// (in der Befehlszeile definiert) kompiliert. Dieses Symbol darf für kein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen 
// ENCODER_API-Funktionen als aus einer DLL importiert, während die DLL
// mit diesem Makro definierte Symbole als exportiert ansieht.
#ifdef ENCODER_AMTEC_EXPORTS
#define ENCODER_AMTEC_API __declspec(dllexport)
#else
#define ENCODER_AMTEC_API __declspec(dllimport)
#endif


struct InitParameter
{
	double dMaxPos;
	double dMinPos;
	double dVel;
	double dAcc;
};


extern "C" {
	ENCODER_AMTEC_API bool init( InitParameter const * pInitParam );
	ENCODER_AMTEC_API void close( void );
	ENCODER_AMTEC_API bool startMovement( void );
	ENCODER_AMTEC_API bool stopMovement( void );
	ENCODER_AMTEC_API bool setVel( double dVel );
	ENCODER_AMTEC_API bool setAcc( double dAcc );
	ENCODER_AMTEC_API bool getPos( double * pdPos );
}
