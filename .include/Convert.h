#ifndef CONVERT_HEADER
#define CONVERT_HEADER

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>



enum ConvertInfos
{
	CONVERT_CHAR_ARRAY_GROESSE = 50,
	CONVER_CHAR_AUFFUELL_ZEICHEN = 0
};

std::string DoubleTostring( double d );
std::string IntTostring( int i );
std::string UnsignedTostring( unsigned ui );
std::string UnsignedShortTostring( unsigned us );
std::string UnsignedLongTostring( unsigned long ul );
std::string BoolTostring( bool b );

std::string UnsignedShortHexTostring( unsigned short us );
unsigned short HexInUnsignedShort( const char* c );

const char* UnsignedShortToCharArray( unsigned short us, unsigned int &lng);

char* ConstCharToChar( const char* cArray);

std::string CharTostring( char c );

std::string stringVerkuerzen( std::string sEingabe, unsigned uiAsciilaenge );
std::string stringAuffuellen( std::string sEingabe, unsigned uiAsciilaenge );

unsigned int stringToChecksum( std::string sTransaktionsnummer, std::string sKommando, std::string sDatenlaenge, std::string sDaten );


void Convert_DoubleToChar( char DestChar[], double SrcDouble, unsigned uiZahlenGroesse );
void Convert_IntegerToChar( char DestChar[], int SrcInteger, unsigned uiZahlenGroesse );
void Convert_UnsignedShortToChar( char DestChar[], unsigned short SrcUnsignedShort, unsigned uiZahlenGroesse );
char* Convert_UnsignedintegerToChar( unsigned int ui, unsigned uiZahlenGroesse );

void My_strcpy(char* Dest, const char* Src);
void My_strncpy(char* Dest, const char* Src, size_t size);

void CharArrayAuffuellen(char Array[], unsigned uiZahlenGroesse);

std::vector<std::string> split( char* str,const char* delim );




#endif