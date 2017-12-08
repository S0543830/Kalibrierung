// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "stdafx.h"
#include "CEncoderAmtec.h"

// Globales Klassenelement
extern CEncoderAmtec* pEncoder;

BOOL APIENTRY DllMain( HMODULE /*hModule*/,
                       DWORD  ul_reason_for_call,
                       LPVOID /*lpReserved*/
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		{
			// Falls einfach beendet wird, noch schnell aufraeumen
			if ( nullptr != pEncoder )
			{
				delete pEncoder;
				pEncoder = nullptr;
			}
			break;
		}
	}
	return TRUE;
}

