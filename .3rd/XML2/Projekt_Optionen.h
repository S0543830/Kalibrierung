// Diese Headerdatei stellt Funktionen zum Auslesen von Xml Datei bereit

#ifndef PROJEKT_OPTIONEN_H
#define PROJEKT_OPTIONEN_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdarg.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/valid.h>
#include <libxml/xmlversion.h>
#include <libxml/xmlreader.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <Convert.h>


using namespace std;

// Struktur f�r die Geraden Optionen
typedef struct
{
	double uiMinPunkte;
	double uiMaxPunkte;
	double dMax_Abstand;
	double dMax_Abstand_Gerade;
	double dMultiplikator;
	double dGeraden_Zusammensetzen_Abstand;
	double dMinLaenge;
}struct_Geraden_Optionen;

// Struktur f�r die Ecken Optionen
typedef struct
{
	double dMin_Abstand_Der_Geraden;
	double dMax_Abstand_Der_Geraden;
	double dFehlerWinkel;
	double dAbweichung;
	double dWinkelAbweichung;
}struct_Ecken_Optionen;

// Struktur f�r die Merkmals Optionen
typedef struct
{
	double dMerkmalsAbweichung;
	double dMerkmalsAbweichungOrientierung;
	double dFangRadiusTranslation;
	double dFangRadiusRotation;
	unsigned uiMerkmalsZaehler;
	unsigned uiMinEigenschaftenGefunden;
}struct_Merkmal_Optionen;

// Struktur f�r die OpenGL Optionen
typedef struct
{
	bool bAnzeigeStarten;
	int iBreite;
	int iHoehe;
	unsigned OpenGL_Version;
}struct_OpenGL_Optionen;

// Struktur f�r die Laserscanner Optionen
typedef struct
{
	bool bSimulieren;
	bool bMitteln;
	bool bStackDatenSpeichern;
	bool bProtokoll;

	unsigned uiStackgroesse;
	unsigned uiDatengroesse;
	unsigned uiBilderAufnehmen;
	unsigned uiWarteZeit;
	unsigned uiScannerPunkte;
	unsigned uiPort;

	string cSimulationsPfad;	
	string Scanner_IP_Addresse;

}struct_Laserscanner_Optionen;


typedef struct
{
    bool bKommunikationStarten;
	bool bKommuniktaionsfehler;
	unsigned uiKommunikationsport;
    string Kommunikations_IP_Adresse;
	string cSpeicherpfad;

}struct_LaserscannerServer_Optionen;

// Struktur f�r die Datentransfer Optionen
typedef struct
{
	string Computer_IP_Addresse;
	unsigned uiPort;
	unsigned Datengr��e;

}struct_Datentransfer_Optionen;

typedef struct
{
	string sIp;
	string sPort;

}struct_Mobile_Plattform;

typedef struct
{
	string sIp;
	string sPort;

}struct_SchwenkNeigeEinheit;

typedef struct
{
	double dPlattform_Radius;
	double dPlattform_Achse;
	double dPlattform_Increment;
	double dPlattform_Ratio;

	double dLaser_Offset;
	int uiLaser_MaxPunkte;
	int iLaser_MinAngle;
	int iLaser_MaxAngle;
	double dLaser_Margin;
	double dLaser_No_Distance;

	int iKarte_Zoom;
	bool bKarte_Debug;
	int uiKarte_Display_Groesse;
	bool bKarteSpiegeln;

	unsigned uiHoleWidth;
	double dKartenRaster;
	unsigned uiMonteCarloSuche;
	double dSigmaTheta;
	double dSigmaXY;

	unsigned uiMax_Laser_Distanz;
	unsigned uiKarte_Groesse;

	string sSpeicherOrt;
	bool bSimulieren;
	string sSimulationsdatei;
	

}struct_Kartenerstellung_Optionen;

// Struktur f�r die oben genannten Optionen
typedef struct
{
	struct_Merkmal_Optionen				smoMerkmaloptionen;
	struct_Ecken_Optionen				seoEckenoptionen;
	struct_Geraden_Optionen				sgoGeradenoptionen;
	struct_OpenGL_Optionen				sooOpenGLoptionen;
	struct_Laserscanner_Optionen		sloLaserscanneroptionen;
	struct_Datentransfer_Optionen		sdoDatentransferoptionen;
	struct_LaserscannerServer_Optionen	slsoLaserscannerServeroptionen;
	struct_Kartenerstellung_Optionen	skeoKartenerstellungsOptionenen;
	struct_Mobile_Plattform				smpMobile_PlattformOptionen;
	struct_SchwenkNeigeEinheit			ssneSchwenkNeigeEinheitOptionen;
	bool bDebug;
}structOptionen;


enum XMLInfos
{
	XML_OK = 0,				//XML war erfolgreich eim auslesen / parsen
	XML_PHRASE_ERROR = 1,	//XML parse Fehler
	XML_SEARCH_FAILED = 2	//XML Sche fehlgeschlagen SUchwort nicht gefunden
};

class CProjekt_Optionen
{
private:

	// Suchen eines Integers in einer XML Datei und deren R�ckgabe
	int GetWertAusXML_Int( const char* XMLPfad, int Parameter, ... );

	// Suchen eines Boolean in einer XML Datei und deren R�ckgabe
	bool GetWertAusXML_Bool( const char* XMLPfad, int Parameter, ... );

	// Suchen eines Strings in einer XML Datei und deren R�ckgabe
	string GetWertAusXML_Str( const char* XMLPfad, int Parameter, ... );

	// Suchen eines Unsigned in einer XML Datei und deren R�ckgabe
	unsigned GetWertAusXML_Unsi( const char* XMLPfad, int Parameter, ... );

	// Suchen eines Doubles in einer XML Datei und deren R�ckgabe
	double GetWertAusXML_Dou( const char* XMLPfad, int Parameter, ... );

	// Funktion zu lesen der XML Datei
	string XMLLesen(vector<string> vParameterListe, const char* XMLPfad);


	// XML Pfad
	const char* PfadZuDenOptionen;

public: 
	// Konstruktor der Klasse, dieser ben�tigt einen Pfad zu der XML Datei
	CProjekt_Optionen(const char* PfadZuDenOptionen);

	// Funktion zu laden der Optionen, lesen der XML Datei
	structOptionen Optionen_laden();

	// Funktion zum Speichern der OPtionen in eine XML Datei
	void Optionen_speichern(structOptionen spOptionen);
};


#endif; //PROJEKT_OPTIONEN_H