#define  STRICT
#define  _WIN32_WINNT  0x0500

/***************************************************************************************************

Windows- und C-Runtime-Header

***************************************************************************************************/


#include <Winsock2.h>
#include <windows.h>
#include <wchar.h>
#include <stdio.h>

/***************************************************************************************************

Gemeinsame Headerdatei für alle Anwendungen - Verbindung zur static Lib

***************************************************************************************************/

#include "ComDev.h"
#include "Position.h"


/***************************************************************************************************

Headerdateien für diese Anwendung

***************************************************************************************************/


//----------------- static´s-------------
static char        axis = '1';
static double      Incr_pro_mm = 67.264573;         // Incremente / mm
static void*      HCom;
static unsigned long	   MaxStep;
static unsigned long	   MaxFreq;
//-----------------funktionen------------

int POSITION_StartUp(void){
     unsigned long   ComLen = 0;
     char     data[128] = {0};
     char     buf[128] = {0};
     unsigned char     status = 0;


     HCom = COMDEV_Init("COM1", "baud=9600 parity=N data=8 stop=1", 100);

     if(HCom == INVALID_HANDLE_VALUE)
        return FALSE;

//Parameter erfragen
     //Achsenbegrenzung
     ComLen = GenCom("PG?", buf);

     COMDEV_Write(HCom, buf, ComLen);
     if(COMDEV_Read(HCom, buf, sizeof(buf)) == 0){
        COMDEV_Close(HCom);
        return FALSE;
     }

     if(Analyse(buf, data, &status) == FALSE){
        COMDEV_Close(HCom);
        return FALSE;
     }

     MaxStep = atol(data);


     //Maximale Frequenz
     ComLen = GenCom("IF?", buf);

     COMDEV_Write(HCom, buf, ComLen);
     if(COMDEV_Read(HCom, buf, sizeof(buf)) == 0){
        COMDEV_Close(HCom);
        return FALSE;
     }

     if(Analyse(buf, data, &status) == FALSE){
        COMDEV_Close(HCom);
        return FALSE;
     }

     MaxFreq = atol(data);

//Parameter setzen
     //Lauffrequenz
     ComLen = GenCom("PF1000", buf);

     COMDEV_Write(HCom, buf, ComLen);
     COMDEV_Read(HCom, buf, sizeof(buf));

     //Offset vom Neg Initiator
     ComLen = GenCom("PM50", buf);

     COMDEV_Write(HCom, buf, ComLen);
     COMDEV_Read(HCom, buf, sizeof(buf));

     //Offset vom Pos Initiator
     ComLen = GenCom("PP50", buf);

     COMDEV_Write(HCom, buf, ComLen);
     COMDEV_Read(HCom, buf, sizeof(buf));

     //Status-Abfrage
     ComLen = GenCom("IS?", buf);

     COMDEV_Write(HCom, buf, ComLen);
     COMDEV_Read(HCom, buf, sizeof(buf));


     Analyse(buf, data, &status);
     if((status & 0x79) != 0){
        COMDEV_Close(HCom);
        return FALSE;
     }

     return TRUE;

}//int POSITION_StartUp()


int POSITION_CleanUp(void){
	if(HCom != INVALID_HANDLE_VALUE)
		return(COMDEV_Close(HCom));
	else
		return (FALSE);
}//int POSITION_CleanUp()


int POSITION_NegInit(void){
     unsigned long   ComLen;
     char     data[128] = {0};
     char     buf[128] = {0};
     unsigned char     status = 0;

     ComLen = GenCom("GI-", buf);
     COMDEV_Write(HCom, buf, ComLen);
     if(COMDEV_Read(HCom, buf, sizeof(buf)) == 0)
        return FALSE;

     do{
        ComLen = GenCom("PC?", buf);
        COMDEV_Write(HCom, buf, ComLen);
        COMDEV_Read(HCom, buf, sizeof(buf));
        Analyse(buf, data, &status);
     }while(status & 0x01);

     return TRUE;
}//int POSITION_NegInit()

int POSITION_SetAbsPos(double pos_mm){
     unsigned long   ComLen;
     char     command[128] = {0};
     char     buf[128] = {0};
     unsigned char     status = 0;
     unsigned long    pos;

     //Positionsangabe umwandeln
     pos = (unsigned long)(pos_mm * Incr_pro_mm + 0.5);

     if(pos > MaxStep){
        MessageBox(NULL, "ungültige Positionsangabe!!", "Fehler", MB_OK);
        return FALSE;
     }//if()

     sprintf_s(command, 128, "GA%d", pos);
     ComLen = GenCom(command, buf);
     COMDEV_Write(HCom, buf, ComLen);
     COMDEV_Read(HCom, buf, sizeof(buf));

     ComLen = GenCom("PC?", command);
     do{
          COMDEV_Write(HCom, command, ComLen);
          COMDEV_Read(HCom, buf, sizeof(buf));
          Analyse(buf, NULL, &status);
     }while(((status & 0x01) == 1) && ((status & 0x78) == 0));

     if((status & 0x78) > 0){
          ComLen = GenCom("IS?", buf);
          COMDEV_Write(HCom, buf, ComLen);
          COMDEV_Read(HCom, buf, sizeof(buf));
          MessageBox(NULL, "Fehler sind aufgetreten", "Fehler", MB_OK);
          MessageBox(NULL, buf, "Fehlercode", MB_OK);
          return FALSE;
     }//if

     return TRUE;

}//int POSITION_SetAbsPos()

int POSITION_SetRelPos(double pos_rel_mm){
     unsigned long   ComLen;
     char     command[128] = {0};
     char     buf[128] = {0};
     unsigned char     status = 0;
     unsigned long    pos;


     POSITION_GetPos_steps(&pos);
     pos += (unsigned long)(pos_rel_mm * Incr_pro_mm + 0.5);
     if(pos < 0 || pos > MaxStep){
        MessageBox(NULL, "ungültige Position", "Fehler", MB_OK);
        return FALSE;
     }//if

     //Positionsangabe umwandeln
     if(pos_rel_mm < 0){
          pos = (unsigned long)(pos_rel_mm*(-1) * Incr_pro_mm + 0.5);
		  sprintf_s(command,128, "GR-%d", pos);
     }
     else{
          pos = (unsigned long)(pos_rel_mm * Incr_pro_mm + 0.5);
		  sprintf_s(command, 128, "GR+%d", pos);
     }//if() else

     ComLen = GenCom(command, buf);
     COMDEV_Write(HCom, buf, ComLen);
     COMDEV_Read(HCom, buf, sizeof(buf));
     Analyse(buf, NULL, &status);
     if(status & 0x78){
        MessageBox(NULL, "Fehler", "Fehler", MB_OK);
        return FALSE;
     }//if()

     ComLen = GenCom("PC?", command);
     do{
          COMDEV_Write(HCom, command, ComLen);
          COMDEV_Read(HCom, buf, sizeof(buf));
          Analyse(buf, NULL, &status);
     }while(((status & 0x01) == 1) && ((status & 0x78) == 0));

     if((status & 0x78) > 0){
          ComLen = GenCom("IS?", buf);
          COMDEV_Write(HCom, buf, ComLen);
          COMDEV_Read(HCom, buf, sizeof(buf));
          MessageBox(NULL, "Fehler sind aufgetreten", "Fehler", MB_OK);
          MessageBox(NULL, buf, "Fehlercode", MB_OK);
          return FALSE;
     }//if

     return TRUE;

}//int POSITION_SetRelPos()

int POSITION_SetSpeed(unsigned long speed){
     unsigned long   ComLen;
     char     command[128] = {0};
     char     buf[128] = {0};
     unsigned char     status = 0;

     if((speed > MaxFreq) || (speed < 10))
        return FALSE;

	 sprintf_s(command, 128, "PF%d", speed);
     ComLen = GenCom(command, buf);
     COMDEV_Write(HCom, buf, ComLen);
     COMDEV_Read(HCom, buf, sizeof(buf));
     Analyse(buf, NULL, &status);
     if(status != 0x00)
        return FALSE;
     return TRUE;
}//int POSITION_SetSpeed()

int POSITION_GetPos_steps(unsigned long *pos){
     unsigned long   ComLen;
     char     data[128] = {0};
     char     buf[128] = {0};
     unsigned char     status = 0;

     ComLen = GenCom("PC?", buf);
     COMDEV_Write(HCom, buf, ComLen);
     COMDEV_Read(HCom, buf, sizeof(buf));
     Analyse(buf, data, &status);
     if(status != 0x00)
        return FALSE;

     *pos = atol(data);

     return TRUE;
}//int POSITION_GetPos_steps()

int POSITION_GetPos(unsigned long *pos){
     unsigned long   ComLen;
     char     data[128] = {0};
     char     buf[128] = {0};
     unsigned char     status = 0;

     ComLen = GenCom("PC?", buf);
     COMDEV_Write(HCom, buf, ComLen);
     COMDEV_Read(HCom, buf, sizeof(buf));
     Analyse(buf, data, &status);
     if(status != 0x00)
        return FALSE;

     *pos = (unsigned long)(atol(data) / Incr_pro_mm + 0.5);

     return TRUE;
}//int POSITION_GetPos()

int POSITION_GetMaxPos(unsigned long *pos){
     *pos = (unsigned long)(MaxStep / Incr_pro_mm + 0.5);

     return TRUE;
}//int POSITION_GetMaxPos()




/*
int POSITION_GetStatus(unsigned long *st){
     unsigned long   ComLen;
     char     data[128] = {0};
     char     buf[128] = {0};
     unsigned char     short_status = 0;
     unsigned long    long_status = 0;

     *st = 0;

     ComLen = GenCom("IS?", buf);
     COMDEV_Write(HCom, buf, ComLen);

     COMDEV_Read(HCom, buf, sizeof(buf));

     if(Analyse(buf, data, &short_status) == FALSE)
        return FALSE;

     long_status += short_status;

     long_status |= CharToNibble(data[0])<<12;
     long_status |= CharToNibble(data[1])<<8;
     long_status |= CharToNibble(data[2])<<20;
     long_status |= CharToNibble(data[3])<<16;
     long_status |= CharToNibble(data[4])<<28;
     long_status |= CharToNibble(data[5])<<24;

     *st = long_status;

     return TRUE;

}//DOWRD POSITION_GetStatus()
*/

unsigned char CalcCheckSum(char *data, unsigned char len){
     unsigned char  checksum = 0;
     WORD  i;

     for(i = 0; i < len; i++){
           checksum ^= (unsigned char)data[i];
     }//for()

     return(checksum);
}//unsigned char CalcCheckSum()



unsigned long GenCom(char *command, char *data){
       unsigned char   comlen;
       unsigned char   cs;

       comlen = strlen(command);

       data[0] = 0x02;
       data[1] = axis;

       memcpy(data + 2, command, comlen);

       data[comlen+2] = ':';
       data[comlen+3] = 'X';
       data[comlen+4] = 'X';
       data[comlen+5] = 0x03;
       data[comlen+6] = '\0';

       cs = CalcCheckSum(data + 1, comlen + 2);

       data[comlen+3] = NibbleToChar(cs>>4);
       data[comlen+4] = NibbleToChar(cs & 0x0F);

       return(comlen + 6);
}//unsigned long GenCom()

char NibbleToChar(unsigned char nibble)
{
     char tmp;

     if(nibble > 9)
         tmp = nibble + 0x37;
     else
         tmp = nibble + 0x30;

     return tmp;
}//char NibbleToChar()

unsigned char CharToNibble(char c)
{
     if(c > 0x40)
         return c - 0x37;
     else
         return c - 0x30;
}//unsigned char CharToNibble1()


int Analyse(char* answ, char* data, unsigned char* status){
     char   axis='1';

     unsigned short   start = 0xFFFF;
	 unsigned short   end = 0;
     unsigned long i;
     unsigned char   cs = 0;

     *status = 0;
//Antwortstring im Gesamtstring suchen
     for(i = 0; i < strlen(answ); i++)
         if(answ[i] == 0x02){
            start = i + 1;
            break;
         }//if()
     if(start == 0xFFFF) return FALSE;

     for(i = start; i < strlen(answ); i++)
         if(answ[i] == 0x03){
            end = i - 1;
            break;
         }//if()

//Plausibilitätsprüfung
     if(end == 0) return FALSE;
     if(answ[start] != axis) return FALSE;
     if((end - start) < 6) return FALSE;



//Datenfeld kopieren
     if(data != NULL){
          memset(data, 0, strlen(data));
          if(answ[start + 4] != ':')
              memcpy(data, answ + start + 4, end - start - 6);
     }//if()

//Checksumme extrahieren
     cs = CharToNibble(answ[end - 1]);
     cs = cs<<4;
     cs += CharToNibble(answ[end]);

//Checksumme aus String errechnen und vergleichen
     if(cs != CalcCheckSum(answ + start, end - start -1))
           return FALSE;

//Kurz-Status extrahieren
     *status = CharToNibble(answ[start + 1]);
     *status = *status<<4;
     *status += CharToNibble(answ[start + 2]);

     return TRUE;
}//bool Analyse()


