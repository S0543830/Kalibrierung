#include "Client.h"

// +++++++++++++++++++++++++++++  Public Methoden  ++++++++++++++++++++++++++++

//++++++++ Konstruktor  ++++++++
Client::Client(void)
{
}

//++++++++ initialisiert Socket und stellt Verbindung her  ++++++++
int Client::connectSocket(std::string strIp, unsigned short usPort)
{
	long p_lVal;
	int p_iVal = 0;
	try
	{
		WSADATA wsaData;
		p_lVal = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (0 == p_lVal)
		{
			m_objSocketClient = socket(AF_INET, SOCK_STREAM, 0);


			m_clientinfo.sin_addr.s_addr = inet_addr(strIp.c_str());
			m_clientinfo.sin_family = AF_INET;
			m_clientinfo.sin_port = htons(usPort);
			m_iClientInfoLen = sizeof(m_clientinfo);


			p_lVal = connect(m_objSocketClient, (struct sockaddr*)&m_clientinfo, m_iClientInfoLen);
			if (SOCKET_ERROR != p_lVal)
			{
				p_iVal = 0;
			}
			else
			{
				m_objDebugOut.debug(0, "Fehler connect Socket");
				p_iVal = -1;
				WSACleanup();
			}
		}
	}
	catch (...)
	{
		m_objDebugOut.debug(0, "Fehler connect Socket");
	}
	// return 0 = succes, -1 = Error
	return p_iVal;
}

//++++++++ Sendet NAchricht  ++++++++
int Client::sendMessage(std::string strSendBuf)
{
	int p_iVal = 0;
	try
	{
		p_iVal = send(m_objSocketClient, strSendBuf.c_str(), static_cast<int>(strSendBuf.length()), 0);
		if (0 <= p_iVal)
		{
			p_iVal = 0;
		}
		else
		{
			m_objDebugOut.debug(0, "Fehler Client.sendMessage");
			p_iVal = -1;
		}
	}
	catch (...)
	{
		m_objDebugOut.debug(0, "Fehler Client.sendMessage");
	}
	// return 0 = succes, -1 = Error
	return p_iVal;
}

//++++++++ Empfaengt Nachricht  ++++++++
int Client::receiveMessage(char* cBuffer, int iSize)
{
	int p_iVal = -1;

	p_iVal = recv(this->m_objSocketClient, cBuffer, iSize, 0);
	if (0 >= p_iVal)
	{
		m_objDebugOut.debug(0, "Fehler receiveMessage");
		p_iVal = -1;
	}
	return p_iVal;
}

//++++++++ schlieﬂt die Verbindung und den Socket  ++++++++
void Client::closeSocket()
{
	m_objDebugOut.debug(0, "Close Socket");
	closesocket(m_objSocketClient);
	WSACleanup();
}

//++++++++ Destruktor  ++++++++
Client::~Client(void)
{
}
