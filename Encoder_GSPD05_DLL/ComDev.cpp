/***************************************************************************************************

Defines für Windows

***************************************************************************************************/

#define  STRICT
#define  _WIN32_WINNT  0x0500

/***************************************************************************************************

Windows- und C-Runtime-Header

***************************************************************************************************/

#include <windows.h>
#include <stdio.h>


void* COMDEV_Init(
	char*           Name,
	char*	        Command, //baud=9600 parity=N data=8 stop=1
	unsigned long	        ReadTimeout)
{
	DCB   		dcb;
	void*		HCom;
	COMMTIMEOUTS	TimeOut;
        int            err = FALSE;

	HCom = CreateFile(Name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if(HCom == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "COMDEV_Init Create File", "Fehler", MB_OK);
	}
	else
	{
		if(GetCommState( HCom, &dcb ) == FALSE)
		{
                        err = TRUE;
                        MessageBox(NULL, "COMDEV_Init GetCommState", "Fehler", MB_OK);
		}

		if(BuildCommDCB(Command, &dcb) == FALSE)
		{
                        err = TRUE;
                       	MessageBox(NULL, "COMDEV_Init BuildCimmDCB", "Fehler", MB_OK);
		}

		if(SetCommState(HCom, &dcb)== FALSE)
		{
                        err = TRUE;
                        MessageBox(NULL, "COMDEV_Init SetCommState", "Fehler", MB_OK);
		}

		if(GetCommTimeouts(HCom, &TimeOut) == FALSE)  // time-out values
		{
                        err = TRUE;
                        MessageBox(NULL, "COMDEV_Init GetCommTimeouts", "Fehler", MB_OK);
		}
		TimeOut.ReadTotalTimeoutConstant = ReadTimeout; // 100 milliseconds;


		if(SetCommTimeouts(HCom, &TimeOut) == FALSE)  // time-out values
		{
                        err = TRUE;
                        MessageBox(NULL, "COMDEV_Init SetCommTimeouts", "Fehler", MB_OK);
		}


	}

        if(err == TRUE){
               MessageBox(NULL, "Initialisierung NICHT erfolgreich!!", "FEHLER", MB_OK);
               CloseHandle(HCom);
               HCom = INVALID_HANDLE_VALUE;
        }//if

	return(HCom);
}


unsigned long COMDEV_Write(
	void*		HCom,
	char*           data,
	unsigned long           size)
{
	unsigned long		RVal;
	unsigned long		NrBytesWritten;

	if((RVal = WriteFile(HCom, (void*)data, size, &NrBytesWritten, NULL)) == FALSE)
	{
		MessageBox(NULL, "WriteFile", "Fehler", MB_OK);
		RVal = 0;
	}
	else
		RVal = NrBytesWritten;

	return(RVal);
}

unsigned long COMDEV_Read(
      void*       HCom,
	char*        data,
      unsigned long        size){

      unsigned long        RVal;
      unsigned long        NrBytesRead;

      memset(data, 0, strlen(data));

      if((RVal = ReadFile(HCom, (void*)data, size, &NrBytesRead, NULL)) == FALSE)
      {
      	 MessageBox(NULL, "Read File", "Fehler", MB_OK);
	 RVal = 0;
      }
      else
          RVal = NrBytesRead;

      return(RVal);
}//unsigned long COMDEV_Read()

int COMDEV_Close(
	void*	HCom)
{
	int 	RVal;

	RVal = CloseHandle(HCom);

	if(RVal == FALSE)
	{
            MessageBox(NULL, "Close", "Fehler", MB_OK);
	}
	return(RVal);
}






