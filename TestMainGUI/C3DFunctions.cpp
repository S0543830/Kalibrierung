#include "stdafx.h"
#include "C3DFunctions.h"
#include "Progress3DLibRecv.h"

#include <3DLibCpp\3DBasic\3Dtypes.h>
#include <3DLibCpp\DATA_OBJ\C3DObjectTypes.h>
#include <3DLibCpp\DATA_OBJ\C3DObjectIO.h>
#include <3DLibCpp/DATA_OBJ/C3DPointObj.h>


C3DFunctions::C3DFunctions(QObject *parent)
	: QObject(parent)
{
}

C3DFunctions::~C3DFunctions()
{

}

bool C3DFunctions::check3DChunkFilePointCloudTyp( const QString& fileName )
{
	int iTyp = O3D_C3DPOINTOBJ ;
	return check3DChunkFileTyp( fileName, iTyp );
}

bool C3DFunctions::check3DChunkFileTriangleMeshTyp( const QString& fileName )
{
	int iTyp = O3D_C3DTRIANGLEOBJ ;
	//int iTyp = O3D_C3DMESHOBJ;

	return check3DChunkFileTyp( fileName, iTyp );
}

bool C3DFunctions::check3DChunkFileGroupTyp( const QString& fileName )
{
	int iTyp = O3D_C3DGROUPOBJ ;
	return check3DChunkFileTyp( fileName, iTyp );
}

bool C3DFunctions::check3DChunkFileTyp( const QString& fileName, int& riType )
{
	bool bRet = false;

	int iType = C3DObjectIO::ReadChunkFormatObjectType( fileName.toLatin1().constData() );

	if ( iType == riType )
	{
		bRet = true;
	}

	// Es wird der ermittelte Typ zurueckgegeben
	riType = iType;

	return bRet;
}

template<class Object3DType>
Object3DType* C3DFunctions::load3DObject( const QString& fileName )
{
	// Neues 3D-Objekt anlegen damit dieses die Daten aus der Datei laden kann
	auto obj3D = new Object3DType();

	if ( nullptr != obj3D )
	{
		// Ueberpruefen ob dieses 3D-Objekt ladbar ist
		if ( obj3D->GetType() == C3DObjectIO::ReadChunkFormatObjectType( fileName.toLatin1().constData() ) )
		{
			auto progressReceiver = new Progress3DLibRecv();

			if ( nullptr != progressReceiver )
			{
				connect(
					&progressReceiver->m_3DLibQtWraperObj,
					SIGNAL( sigProgressPercent( int ) ),
					this,
					SIGNAL( sigProgressPercent( int ) ) );
			}

			if ( OK != obj3D->LoadFromFile( fileName.toLatin1().constData(), progressReceiver ) )
			{
				delete obj3D;
				obj3D = nullptr;
			}
			else
			{
				//lokale Matrix zuruecksetzen
				C3DMatrix Trafo;
				Trafo.Reset();
				obj3D->SetLocalMatrix( Trafo );
			}

			if ( nullptr != progressReceiver )
			{
				disconnect(
					&progressReceiver->m_3DLibQtWraperObj,
					SIGNAL( sigProgressPercent( int ) ),
					this,
					SIGNAL( sigProgressPercent( int ) ) );

				delete progressReceiver;
			}
		}
		else
		{
			delete obj3D;
			obj3D = nullptr;
		}
	}

	return obj3D;
}

C3DPointObj*  C3DFunctions::loadPointCloudFromASC( const QString& fileName )
{
	// Neues 3D-Objekt anlegen 
	auto obj3D = new C3DPointObj();

	if ( nullptr != obj3D )
	{
		QFile file( fileName );		

		QFileInfo info( fileName );

		emit sigProgressPercent( 30 );
				
		if ( true == file.open( QIODevice::ReadOnly | QFile::Text ) )
		{		
			T3DVECF_ARRAY & rPointsArray = obj3D->GetPoints();
			T3DVECF tmpPoint;
			
			QTextStream inData( &file );
			while ( !inData.atEnd() )
			{
				inData >> tmpPoint[ 0 ] >> tmpPoint[ 1 ] >> tmpPoint[ 2 ];

				if ( !inData.atEnd() )
				{
					rPointsArray.Append( tmpPoint, 1 );
				}
			}

			file.close();
		}

		emit sigProgressPercent( 100 );

		// Nur wenn Punkte gelesen wurden, wird ein Objekt zurueckgegeben
		if ( 0 < obj3D->GetPointCount() )
		{
			return obj3D;
		}
		else
		{
			delete obj3D;
			obj3D = nullptr;
		}
	}

	return obj3D;
}

C3DPointObj* C3DFunctions::loadPointCloud( const QString& fileName )
{
	QFileInfo info( fileName );

	if ( info.suffix() == QString( "cnk" ) )
	{
		return load3DObject<C3DPointObj>( fileName );
	}

	else if ( info.suffix() == QString( "asc" ) )
	{
		return loadPointCloudFromASC( fileName );
	}

	return  nullptr;
}
