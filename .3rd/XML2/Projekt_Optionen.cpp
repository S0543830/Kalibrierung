#include "Projekt_Optionen.h"

CProjekt_Optionen::CProjekt_Optionen(const char* PfadZuDenOptionen)
{
	this->PfadZuDenOptionen = PfadZuDenOptionen;
}

structOptionen CProjekt_Optionen::Optionen_laden()
{
	structOptionen so;

	//
	// Laserscanner Daten lesen
	//
	so.sloLaserscanneroptionen.bMitteln					= GetWertAusXML_Bool( this->PfadZuDenOptionen, 3,"ConMon", "Laserscanner", "bMitteln" );
	so.sloLaserscanneroptionen.bSimulieren				= GetWertAusXML_Bool( this->PfadZuDenOptionen, 3,"ConMon", "Laserscanner", "bSimulieren" );
	so.sloLaserscanneroptionen.cSimulationsPfad			= GetWertAusXML_Str ( this->PfadZuDenOptionen, 3,"ConMon", "Laserscanner", "cSimulationsPfad" );
	so.sloLaserscanneroptionen.Scanner_IP_Addresse		= GetWertAusXML_Str ( this->PfadZuDenOptionen, 3,"ConMon", "Laserscanner", "Scanner_IP_Adresse" );
	so.sloLaserscanneroptionen.uiBilderAufnehmen		= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Laserscanner", "uiBilderAufnehmen" );
	so.sloLaserscanneroptionen.uiPort					= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Laserscanner", "uiPort" );
	so.sloLaserscanneroptionen.uiScannerPunkte			= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Laserscanner", "uiScannerPunkte" );
	so.sloLaserscanneroptionen.uiWarteZeit				= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Laserscanner", "uiWarteZeit" );
	so.sloLaserscanneroptionen.uiStackgroesse			= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Laserscanner", "uiStackgroesse" );
	so.sloLaserscanneroptionen.uiDatengroesse			= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Laserscanner", "uiDatengroesse" );
	so.sloLaserscanneroptionen.bStackDatenSpeichern		= GetWertAusXML_Bool( this->PfadZuDenOptionen, 3,"ConMon", "Laserscanner", "bStackDatenSpeichern" );
	so.sloLaserscanneroptionen.bProtokoll				= GetWertAusXML_Bool( this->PfadZuDenOptionen, 3,"ConMon", "Laserscanner", "bProtokoll" );
	//
	// OpenGL Daten lesen
	//
	so.sooOpenGLoptionen.bAnzeigeStarten				= GetWertAusXML_Bool( this->PfadZuDenOptionen, 3,"ConMon", "OpenGL", "bAnzeigeStarten" );
	so.sooOpenGLoptionen.iBreite						= GetWertAusXML_Int ( this->PfadZuDenOptionen, 3,"ConMon", "OpenGL", "uiBreite" );
	so.sooOpenGLoptionen.iHoehe							= GetWertAusXML_Int ( this->PfadZuDenOptionen, 3,"ConMon", "OpenGL", "uiHoehe" );
	so.sooOpenGLoptionen.OpenGL_Version					= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "OpenGL", "OpenGL_Version" );
	//
	// Datentransfer Daten lesen
	//
	so.sdoDatentransferoptionen.Computer_IP_Addresse	= GetWertAusXML_Str ( this->PfadZuDenOptionen, 3,"ConMon", "Datentransfer", "Computer_IP" );
	so.sdoDatentransferoptionen.Datengröße				= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Datentransfer", "Datengroesse" );
	so.sdoDatentransferoptionen.uiPort					= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Datentransfer", "uiPort" );
	//
	// Ecken Daten lesen
	//
	so.seoEckenoptionen.dAbweichung						= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Ecken", "dAbweichung" );
	so.seoEckenoptionen.dFehlerWinkel					= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Ecken", "dFehlerWinkel" );
	so.seoEckenoptionen.dMax_Abstand_Der_Geraden		= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Ecken", "dMax_Abstand_der_Geraden" );
	so.seoEckenoptionen.dMin_Abstand_Der_Geraden		= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Ecken", "dMin_Abstand_der_Geraden" );
	so.seoEckenoptionen.dWinkelAbweichung				= GetWertAusXML_Dou(  this->PfadZuDenOptionen, 3,"ConMon", "Ecken", "dWinkelAbweichung" );
	//
	// Merkmals Daten lesen
	//
	so.smoMerkmaloptionen.dFangRadiusRotation			= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Merkmale", "dFangRadiusRotation" );
	so.smoMerkmaloptionen.dFangRadiusTranslation		= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Merkmale", "dFangRadiusTranslation" );
	so.smoMerkmaloptionen.dMerkmalsAbweichung			= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Merkmale", "dMerkmalsAbweichung" );
  so.smoMerkmaloptionen.dMerkmalsAbweichungOrientierung = GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Merkmale", "dMerkmalsAbweichungOrientierung" );
	so.smoMerkmaloptionen.uiMerkmalsZaehler				= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Merkmale", "uiMerkmalsZaehler" );
	so.smoMerkmaloptionen.uiMinEigenschaftenGefunden	= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Merkmale", "uiMinEigenschaftenGefunden" );
	//
	// Geraden Daten lesen
	//
  so.sgoGeradenoptionen.dGeraden_Zusammensetzen_Abstand = GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Geraden", "dGeraden_Zusammensetzen" );
	so.sgoGeradenoptionen.dMax_Abstand					= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Geraden", "dMax_Abstand" );
	so.sgoGeradenoptionen.dMax_Abstand_Gerade			= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Geraden", "dMax_Abstand_Gerade" );
	so.sgoGeradenoptionen.dMinLaenge					= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Geraden", "dMin_Laenge" );
	so.sgoGeradenoptionen.dMultiplikator				= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Geraden", "dMultiplikator" );
	so.sgoGeradenoptionen.uiMaxPunkte					= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Geraden", "uiMaxPunkte" );
	so.sgoGeradenoptionen.uiMinPunkte					= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Geraden", "uiMinPunkte" );
	//
	// Laserscanner Server daten lesen
	//
	so.slsoLaserscannerServeroptionen.bKommunikationStarten		= GetWertAusXML_Bool( this->PfadZuDenOptionen, 3,"ConMon", "LaserscannerServer", "bKommunikationStarten" );
	so.slsoLaserscannerServeroptionen.bKommuniktaionsfehler		= GetWertAusXML_Bool( this->PfadZuDenOptionen, 3,"ConMon", "LaserscannerServer", "bKommuniktaionsfehler" );
	so.slsoLaserscannerServeroptionen.Kommunikations_IP_Adresse	= GetWertAusXML_Str ( this->PfadZuDenOptionen, 3,"ConMon", "LaserscannerServer", "Kommunikations_IP_Adresse" );
	so.slsoLaserscannerServeroptionen.uiKommunikationsport		= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "LaserscannerServer", "uiKommunikationsport" );
	so.slsoLaserscannerServeroptionen.cSpeicherpfad				= GetWertAusXML_Str ( this->PfadZuDenOptionen, 3,"ConMon", "LaserscannerServer", "cSpeicherpfad" );
	//
	// Kartenerstellungs Daten lesen
	//
	so.skeoKartenerstellungsOptionenen.dPlattform_Radius		= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "dPlattform_Radius" );
	so.skeoKartenerstellungsOptionenen.dPlattform_Achse			= GetWertAusXML_Dou	( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "dPlattform_Achse" );
	so.skeoKartenerstellungsOptionenen.dPlattform_Increment		= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "dPlattform_Increment" );
	so.skeoKartenerstellungsOptionenen.dPlattform_Ratio			= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "dPlattform_Ratio" );
	so.skeoKartenerstellungsOptionenen.dLaser_Offset			= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "dLaser_Offset" );
	so.skeoKartenerstellungsOptionenen.iLaser_MaxPunkte			= GetWertAusXML_Int ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "iLaser_MaxPunkte" );
	so.skeoKartenerstellungsOptionenen.iLaser_MinAngle			= GetWertAusXML_Int ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "iLaser_MinAngle" );
	so.skeoKartenerstellungsOptionenen.iLaser_MaxAngle			= GetWertAusXML_Int ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "iLaser_MaxAngle" );
	so.skeoKartenerstellungsOptionenen.dLaser_Margin			= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "dLaser_Margin" );
	so.skeoKartenerstellungsOptionenen.dLaser_No_Distance		= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "dLaser_No_Distance" );
	so.skeoKartenerstellungsOptionenen.iKarte_Zoom				= GetWertAusXML_Int ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "iKarte_Zoom" );
	so.skeoKartenerstellungsOptionenen.bKarte_Debug				= GetWertAusXML_Bool( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "bKarte_Debug" );
	so.skeoKartenerstellungsOptionenen.iKarte_Display_Groesse	= GetWertAusXML_Int ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "iKarte_Display_Groesse" );
	so.skeoKartenerstellungsOptionenen.uiHoleWidth				= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "uiHoleWidth" );
	so.skeoKartenerstellungsOptionenen.dKartenRaster			= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "dKartenRaster" );
	so.skeoKartenerstellungsOptionenen.uiMonteCarloSuche		= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "uiMonteCarloSuche" );
	so.skeoKartenerstellungsOptionenen.sSpeicherOrt				= GetWertAusXML_Str ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "sSpeicherOrt" );
	so.skeoKartenerstellungsOptionenen.bSimulieren				= GetWertAusXML_Bool( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "bSimulieren" );
	so.skeoKartenerstellungsOptionenen.sSimulationsdatei		= GetWertAusXML_Str ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "sSimulationsdatei" );
	so.skeoKartenerstellungsOptionenen.bKarteSpiegeln			= GetWertAusXML_Bool( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "bKarteSpiegeln" );
	so.skeoKartenerstellungsOptionenen.uiMax_Laser_Distanz		= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "uiMax_Laser_Distanz" );
	so.skeoKartenerstellungsOptionenen.uiKarte_Groesse			= GetWertAusXML_Unsi( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "uiKarte_Groesse" );
	so.skeoKartenerstellungsOptionenen.dSigmaTheta				= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "dSigmaTheta" );
	so.skeoKartenerstellungsOptionenen.dSigmaXY					= GetWertAusXML_Dou ( this->PfadZuDenOptionen, 3,"ConMon", "Kartenerstellung", "dSigmaXY" );
	//
	// Mobile Plattform Daten lesen
	//
	so.smpMobile_PlattformOptionen.sIp							= GetWertAusXML_Str ( this->PfadZuDenOptionen, 3,"ConMon", "MobilePlattform", "sIP" );
	so.smpMobile_PlattformOptionen.sPort						= GetWertAusXML_Str ( this->PfadZuDenOptionen, 3,"ConMon", "MobilePlattform", "sPort" );
	//
	// Schwenkneigeeinheit Daten lesen
	//
	so.ssneSchwenkNeigeEinheitOptionen.sIp						= GetWertAusXML_Str ( this->PfadZuDenOptionen, 3,"ConMon", "SchwenkNeigeEinheit", "sIP" );
	so.ssneSchwenkNeigeEinheitOptionen.sPort					= GetWertAusXML_Str ( this->PfadZuDenOptionen, 3,"ConMon", "SchwenkNeigeEinheit", "sPort" );
	//
	// Debug Flag lesen
	//
	so.bDebug = GetWertAusXML_Bool( this->PfadZuDenOptionen, 2,"ConMon", "bDebug" );

	//
	// Optionen zurückgeben
	//
	return so;
}

void CProjekt_Optionen::Optionen_speichern(structOptionen spOptionen)
{


}

bool CProjekt_Optionen::GetWertAusXML_Bool( const char* XMLPfad, int Parameter, ... )
{
	va_list ap;
	vector<string> vParameterListe;
	string sTmp;
	bool bUmwandlung = false;
	int iUmwandlung;

	//Parameter einlesen
	if(Parameter)
	{		
		va_start(ap, Parameter);
			while(Parameter--)
			{
				sTmp = va_arg(ap, char*);
				vParameterListe.push_back(sTmp);
			}
		va_end(ap);
	}
	//Parameter in XML-datei suchen
	sTmp = XMLLesen(vParameterListe, XMLPfad);

	//Typen Umwandeln
	if(sTmp.compare("XML_SEARCH_FAILED") == 0)
	{
		cout << "Bei Optionsparameter: " << vParameterListe.at(vParameterListe.size()-1) << ", ist ein Fehler aufgetreten" << endl;
		sTmp = "";
	}

	if(sTmp.compare(" true ") == 0 || sTmp.compare(" True ") == 0 || sTmp.compare("True") == 0 || sTmp.compare("true") == 0)
	{
		bUmwandlung = true;
	}
	else if(sTmp.compare(" false ") == 0 || sTmp.compare(" False ") == 0 || sTmp.compare("False") == 0 || sTmp.compare("false") == 0)
	{	
		bUmwandlung = false;
	}
	else
	{
		iUmwandlung = atoi(sTmp.c_str());
		if(0 != iUmwandlung && 1 != iUmwandlung)
		{
			bUmwandlung = false;
		}
		else if(0 == iUmwandlung)
		{
			bUmwandlung = false;
		}
		else
		{
			iUmwandlung = true;
		}
	}

	return bUmwandlung;

}
string CProjekt_Optionen::GetWertAusXML_Str( const char* XMLPfad, int Parameter, ... )
{
	va_list ap;
	vector<string> vParameterListe;
	string sTmp;

	//Parameter einlesen
	if(Parameter)
	{		
		va_start(ap, Parameter);
			while(Parameter--)
			{
				sTmp = va_arg(ap, char*);
				vParameterListe.push_back(sTmp);
			}
		va_end(ap);
	}
	//Parameter in XML-datei suchen
	sTmp = XMLLesen(vParameterListe, XMLPfad);
	if(sTmp.compare("XML_SEARCH_FAILED") == 0)
	{
		cout << "Bei Optionsparameter: " << vParameterListe.at(vParameterListe.size()-1) << ", ist ein Fehler aufgetreten" << endl;
		sTmp = "";
	}
	if(sTmp[0] == ' ' || sTmp[sTmp.length()-2] == ' ')
	{
		sTmp = sTmp.substr(1, sTmp.length()-2);
	}

	return sTmp;
}

double CProjekt_Optionen::GetWertAusXML_Dou( const char* XMLPfad, int Parameter, ... )
{
	va_list ap;
	vector<string> vParameterListe;
	string sTmp;
	double dUmwandlung;

	//Parameter einlesen
	if(Parameter)
	{		
		va_start(ap, Parameter);
			while(Parameter--)
			{
				sTmp = va_arg(ap, char*);
				vParameterListe.push_back(sTmp);
			}
		va_end(ap);
	}
	//Parameter in XML-datei suchen
	sTmp = XMLLesen(vParameterListe, XMLPfad);

	//Typen Umwandeln
	if(sTmp.compare("XML_SEARCH_FAILED") == 0)
	{
		cout << "Bei Optionsparameter: " << vParameterListe.at(vParameterListe.size()-1) << ", ist ein Fehler aufgetreten" << endl;
		sTmp = "";
	}
	dUmwandlung = atof(sTmp.c_str());

	return dUmwandlung;
}

unsigned CProjekt_Optionen::GetWertAusXML_Unsi( const char* XMLPfad, int Parameter, ... )
{	
	va_list ap;
	vector<string> vParameterListe;
	string sTmp;
	unsigned uiUmwandlung = 0;

	//Parameter einlesen
	if(Parameter)
	{		
		va_start(ap, Parameter);
			while(Parameter--)
			{
				sTmp = va_arg(ap, char*);
				vParameterListe.push_back(sTmp);
			}
		va_end(ap);
	}
	//Parameter in XML-datei suchen
	sTmp = XMLLesen(vParameterListe, XMLPfad);

	//Typen Umwandeln
	if(sTmp.compare("XML_SEARCH_FAILED") == 0)
	{
		cout << "Bei Optionsparameter: " << vParameterListe.at(vParameterListe.size()-1) << ", ist ein Fehler aufgetreten" << endl;
		sTmp = "";
	}
	uiUmwandlung = (unsigned) atol(sTmp.c_str());

	return uiUmwandlung;
}

int CProjekt_Optionen::GetWertAusXML_Int( const char* XMLPfad, int Parameter, ... )
{
	va_list ap;
	vector<string> vParameterListe;
	string sTmp;
	int iUmwandlung = 0;

	//Parameter einlesen
	if(Parameter)
	{		
		va_start(ap, Parameter);
			while(Parameter--)
			{
				sTmp = va_arg(ap, char*);
				vParameterListe.push_back(sTmp);
			}
		va_end(ap);
	}
	//Parameter in XML-datei suchen
	sTmp = XMLLesen(vParameterListe, XMLPfad);

	//Typen Umwandeln
	if(sTmp.compare("XML_SEARCH_FAILED") == 0)
	{
		cout << "Bei Optionsparameter: " << vParameterListe.at(vParameterListe.size()-1) << ", ist ein Fehler aufgetreten" << endl;
		sTmp = "";
	}
	iUmwandlung = atoi(sTmp.c_str());

	return iUmwandlung;
}

string CProjekt_Optionen::XMLLesen(vector<string> vParameterListe, const char* XMLPfad)
{
    xmlDoc* doc = NULL;
    xmlNode* root_element = NULL;
	xmlNode* cur_node = NULL;

	unsigned tt;
	int iRet;
	string sRueckgabeString = "";

	iRet = XML_OK;
	tt = 0;

    doc = xmlReadFile(XMLPfad, NULL, 0);
    if (doc == NULL) 
	{
		iRet = XML_PHRASE_ERROR;
    }
	else
	{
		root_element = xmlDocGetRootElement(doc);
		cur_node = root_element;

		while(true)
		{
			if(cur_node == NULL)
			{
				sRueckgabeString = "XML_SEARCH_FAILED";
				break;
			}
			else
			{
				if(cur_node->type == XML_ELEMENT_NODE) 
				{	
					if(0 == xmlStrcmp(cur_node->name, (const xmlChar *) vParameterListe.at(tt).c_str()))
					{	
						cur_node = cur_node->children;
						tt++;
					}

					if(cur_node->type == XML_TEXT_NODE && vParameterListe.size() == tt)
					{
						sRueckgabeString.append((char*)cur_node->content);
						break;
					}
				}
			}
			cur_node = cur_node->next;
		}		
		xmlFreeDoc(doc);
	}

	return sRueckgabeString;
}
