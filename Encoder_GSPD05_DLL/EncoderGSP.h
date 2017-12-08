// Folgender ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren 
// aus einer DLL vereinfachen. Alle Dateien in dieser DLL werden mit dem ENCODER_EXPORTS-Symbol
// (in der Befehlszeile definiert) kompiliert. Dieses Symbol darf für kein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, erkennen 
// ENCODER_API-Funktionen als aus einer DLL importiert, während die DLL
// mit diesem Makro definierte Symbole als exportiert ansieht.
#ifdef ENCODER_GSP_EXPORTS
#define ENCODER_GSP_API __declspec(dllexport)
#else
#define ENCODER_GSP_API __declspec(dllimport)
#endif


extern "C" 
{
	ENCODER_GSP_API bool init(char* cComPort, int iBaud, char cParity, int iData, int iStop);
	ENCODER_GSP_API void close(void);
	ENCODER_GSP_API bool startMovement(void);
	ENCODER_GSP_API bool stopMovement(void);
	ENCODER_GSP_API bool moveToPos(double pos);
	ENCODER_GSP_API bool setVel(double dVel);
	ENCODER_GSP_API bool setAcc(double dAcc);
	ENCODER_GSP_API bool getPos(double * pdPos);
}
