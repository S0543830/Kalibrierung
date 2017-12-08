#ifndef CONVERT_HEADER
#define CONVERT_HEADER

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>


using namespace std;

enum ConvertInfos
{
	CONVERT_CHAR_ARRAY_GROESSE = 50,
	CONVER_CHAR_AUFFUELL_ZEICHEN = 0
};

string DoubleToString( double d );
string IntToString( int i );
string UnsignedToString( unsigned ui );
string UnsignedShortToString( unsigned us );
string UnsignedLongToString( unsigned long ul );
string BoolToString( bool b );

string UnsignedShortHexToString( unsigned short us );
unsigned short HexInUnsignedShort( const char* c );

const char* UnsignedShortToCharArray( unsigned short us, unsigned int &lng);

char* ConstCharToChar( const char* cArray);

string CharToString( char c );

string StringVerkuerzen( string sEingabe, unsigned uiAsciilaenge );
string StringAuffuellen( string sEingabe, unsigned uiAsciilaenge );

unsigned int StringToChecksum( string sTransaktionsnummer, string sKommando, string sDatenlaenge, string sDaten );


void Convert_DoubleToChar( char DestChar[], double SrcDouble, unsigned uiZahlenGroesse );
void Convert_IntegerToChar( char DestChar[], int SrcInteger, unsigned uiZahlenGroesse );
void Convert_UnsignedShortToChar( char DestChar[], unsigned short SrcUnsignedShort, unsigned uiZahlenGroesse );
char* Convert_UnsignedintegerToChar( unsigned int ui, unsigned uiZahlenGroesse );

void My_strcpy(char* Dest, const char* Src);
void My_strncpy(char* Dest, const char* Src, size_t size);

void CharArrayAuffuellen(char Array[], unsigned uiZahlenGroesse);

vector<string> split( char* str,const char* delim );




#endif