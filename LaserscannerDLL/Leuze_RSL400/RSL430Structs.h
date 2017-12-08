#pragma once

#pragma pack(1)

namespace RSL430 
{
	typedef struct tagHeader1
	{
		unsigned long TotalSize;
		unsigned char Size;
		unsigned char FollowFlag;
		unsigned short RegID;
	} 
	Header1, *pHeader1;

	typedef struct tagHeader2
	{
		unsigned char Size;
		unsigned char Dest; // Destination
		unsigned char Source;
		unsigned char TeleType;//Telegram Typ
	}
	Header2, *pHeader2;

	typedef struct tagUDPTelegramType
	{
		Header1 H1;
		Header2 H2;
		unsigned short ID;
		unsigned short BlockNr;
		unsigned long ScanNr;
	}
	UDPTelegramType, *pUDPTelegramType;

	typedef struct tagMessKontBesch
	{
		unsigned short Start;
		unsigned short Stop;
		unsigned short Resolution;
		unsigned short Res;
	}
	MessKontBesch, *pMessKontBesch;

	typedef struct tagStateImageType
	{
		unsigned char tmp[20];
		MessKontBesch KontBesch;
	}
	StateImageType, *pStateImageType;

}

	
#pragma pack()