// ========================================================================= //
//                                                                           //
//                                                                           //
// ========================================================================= //
#ifndef ___THREAD_H_INCLUDED__
#define ___THREAD_H_INCLUDED__

//!	This file is intended to implement a wrapper class for thread functions
// ---- global includes ----------------------------------------------------- ;

#include <windows.h>
#include <process.h>
#include <direct.h>

// ---- constants ----------------------------------------------------------- ;

// ---- typedefs ------------------------------------------------------------ ;

class CThread;
typedef void ( *THREAD_FUNCTION ) ( CThread* pclThread );

// ---- class definition ---------------------------------------------------- ;

/**
	This class encapsulates the thread functions for different platforms
*/
class CThread
{

private:

	// ---- private data ---------------------------------------------------- ;

	// ---- private auxiliary functions -------------------------------------- ;


protected:
	
	// ---- protected data -------------------------------------------------- ;

		unsigned int m_uiStackSize;
		char* m_pcStack;
		HANDLE m_hThreadHandle;
		volatile bool m_bThreadRunFlag;
		volatile bool m_bThreadStopFlag;

	// ---- protected auxiliary functions ------------------------------------- ;

public:

	// ---- public data ------------------------------------------------------- ;

		void* m_pvThreadObject;
		THREAD_FUNCTION m_pfuThreadFunction;

		// ---- constructors / destructor ----------------------------------------- ;

		// default constructor
		CThread();
		
		// destructor
		~CThread( void );

	// ---- modify functions -------------------------------------------------- ;
	
		void setThreadStackSize( unsigned int uiSize );

	// ---- I/O functions ----------------------------------------------------- ;

	// ---- exec functions ---------------------------------------------------- ;

		int createThread( THREAD_FUNCTION pfuThreadFunction, void* pvThreadObject );
		
		/// called inside the thread function before leaving the thread
		void exitThread(); 

		/// called outside the thread function to terminate the thread
		void terminateThread();

		/// check outside the thread function to check running
		volatile bool checkThreadRun();

		/// check inside the thread function to check termination
		volatile bool checkThreadStop();
};

#endif //__CONMON_THREAD_H_INCLUDED__

