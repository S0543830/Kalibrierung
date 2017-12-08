#include "stdafx.h"
#include "Progress3DLibRecv.h"


Progress3DLibRecv::Progress3DLibRecv(void)
{
}
Progress3DLibRecv::~Progress3DLibRecv(void)
{
}
void Progress3DLibRecv::OnSetProgressPercent( int iProgressPercent )
{
	m_3DLibQtWraperObj.SetProgressPercent( iProgressPercent);
}

//////////////////////////////////////////////////////////////////////////
// Qt-Wrapper
C3DLibQtWrapper::C3DLibQtWrapper( QObject *  /*= 0*/ )
{

}
C3DLibQtWrapper::~C3DLibQtWrapper()
{

}
void C3DLibQtWrapper::SetProgressPercent( int iProgressPercent)
{
	emit sigProgressPercent( iProgressPercent );
}
