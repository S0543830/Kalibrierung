#ifndef ComDev_Header
#define ComDev_Header

void* COMDEV_Init(char*, char*, unsigned long);
unsigned long COMDEV_Write(void*, char*, unsigned long);
unsigned long COMDEV_Read(void*, char*, unsigned long);
int COMDEV_Close(void*);

#endif
