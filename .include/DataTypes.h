#ifndef __DATATYPES_H_INCLUDED__
#define __DATATYPES_H_INCLUDED__

#include <list>
#include <vector>
enum LaScanLibTelegramErrors
{
	LASCANLIB_OK						= 0LU,	// no error
	ERR_LASCANLIB_INVALID_TELEGRAM	= 1LU,	// syntacticaly  telegram
	ERR_LASCANLIB_TELEGRAM_CRC_ERROR	= 2LU,	// crc error ( crc - cyclic redundancy check = checksum )
	ERR_LASCANLIB_UNKOWN_ERROR		= 3LU,	// should never occur!!! 
	ERR_LASCANLIB_SCANNER_NULL		= 4LU,	// Scanner Objekt nicht vorhanden
	ERR_LASCANLIB_SCANNER_ID			= 5LU	// Handle ID nicht vorhanden
};

enum BenutzerLevel
{
	KEIN_BENUTZERLEVEL = 0,
	MASCHINENFUEHRER = 1,
	INSTANDHALTER = 2,
	AUTHORISIERTER_KUNDE = 3,
	SERVICE = 4
};

enum LaScanLibReceiveThreadStates
{
	RECEIVE_THREAD_NOT_RUNNING			= 0LU,	// initial value - thread not startet yet
	RECEIVE_THREAD_IDLE					= 1LU,	// the thread is collecting data from the scanner but does not store them
	RECEIVE_THREAD_ACTIVE				= 2LU,	// the thread is collecting data from the scanner and stores them
	RECEIVE_THREAD_TERMINATE_REQUEST	= 3LU,	// thread is requested to terminate
	RECEIVE_THREAD_TERMINATED			= 4LU,	// thread is running no more
	RECEIVE_THREAD_ERROR				= 5LU	// error ( e.g. socket or network problem )
};

//noch hinzugef�gt
enum VerbindungsErrors
{
	WSA_DATA_ERROR						= 0LU,
	MY_SOCKET_ERROR						= 1LU,
	MY_SOCKET_ERROR_CREATE				= 2LU,
	HOST_FAILED							= 3LU,
	LISTEN_FAILED						= 4LU,
	ACCEPT_FAILED						= 5LU,
	SERVER_FAILED_NOT_RECIEVED			= 6LU,
	RECIEVE_FAILED						= 7LU
};


// Definition Punkt Klasse 
struct SPunkt
{
	double x;
	double y;
	double z;	
	double dOrientierung;
	double dDistanz;
};

// CPolar Struktur
struct SPolar
{
	double r;
	double phi;
};

struct SPolar_Pep
{
	std::vector<double> dVecDistanz;
	double dWinkel;
	std::vector<double> dVecTime;
	int iAnz;
};

// Laserscanner Struktur
struct SLaserscan
{
	double dZeitstempel_Datenempfangen;			// Zeit wann die Daten in den Fifo geladen wurden
	std::list<SPunkt> lScannerDaten;				// Scannerdaten 
	std::list<double> lScannerZeiten;				// Scannerzeiten, diese Zeit ist f�r jede Scannerpunkt anderes
};

struct SEncoder
{
	double dZeitstempel;
	double dPosition;
};

struct SKalibrierung
{
	double dRotX;
	double dRotY;
	double dRotZ;
	double dTransX;
	double dTransY;
	double dTransZ;
};

#endif // __DATATYPES_H_INCLUDED__
