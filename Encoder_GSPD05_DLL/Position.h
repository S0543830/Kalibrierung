#ifndef Position_Header
#define Position_Header

#include<stdio.h>

int POSITION_StartUp(void);
int POSITION_CleanUp(void);
int POSITION_NegInit(void);
int POSITION_SetAbsPos(double);
int POSITION_SetRelPos(double);
int POSITION_GetStatus(unsigned long*);
int POSITION_GetPos_steps(unsigned long*);
int POSITION_GetPos(unsigned long*);
int POSITION_GetMaxPos(unsigned long*);
int POSITION_SetSpeed(unsigned long speed);

char NibbleToChar(unsigned char);
unsigned char CharToNibble(char);
unsigned char CalcCheckSum(char*, unsigned char);
unsigned long GenCom(char*, char*);
int Analyse(char*, char*, unsigned char*);


#endif