// ========================================================================= //
//                                                                           //
//                                                                           //
// ========================================================================= //
#include "Thread.h"

unsigned int __stdcall threadFunction( void* pvThread )
{
	CThread* pclThread = ( CThread* )pvThread;
	( pclThread->m_pfuThreadFunction )( pclThread );
	return 0;
}

// ========================================================================== ;
//                                                                            ;
// ---- constructors / destructor ------------------------------------------- ;
//                                                                            ;
// ========================================================================== ;

CThread::CThread( void ) : m_uiStackSize( 1228000 ), 
						 m_pcStack( 0 ),
						 m_hThreadHandle( 0 ),
						 m_bThreadRunFlag( false ),
						 m_bThreadStopFlag( false ),
						 m_pvThreadObject( 0 ),
						 m_pfuThreadFunction( 0 )
{
}

CThread::CThread( const CThread& clThread )
{

}

CThread::~CThread( void )
{

}

// ========================================================================== ;
//                                                                            ;
// ---- operators ----------------------------------------------------------- ;
//                                                                            ;
// ========================================================================== ;

CThread& CThread::operator=( const CThread& clThread )
{
	//g_clDebugOutput.OutputError( -1, "assignment operator : method should not be called!" );
	return *this;
}

// ========================================================================== ;
//                                                                            ;
// ---- query functions ----------------------------------------------------- ;
//                                                                            ;
// ========================================================================== ;

// ========================================================================== ;
//                                                                            ;
// ---- modify functions ---------------------------------------------------- ;
//                                                                            ;
// ========================================================================== ;

void CThread::setThreadStackSize( unsigned int uiSize )
{
	m_uiStackSize = uiSize;
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

int CThread::createThread( THREAD_FUNCTION pfuThreadFunction, void* pvThreadObject )
{
	m_bThreadStopFlag = false;
	m_pvThreadObject = pvThreadObject;
	m_pfuThreadFunction = pfuThreadFunction;
#if defined( _WIN32 )					
	unsigned int iThreadId;

	m_hThreadHandle = ( HANDLE )_beginthreadex( NULL, 0, threadFunction, ( void* )this, 0, &iThreadId );

	if( m_hThreadHandle == NULL )
	{
		//g_clDebugOutput.OutputWarning( "createThread : creating thread failed!" );
		m_bThreadRunFlag = false;
		return -1;
	}
	else
	{
		m_bThreadRunFlag = true;
		return 0;
	}
#endif
}

void CThread::exitThread()
{
	m_bThreadRunFlag = false;
	// void* vp;

#if defined( __WIN32 )
	 _endthreadex( 0 );
#endif
}

void CThread::terminateThread()
{
	m_bThreadStopFlag = true;
}

volatile bool CThread::checkThreadRun()
{
	return m_bThreadRunFlag;
}

volatile bool CThread::checkThreadStop()
{
	return m_bThreadStopFlag;
}


