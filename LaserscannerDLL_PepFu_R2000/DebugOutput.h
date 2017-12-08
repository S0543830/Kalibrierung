// DebugOutput.h

// ---- local includes ------------------------------------------------------ ;

// ---- global includes ----------------------------------------------------- ;

// #include <stdio.h>
// #include <stdlib.h>
// #include <stdarg.h>
// #include <string.h>
#include <string>
// #include <sstream>
// #include <list>
// #include <vector>
// #include <fstream>

// #include "afxsock.h"
#include <windows.h>
// #include <winbase.h>
#include <time.h>
// #include <iostream>
// #include <list>
// #include <iterator>
// #include <queue>
// #include <limits.h>
// #include <float.h>
#include "sys\timeb.h"


#ifndef __DEBUGOUTPUT_H_INCLUDED__
#define __DEBUGOUTPUT_H_INCLUDED__

// ---- globals ----------------------------------------------------------- ;

extern int g_iDebugLevel;
extern bool g_bDebugFile;
extern bool g_bDebug;
extern std::string g_strDebugFileName;
extern bool g_bTimeStamp;
extern bool g_bHumanRedable;

// ---- typedefs ------------------------------------------------------------ ;

// ---- class definition ---------------------------------------------------- ;

/**
	Debug message class.
	This class manages the debug messages and should be used as base class for all classes which should be debuged
*/
class CDebugOutput
{

private:

	// ---- private data ---------------------------------------------------- ;

	static CRITICAL_SECTION *m_csDebugOutput;

protected:

	// ---- protected data -------------------------------------------------- ;

		std::string m_acClassName;
		static double m_fInitTime;

public:

	// ---- public data ------------------------------------------------------- ;

	// ---- constructors / destructor ----------------------------------------- ;

		// default constructor
		CDebugOutput(void);
		CDebugOutput(const char* pcClassName);
		// copy constructor
		CDebugOutput(const CDebugOutput& clDebugOutput);
		// destructor
		virtual ~CDebugOutput(void);

	// ---- operators --------------------------------------------------------- ;

		// assignment operator
		CDebugOutput& operator=(const CDebugOutput& clDebugOutput);

	// ---- query functions --------------------------------------------------- ;

		bool getDebug() const;
		bool getDebugFile() const;
		int getDebugLevel() const;

	// ---- modify functions -------------------------------------------------- ;


		int initDebugOutput(const char* pcClassName);

		static void setInitTime(void);
		void setDebug(bool bFlag);
		void setDebugFile(bool bFlag);
		void setDebugLevel(int iLevel);
		void CDebugOutput::setDebugFileName ( const char * acFileName );

		static void setCriticalSection(CRITICAL_SECTION *cs);

	// ---- I/O functions ----------------------------------------------------- ;

	// ---- exec functions ---------------------------------------------------- ;
		void logging(const char *pcLoggingMessage,...);

		// output of a debug message with a debug level
		void debug(const int iDebugLevel,
				   const char *pcDebugMessage,...) const;

		// output of a warning message
		void warning(const char *pcWarningMessage,...) const;

		// output of an error message
		void error ( const int iErrorCode, const char *pcErrorMessage,...) const;
		void error ( const char *pcErrorMessage,...) const;
};


#endif // __DEBUGOUTPUT_H_INCLUDED__
