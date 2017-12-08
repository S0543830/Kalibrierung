// DebugOutput.cpp
#include "DebugOutput.h"

int g_iDebugLevel = 0;
bool g_bDebugFile = false;
bool g_bDebug = true;
std::string g_strDebugFileName = std::string ("debug.txt");
// const char* g_pcDebugFileName = "debug.txt";
bool g_bTimeStamp = true;
bool g_bHumanRedable = true;

double CDebugOutput::m_fInitTime;

CRITICAL_SECTION* CDebugOutput::m_csDebugOutput = NULL;

#define ENTERCS if(m_csDebugOutput!=NULL) EnterCriticalSection(m_csDebugOutput);
#define LEAVECS if(m_csDebugOutput!=NULL) LeaveCriticalSection(m_csDebugOutput);

// ========================================================================== ;
//                                                                            ;
// ---- constructors / destructor ------------------------------------------- ;
//                                                                            ;
// ========================================================================== ;

CDebugOutput::CDebugOutput()
{
}


CDebugOutput::CDebugOutput(const char* pcClassName)
{
	m_acClassName = pcClassName;
}


CDebugOutput::CDebugOutput(const CDebugOutput& clDebugOutput)
{
	m_acClassName = clDebugOutput.m_acClassName;
}


CDebugOutput::~CDebugOutput(void)
{
}

// ========================================================================== ;
//                                                                            ;
// ---- operators ----------------------------------------------------------- ;
//                                                                            ;
// ========================================================================== ;

CDebugOutput& CDebugOutput::operator=(const CDebugOutput& clDebugOutput)
{
	m_acClassName = clDebugOutput.m_acClassName;
	return *this;
}

// ========================================================================== ;
//                                                                            ;
// ---- query functions ----------------------------------------------------- ;
//                                                                            ;
// ========================================================================== ;

bool CDebugOutput::getDebug() const
{
	return g_bDebug;
}

bool CDebugOutput::getDebugFile() const
{
	return g_bDebugFile;
}

int CDebugOutput::getDebugLevel() const
{
	return g_iDebugLevel;
}

// ========================================================================== ;
//                                                                            ;
// ---- modify functions ---------------------------------------------------- ;
//                                                                            ;
// ========================================================================== ;

int CDebugOutput::initDebugOutput(const char* pcClassName)
{
	m_acClassName = pcClassName;
	return 0;
}

void CDebugOutput::setInitTime()
{
		_timeb nowTimeVal;
		_ftime_s(&nowTimeVal);
		m_fInitTime = (nowTimeVal.time + (double(nowTimeVal.millitm)/1e+3));
}

void CDebugOutput::setDebugLevel(int iLevel)
{
	g_iDebugLevel = iLevel;
}

void CDebugOutput::setDebug(bool bFlag)
{
	g_bDebug = bFlag;
}

void CDebugOutput::setDebugFile(bool bFlag)
{
	g_bDebugFile = bFlag;
}


void CDebugOutput::setDebugFileName ( const char * acFileName )
{
	g_strDebugFileName = std::string ( acFileName );
}

void CDebugOutput::setCriticalSection(CRITICAL_SECTION *csMessage)
{
	m_csDebugOutput = csMessage;
}

// ========================================================================== ;
//                                                                            ;
// ---- I/O functions ------------------------------------------------------- ;
//                                                                            ;
// ========================================================================== ;

// ========================================================================== ;
//                                                                            ;
// ---- exec functions ------------------------------------------------------ ;
//                                                                            ;
// ========================================================================== ;

void CDebugOutput::error(const char *pcErrorMessage,...) const
{
	double fSeconds;
	time_t iSeconds;
	int iMicroSeconds;
	int iRetVal;

	ENTERCS;

	va_list args;
	va_start(args, pcErrorMessage);

	_timeb nowTimeVal;
	_ftime_s(&nowTimeVal);
	iSeconds = nowTimeVal.time;
	iMicroSeconds = nowTimeVal.millitm * 1000;
	fSeconds = (nowTimeVal.time + (double(nowTimeVal.millitm)/1e+3)) - m_fInitTime;

	static char acBuffer[255];
	static char acOutBuffer[300];
	vsprintf_s(acBuffer, 255, pcErrorMessage, args);
	sprintf_s(acOutBuffer, 300, "ERROR: %5.3f %s::%s\n", fSeconds, m_acClassName.c_str(), acBuffer);	if (g_bDebugFile==true)
	if (g_bDebugFile==true)
	{
		FILE* hFile;
		iRetVal = fopen_s(&hFile, g_strDebugFileName.c_str(), "a+");
		if(hFile != NULL)
		{
			fprintf(hFile, "%s", acOutBuffer);
			fclose(hFile);
		}
	}

	//OutputDebugString(acOutBuffer);

	va_end(args);
	LEAVECS;

	exit(-1);
}

void CDebugOutput::error(const int iErrorCode, const char *pcErrorMessage,...) const
{
	double fSeconds;
	time_t iSeconds;
	int iMicroSeconds;
	int iRetVal;

	ENTERCS;

	va_list args;
	va_start(args, pcErrorMessage);

		_timeb nowTimeVal;
		_ftime_s ( &nowTimeVal );
		iSeconds = nowTimeVal.time;
		iMicroSeconds = nowTimeVal.millitm * 1000;
		fSeconds = (nowTimeVal.time + (double(nowTimeVal.millitm)/1e+3)) - m_fInitTime;

	static char acBuffer[255];
	static char acOutBuffer[300];
	vsprintf_s ( acBuffer, 255, pcErrorMessage, args);
	sprintf_s ( acOutBuffer, 300, "ERROR: #%i %5.3f %s::%s\n", iErrorCode, fSeconds, m_acClassName.c_str(), acBuffer );
	if (g_bDebugFile==true)
	{
		FILE* hFile;
		iRetVal = fopen_s(&hFile, g_strDebugFileName.c_str(), "a+");
		if(hFile != NULL)
		{
			fprintf(hFile, "%s", acOutBuffer);
			fclose(hFile);
		}
	}
	//OutputDebugString(acOutBuffer);
#if 0
	if (true == g_bHumanRedable)
	{
		char acMicroSeconds[7];
		char acBuffer[80];
		sprintf(acMicroSeconds, "%.6d", iMicroSeconds);
		strftime (acBuffer, 80, "%Y-%m-%d %T.", localtime ((time_t*) &iSeconds));
		strcat (acBuffer, acMicroSeconds);
		fprintf(stderr, "%s ERROR: #%i %s::", acBuffer, iErrorCode, m_acClassName.c_str());
		vfprintf(stderr, pcErrorMessage,args);
		fprintf(stderr, "\n");
	}
	else
	{
		fprintf(stderr, "ERROR: #%i %5.3f %s::", iErrorCode, fSeconds, m_acClassName.c_str());
		vfprintf(stderr, pcErrorMessage,args);
		fprintf(stderr, "\n");
	}
	if (g_bDebugFile==true)
	{
		FILE* hFile;
		hFile=fopen(g_pcDebugFileName,"a+");
		if(hFile != NULL)
		{
			if (true == g_bHumanRedable)
			{
				char acMicroSeconds[7];
				char acBuffer[80];
				sprintf(acMicroSeconds, "%.6d", iMicroSeconds);
				strftime (acBuffer, 80, "%Y-%m-%d %T.", localtime ((time_t*) &iSeconds));
				strcat (acBuffer, acMicroSeconds);
				fprintf(hFile, "%s ERROR: #%i %s::", acBuffer, iErrorCode, m_acClassName.c_str());
				vfprintf(hFile, pcErrorMessage,args);
				fprintf(hFile, "\n");
			}
			else
			{
				fprintf(hFile, "ERROR: #%i %5.3f %s::", iErrorCode, fSeconds, m_acClassName.c_str());
				vfprintf(hFile, pcErrorMessage,args);
				fprintf(hFile, "\n");
			}
			fclose(hFile);
		}
	}
#endif

	va_end(args);
	LEAVECS;


	exit(-1);
}

void CDebugOutput::warning(const char *pcWarningMessage,...) const
{
	if (false == g_bDebug)
	{
		return;
	}

	double fSeconds;
	time_t iSeconds;
	int iMicroSeconds;
	int iRetVal;

	ENTERCS;

	va_list args;
	va_start(args, pcWarningMessage);

	_timeb nowTimeVal;
	_ftime_s(&nowTimeVal);
	iSeconds = nowTimeVal.time;
	iMicroSeconds = nowTimeVal.millitm * 1000;
	fSeconds = (nowTimeVal.time + (double(nowTimeVal.millitm)/1e+3)) - m_fInitTime;

	static char acBuffer[255];
	static char acOutBuffer[300];
	vsprintf_s(acBuffer, 255, pcWarningMessage, args);
	sprintf_s(acOutBuffer, 300, "WARNING: %5.3f %s::%s\n", fSeconds, m_acClassName.c_str(), acBuffer);
	if (g_bDebugFile==true)
	{
		FILE* hFile;
		iRetVal = fopen_s(&hFile, g_strDebugFileName.c_str(), "a+");
		if(hFile != NULL)
		{
			fprintf(hFile, "%s", acOutBuffer);
			fclose(hFile);
		}
	}
	//OutputDebugString(acOutBuffer);

	va_end(args);
	LEAVECS;

}

void CDebugOutput::logging(const char *pcLoggingMessage,...)
{

	static char acBuffer[255];
	FILE *m_hLogFile;
	int iRetVal;

	ENTERCS;

	va_list args;
	va_start(args, pcLoggingMessage);
	vsprintf_s(acBuffer, 255, pcLoggingMessage, args);
	va_end(args);
	iRetVal = fopen_s(&m_hLogFile, "log.txt", "a+");

	if(m_hLogFile != NULL)
	{
		fprintf(m_hLogFile,"%s",acBuffer);
		fclose(m_hLogFile);
	}
	LEAVECS;

}

void CDebugOutput::debug(const int iDebugLevel, 
						 const char *pcDebugMessage,
						 ...) const
{

	if(iDebugLevel > g_iDebugLevel || !g_bDebug)
	{
		return;
	}

	double fSeconds;
	time_t iSeconds;
	int iMicroSeconds;
	int iRetVal;

	ENTERCS;

	va_list args;
	va_start(args, pcDebugMessage);

	_timeb nowTimeVal;
	_ftime_s(&nowTimeVal);
	iSeconds = nowTimeVal.time;
	iMicroSeconds = nowTimeVal.millitm * 1000;
	fSeconds = (nowTimeVal.time + (double(nowTimeVal.millitm)/1e+3)) - m_fInitTime;
	static char acBuffer[80];
	static char acBuffer2[255];
	static char acOutBuffer[300];

	vsprintf_s(acBuffer2, 255, pcDebugMessage, args);

	if (true == g_bHumanRedable)
	{
		char acMicroSeconds[7];
		struct tm _tm;

		if(true == g_bTimeStamp)
		{
			sprintf_s(acMicroSeconds, 7, "%.6d", iMicroSeconds);
			iRetVal = localtime_s (&_tm, (time_t*) &iSeconds);
			strftime (acBuffer, 80, "%Y-%m-%d %H:%M:%S.", &_tm);
			strcat_s (acBuffer, 80, acMicroSeconds);
			sprintf_s(acOutBuffer, 300, "%s DEBUG: %i %s::%s\n", acBuffer, iDebugLevel, m_acClassName.c_str(), acBuffer2);
		}
		else
		{
			sprintf_s(acOutBuffer, 300, "DEBUG: %i %s::%s\n", iDebugLevel, m_acClassName.c_str(), acBuffer2);
		}
	}
	else
	{
		sprintf_s(acOutBuffer, 300, "DEBUG: %i %5.3f %s::%s\n", iDebugLevel, fSeconds, m_acClassName.c_str(), acBuffer2);
	}
	if (g_bDebugFile==true)
	{
		FILE* hFile;
		iRetVal = fopen_s(&hFile, g_strDebugFileName.c_str(), "a+");
		if(hFile != NULL)
		{
			fprintf(hFile, "%s", acOutBuffer);
			fclose(hFile);
		}
	}
	OutputDebugStringA(acOutBuffer);

	va_end(args);
	LEAVECS;

}
