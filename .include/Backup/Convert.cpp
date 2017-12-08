#include "Convert.h"

using namespace std;
#pragma warning ( disable : 4996 )

string DoubleToString( double d )
{
	// Objekt auf die Klasse stringstream erstellen.
	stringstream ss; 
	ss << d;
	return ( ss.str() );
}

string IntToString( int i )
{
	// Objekt auf die Klasse stringstream erstellen.
	stringstream ss; 
	ss << i;
	return ( ss.str() );
}

char* ConstCharToChar( const char* cArray)
{
	char* ccArray;
	ccArray = const_cast<char*>(cArray);
	return ccArray;
}

string UnsignedToString( unsigned ui )
{
	// Objekt auf die Klasse stringstream erstellen.
	stringstream ss; 
	ss << ui;
	return ( ss.str() );
}

string UnsignedLongToString( unsigned long ul )
{
	// Objekt auf die Klasse stringstream erstellen.
	stringstream ss; 
	ss << ul;
	return ( ss.str() );
}

string BoolToString( bool b )
{
	// Objekt auf die Klasse stringstream erstellen.
	stringstream ss; 
	ss << b;
	return ( ss.str() );
}

string CharToString( char c )
{
	stringstream ss; 
	ss << c;
	return ( ss.str() );
}

string StringVerkuerzen( string sEingabe, unsigned uiAsciilaenge )
{
	string sAusgabe;
	sAusgabe = "";

	if( sEingabe.size() > uiAsciilaenge)
	{
		sEingabe.resize( uiAsciilaenge );
		sAusgabe = sEingabe;
	}
	else
	{
		sAusgabe = sEingabe;
	}

	return sAusgabe;

}

string StringAuffuellen( string sEingabe, unsigned uiAsciilaenge )
{
	string sAusgabe;
	sAusgabe = "";

	unsigned uiAuffuellen;	
	unsigned uiStringGroesse;
	int p;

	if( sEingabe.size() < uiAsciilaenge)
	{
		uiAuffuellen = uiAsciilaenge - sEingabe.size();
		uiStringGroesse = sEingabe.size();

		sAusgabe.resize( uiAsciilaenge );

		p=uiStringGroesse-1;
		for( unsigned t = sAusgabe.size(); t > 0; t--)
		{
			if( 0 <= p ) 
			{
				sAusgabe[t-1] = sEingabe[p];
				p--;
			}
			else
			{
				sAusgabe[t-1] = '0';
			}			
		}
	}
	else
	{
		sAusgabe = sEingabe;
	}

	return sAusgabe;
}


unsigned int StringToChecksum( string sTransaktionsnummer, string sKommando, string sDatenlaenge, string sDaten )
{
	unsigned int crc = 0;
	unsigned u;

	for ( u = 0; u < sTransaktionsnummer.size(); u++ )
	{
		crc ^= sTransaktionsnummer[ u ];
	}

	for ( u = 0; u < sKommando.size(); u++ )
	{
		crc ^= sKommando[ u ];
	}

	for ( u = 0; u < sDatenlaenge.size(); u++ )
	{
		crc ^= sDatenlaenge[ u ];
	}

	for ( u = 0; u < sDaten.size(); u++ )
	{
		crc ^= sDaten[ u ];
	}
	
	return crc;	
}

const char* UnsignedShortToCharArray( unsigned short us, unsigned int &lng)
{
	stringstream ss; 
	ss << us;

	const char* cs = ss.str().c_str();
	lng = ss.str().length();

	return cs;
}


string UnsignedShortHexToString( unsigned short us )
{
	stringstream ss;
	ss << std::hex << us;

	return ss.str();
}

string UnsignedShortToString( unsigned us )
{
	// Objekt auf die Klasse stringstream erstellen.
	stringstream ss; 
	ss << us;
	return ( ss.str() );
}

// DestChar muss immer eins größer sein als sein ein Ihanlt sonst kommt es zu Heapfehlern
void Convert_DoubleToChar( char DestChar[], double SrcDouble, unsigned uiZahlenGroesse )
{
	_snprintf( DestChar, uiZahlenGroesse, "%lf", SrcDouble );
	DestChar[uiZahlenGroesse] = '\0';
}

// DestChar muss immer eins größer sein als sein ein Ihanlt sonst kommt es zu Heapfehlern
void Convert_UnsignedShortToChar( char DestChar[], unsigned short SrcUnsignedShort, unsigned uiZahlenGroesse )
{
	_snprintf(DestChar, uiZahlenGroesse, "%hX", SrcUnsignedShort);
}

// DestChar muss immer eins größer sein als sein ein Ihanlt sonst kommt es zu Heapfehlern
void Convert_IntegerToChar( char DestChar[], int SrcInteger, unsigned uiZahlenGroesse )
{
	_snprintf(DestChar, uiZahlenGroesse, "%d", SrcInteger);
	DestChar[uiZahlenGroesse] = '\0';
}

void CharArrayAuffuellen(char Array[], unsigned uiZahlenGroesse)
{
	unsigned i;
	char cTmp;
	for(i=0; i < uiZahlenGroesse-1; i++ )
	{
		if( '+' == Array[i]  || '-' == Array[i] )
		{
			continue;
		}
		else
		{
			cTmp = Array[i];
			Array[i] = '0';
			Array[i+1] = cTmp;
		}
	}
}

/*
char* Convert_UnsignedShortToChar( unsigned short us, unsigned uiZahlenGroesse )
{

}

char* Convert_UnsignedintegerToChar( unsigned integer ui, unsigned uiZahlenGroesse )
{

}
*/

void My_strncpy(char* Dest, const char* Src, size_t size)
{
	unsigned tt;
	for( tt = 0; tt<size; tt++)
	{
		if( '\0' == Src[tt])
		{
			break;
		}
		Dest[tt] = Src[tt];
	}
}

void My_strcpy(char* Dest, const char* Src)
{
	unsigned len = strlen(Src);
	unsigned tt;

	for(tt=0; tt<len; tt++)
	{
		Dest[tt] = Src[tt];
	}
}

unsigned short HexInUnsignedShort( const char* c )
{
	unsigned short us;
	stringstream ss;
	
	ss << std::hex << c;
	ss >> us;

	return us;

}

vector<string> split( char* str,const char* delim )
{
	char* token = strtok(str,delim);

	vector<string> result;

	while(token != NULL)
	{
		result.push_back(token);
		token = strtok(NULL,delim);
	}
	return result;
}

