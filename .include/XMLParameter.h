#ifndef XMLPARAMETER_H
#define XMLPARAMETER_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdarg.h>
/*
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/valid.h>
#include <libxml/xmlversion.h>
#include <libxml/xmlreader.h>
*/
//#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
//#include <libxml/tree.h>

#include <Convert.h>


enum XMLInfos
{
	XML_OK = 0,				//XML war erfolgreich eim auslesen / parsen
	XML_PHRASE_ERROR = 1,	//XML parse Fehler
	XML_SEARCH_FAILED = 2	//XML Sche fehlgeschlagen SUchwort nicht gefunden
};

class CXMLParameter{
public:
	// Suchen eines Integers in einer XML Datei und deren Rückgabe
	static int GetWertAusXML_Int( const char* XMLPfad, int* ReturnParameter, int Parameter, ... );

	// Suchen eines Boolean in einer XML Datei und deren Rückgabe
	static int GetWertAusXML_Bool( const char* XMLPfad, bool* ReturnParameter, int Parameter, ... );

	// Suchen eines Strings in einer XML Datei und deren Rückgabe
	static int GetWertAusXML_Str( const char* XMLPfad, std::string* ReturnParameter, int Parameter, ... );

	// Suchen eines Unsigned in einer XML Datei und deren Rückgabe
	//unsigned GetWertAusXML_Unsi( const char* XMLPfad, int Parameter, ... );

	// Suchen eines Doubles in einer XML Datei und deren Rückgabe
	static int GetWertAusXML_Dou( const char* XMLPfad, double* ReturnParameter, int Parameter, ... );

	// Suchen eines Floats in einer XML Datei und deren Rückgabe
	static int GetWertAusXML_Float( const char* XMLPfad, float* ReturnParameter, int Parameter, ... );

	// Suchen eines Unsigned in einer XML Datei und deren Rückgabe
	static int GetWertAusXML_Unsi( const char* XMLPfad, unsigned* ReturnParameter, int Parameter, ... );

	// Funktion zu lesen der XML Datei
	static int XMLLesen(std::vector<std::string> vParameterListe, const char* XMLPfad, std::string* ReturnParameter );


};

#endif; //XMLPARAMETER_H