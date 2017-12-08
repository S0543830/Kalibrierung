#include "XMLParameter.h"

/************************************************************************/
/* Liest den XML Wert vom typ bool aus dem File und schreibt ihn in		*/
/* den Parameter ReturnParameter der Return wert gibt an ob das auslesen*/
/* Erfolgreich war oder nicht											*/
/************************************************************************/
int CXMLParameter::GetWertAusXML_Bool( const char* XMLPfad, bool* ReturnParameter, int Parameter, ... )
{
	va_list ap;
	vector<string> vParameterListe;
	string sTmp;
	int iUmwandlung;
	int iResult = 0;

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
	sTmp.clear();
	iResult = XMLLesen(vParameterListe, XMLPfad, &sTmp);

	//Typen Umwandeln
	if(sTmp.compare("XML_SEARCH_FAILED") == 0)
	{
		cout << "Bei Optionsparameter: " << vParameterListe.at(vParameterListe.size()-1) << ", ist ein Fehler aufgetreten" << endl;
		sTmp = "";
	}

	if(sTmp.compare(" true ") == 0 || sTmp.compare(" True ") == 0 || sTmp.compare("True") == 0 || sTmp.compare("true") == 0)
	{
		*ReturnParameter = true;
	}
	else if(sTmp.compare(" false ") == 0 || sTmp.compare(" False ") == 0 || sTmp.compare("False") == 0 || sTmp.compare("false") == 0)
	{	
		*ReturnParameter = false;
	}
	else
	{
		iUmwandlung = atoi(sTmp.c_str());
		if(0 != iUmwandlung && 1 != iUmwandlung)
		{
			*ReturnParameter = false;
		}
		else if(0 == iUmwandlung)
		{
			*ReturnParameter = false;
		}
		else
		{
			*ReturnParameter = true;
		}
	}

	return iResult;

}

/************************************************************************/
/* Liest den XML Wert vom typ string aus dem File und schreibt ihn in		*/
/* den Parameter ReturnParameter der Return wert gibt an ob das auslesen*/
/* Erfolgreich war oder nicht											*/
/************************************************************************/
int CXMLParameter::GetWertAusXML_Str( const char* XMLPfad, string* ReturnParameter, int Parameter, ... )
{
	va_list ap;
	vector<string> vParameterListe;
	string sTmp;
	int iResult = 0;

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
	sTmp.clear();
	iResult = XMLLesen(vParameterListe, XMLPfad, &sTmp );

	if(0 == iResult )
	{
		ReturnParameter->append(sTmp);
	}
	
	return iResult;
}

/************************************************************************/
/* Liest den XML Wert vom typ double aus dem File und schreibt ihn in		*/
/* den Parameter ReturnParameter der Return wert gibt an ob das auslesen*/
/* Erfolgreich war oder nicht											*/
/************************************************************************/
int CXMLParameter::GetWertAusXML_Dou( const char* XMLPfad, double* ReturnParameter, int Parameter, ... )
{
	va_list ap;
	vector<string> vParameterListe;
	string sTmp;
	int iResult = 0;

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
	sTmp.clear();
	iResult = XMLLesen(vParameterListe, XMLPfad, &sTmp);

	//Typen Umwandeln
	if(0 == iResult )
	{
		*ReturnParameter = atof(sTmp.c_str());
	}
	

	return iResult;
}

/************************************************************************/
/* Liest den XML Wert vom typ double aus dem File und schreibt ihn in		*/
/* den Parameter ReturnParameter der Return wert gibt an ob das auslesen*/
/* Erfolgreich war oder nicht											*/
/************************************************************************/
int CXMLParameter::GetWertAusXML_Float( const char* XMLPfad, float* ReturnParameter, int Parameter, ... )
{
	va_list ap;
	vector<string> vParameterListe;
	stringstream sStream (stringstream::in | stringstream::out);
	string sTmp;
	int iResult = 0;

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
	sTmp.clear();
	iResult = XMLLesen(vParameterListe, XMLPfad, &sTmp);

	//Typen Umwandeln
	if(0 == iResult )
	{
		sStream << sTmp;
		sStream >> *ReturnParameter;
	}


	return iResult;
}

/************************************************************************/
/* Liest den XML Wert vom typ int aus dem File und schreibt ihn in		*/
/* den Parameter ReturnParameter der Return wert gibt an ob das auslesen*/
/* Erfolgreich war oder nicht											*/
/************************************************************************/
int CXMLParameter::GetWertAusXML_Int( const char* XMLPfad, int* ReturnParameter, int Parameter, ... )
{
	va_list ap;
	vector<string> vParameterListe;
	string sTmp;
	int iResult = 0;

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
	sTmp.clear();
	//Parameter in XML-datei suchen
	iResult = XMLLesen(vParameterListe, XMLPfad, &sTmp);

	//Typen Umwandeln
	if(sTmp.compare("XML_SEARCH_FAILED") == 0)
	{
		cout << "Bei Optionsparameter: " << vParameterListe.at(vParameterListe.size()-1) << ", ist ein Fehler aufgetreten" << endl;
		sTmp = "";
	}
	*ReturnParameter = atoi(sTmp.c_str());

	return iResult;
}

/************************************************************************/
/* Liest den XML Wert vom typ unsigned int aus dem File und schreibt	*/
/* ihn in den Parameter ReturnParameter der Return wert gibt an ob das  */
/* auslesen Erfolgreich war oder nicht								    */
/************************************************************************/
int CXMLParameter::GetWertAusXML_Unsi( const char* XMLPfad, unsigned* ReturnParameter, int Parameter, ... )
{
	va_list ap;
	vector<string> vParameterListe;
	string sTmp;
	int iResult = 0;

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
	sTmp.clear();
	//Parameter in XML-datei suchen
	iResult = XMLLesen(vParameterListe, XMLPfad, &sTmp);

	//Typen Umwandeln
	if(sTmp.compare("XML_SEARCH_FAILED") == 0)
	{
		cout << "Bei Optionsparameter: " << vParameterListe.at(vParameterListe.size()-1) << ", ist ein Fehler aufgetreten" << endl;
		sTmp = "";
	}
	*ReturnParameter = (unsigned) atol(sTmp.c_str());

	return iResult;
}

/************************************************************************/
/************************************************************************/
int CXMLParameter::XMLLesen(vector<string> vParameterListe, const char* XMLPfad, string* ReturnParameter )
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
				iRet = XML_PHRASE_ERROR;
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
						ReturnParameter->append((char*)cur_node->content);
						break;
					}
				}
			}
			cur_node = cur_node->next;
		}		
		xmlFreeDoc(doc);
	}
	return iRet;
}
