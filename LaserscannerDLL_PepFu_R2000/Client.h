#ifndef __CLIENT_HEADER__
#define __CLIENT_HEADER__

#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>
#include "DebugOutput.h"

#pragma comment(lib,"Ws2_32.lib")

class Client
{
public:

	Client(void);
	~Client(void);

	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: erstellt Client und  stellt eine Verbindung her
	Parameter:
	IN string p_strIp ñ enthaelt IP Adresse
	IN unsigned short p_usPort - enthaelt zu Port
	Rueckgabe:
	int ñ erfolgreich mit groesser 0
	*********************************************************************/
	int connectSocket(std::string p_strIp,unsigned short p_usPort);

	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: sendet Nachricht
	Parameter:
	IN string p_strSendBuf ñ enthaelt zu sendende Nachricht
	Rueckgabe:
	int ñ erfolgreich mit 0
	*********************************************************************/
	int sendMessage(std::string p_strSendBuf);


	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: empfaengt Nachricht
	Parameter:
	IN char* p_cBuffer ñ enthaelt empfangene Nachricht
	IN int p_iSize - Groesse des Buffers
	OUT char* p_cBuffer - enthaelt empfangene Nachricht
	Rueckgabe:
	int ñ erfolgreich mit 0
	*********************************************************************/
	int receiveMessage(char* p_cBuffer, int p_iSize );


	/********************************************************************
	Erstellt:		07.2017
	Autor:			C. Armas Martinez
	Beschreibung: Baut die Verbindung ab und schlieﬂt den Socket
	Parameter:
	Rueckgabe:
	*********************************************************************/
	void closeSocket();

private:	
	CDebugOutput m_objDebugOut;
	WSADATA m_wsaData;
	SOCKET m_objSocketClient;
	sockaddr_in m_clientinfo;
	int m_iClientInfoLen;
};
#endif

