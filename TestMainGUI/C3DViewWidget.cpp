#include "stdafx.h"
#include "C3DViewWidget.h"
#include "C3DFunctions.h"
#include <memory>


// 3D-Lib von 3DDV
#include <OpenGL_Lib/C3DRenderEngine.h>
#include <OpenGL_Lib/OGLWorld.h>
#include <3DLibCpp/DATA_OBJ/C3DScene.h>
#include <3DLibCpp/DATA_OBJ/C3DPointObj.h>
// #include <3DLibCpp/DATA_OBJ/C3DPointObj.h>
// #include <3DLibCpp/DATA_OBJ/C3DTriangleObj.h>
// #include <3DLibCpp/DATA_OBJ/C3DPolyLineObj.h>
// #include <3DLibCpp/DATA_OBJ/C3DPlaneObj.h>
// #include <3DLibCpp\3DBasic\3Dtypes.h>
// #include <3DLibCpp\3DBasic\3DBaseFunctions.h>
// #include <3DLibCpp\3DBasic\C3DPointDistance.h>
// #include <NumericLib\Approximation\Plane\planefit.h>
// #include <3DToolLib\Octree\C3DPointOctree.h>
// #include <3DToolLib\PtCloud\C3DDistanceCalculator.h>
// #include <3DToolLib\PtCloud\NormalCalculator.h>
// #include <3DToolLib\Filter\C3DPointFilter.h>
// #include <3DToolLib\Mesh\2_5_Triangulation\2SemiTriangulation.h>
// #include <BasicLib\Streams\FixedMemoryStreamBuffer.h>

C3DViewWidget::C3DViewWidget(QWidget *parent) : QOpenGLWidget(parent),
	m_p3dWorld( nullptr ),
	m_pRenderEngine( nullptr ),
	m_p3dScene( nullptr ),
	m_eViewMode( VIEW3D ),
	m_bEnableDistanceLine(false )
{
	ui.setupUi(this);

	m_DefaultView[ 0 ] = m_UserView[ 0 ] = 10000.0;
	m_DefaultView[ 1 ] = m_UserView[ 1 ] = 10000.0;
	m_DefaultView[ 2 ] = m_UserView[ 2 ] = 5000.0;
	m_DefaultView[ 3 ] = m_UserView[ 3 ] = 0.0;
	m_DefaultView[ 4 ] = m_UserView[ 4 ] = 0.0;
	m_DefaultView[ 5 ] = m_UserView[ 5 ] = 0.0;
}

C3DViewWidget::~C3DViewWidget()
{

}

//////////////////////////////////////////////////////////////////////////
#pragma region Slots

void C3DViewWidget::slResetToDefaultView( void )
{
	if ( nullptr != m_p3dWorld )
	{
		C3DCamera kam = m_p3dWorld->GetCamera();

		// Setzen der Kamera auf die Standardposition
		kam.SetPosition(
			m_DefaultView[ 0 ], m_DefaultView[ 1 ], m_DefaultView[ 2 ],
			m_DefaultView[ 3 ], m_DefaultView[ 4 ], m_DefaultView[ 5 ] );

		kam.SetUpVector( 0, 0, 1 );
		m_p3dWorld->SetCamera( kam );

		update();
	}
}

void C3DViewWidget::slMemorizeUserView( void )
{
	if ( nullptr != m_p3dWorld )
	{
		C3DCamera kam = m_p3dWorld->GetCamera();
		T3DVECF camPos = kam.Position();
		T3DVECF camLookPoint = kam.LookVector();
		m_UserView[ 0 ] = camPos[ 0 ];
		m_UserView[ 1 ] = camPos[ 1 ];
		m_UserView[ 2 ] = camPos[ 2 ];
		m_UserView[ 3 ] = camPos[ 0 ] + camLookPoint[ 0 ];
		m_UserView[ 4 ] = camPos[ 1 ] + camLookPoint[ 1 ];
		m_UserView[ 5 ] = camPos[ 2 ] + camLookPoint[ 2 ];
	}
}

void C3DViewWidget::slResetToUserView( void )
{
	if ( nullptr != m_p3dWorld )
	{
		C3DCamera kam = m_p3dWorld->GetCamera();

		// Setzen der Kamera auf die Standardposition
		kam.SetPosition(
			m_UserView[ 0 ], m_UserView[ 1 ], m_UserView[ 2 ],
			m_UserView[ 3 ], m_UserView[ 4 ], m_UserView[ 5 ] );

		kam.SetUpVector( 0, 0, 1 );
		m_p3dWorld->SetCamera( kam );

		update();
	}
}

void C3DViewWidget::slZoomToObject( void )
{
	if ( nullptr != m_p3dWorld )
	{
		// 		m_p3dWorld->ZoomCameraToBoundbox( p3DObj->GetBoundBox() );
		// 		update();
	}
}

void C3DViewWidget::slSetViewMode( ViewMode viewMode )
{
	m_eViewMode = viewMode;
}

void C3DViewWidget::slMeasureDistance( void )
{
	setCursor( QCursor( Qt::CrossCursor ) );
	slSetViewMode( MEASURE_POINT_DISTANCE );
}

#pragma endregion 

//////////////////////////////////////////////////////////////////////////
#pragma region OpenGL

void C3DViewWidget::initializeGL()
{
	// OGL-Context der 3D-Lib
	m_p3dWorld = new OGLWorld();

	// Renderengine der 3D-Lib erstellen
	m_pRenderEngine = new C3DRenderEngine();

	// Szene fuer die 3D-Elemente
	m_p3dScene = new C3DScene();

	// Alle 3 muessen erfolgreich erstellt worden sein
	if ( nullptr != m_p3dWorld &&
		nullptr != m_pRenderEngine &&
		nullptr != m_p3dScene )
	{
		// Shader-Rendering ist nun moeglich
		glewExperimental = TRUE;
		glewInit();

		// Blauer Hintergrund
		m_p3dWorld->SetBackColor( OGLColor( 0x333333 ) );

		// Rendercontext init
		m_p3dWorld->gluInitializeContext();

		// Groesse der Welt setzen (20m)
		m_p3dWorld->SetWorldDimension( 20000, 1000 );

		// Rechtwinklige Darstellung
		m_p3dWorld->SetProjectionMode( C3DCamera::ProjectionMode::ProjectionParallel );

		// Kamera init
		slResetToDefaultView();
		m_p3dWorld->gluInitializeCamera();

		m_pRenderEngine->InitRenderEngine();

	}
	else
	{
		if ( nullptr != m_p3dWorld )
		{
			delete m_p3dWorld;
			m_p3dWorld = nullptr;
		}

		if ( nullptr != m_pRenderEngine )
		{
			delete m_pRenderEngine;
			m_pRenderEngine = nullptr;
		}

		if ( nullptr != m_p3dScene )
		{
			delete m_p3dScene;
			m_p3dScene = nullptr;
		}
	}
}

void C3DViewWidget::resizeGL( int w, int h )
{
	if ( nullptr != m_p3dWorld )
	{
		// Aenderungen am Ausgabefenster anpassen
		m_p3dWorld->gluSetViewport( 0, 0, w, h );
	}
}

void C3DViewWidget::paintGL()
{
	QPainter painter( this );

	painter.beginNativePainting();

	glPushClientAttrib( GL_CLIENT_ALL_ATTRIB_BITS );

	// GL-Rendercontext vorbereiten
	if ( nullptr != m_p3dWorld )
	{
		int iFlags = GLV_SHOW_GRADIENT;

// 		if ( true == m_bShowCoordinateCross )
		{
			iFlags |= GLV_SHOWAXIS;
		}

// 		if ( true == m_bShowFloor )
		{
			iFlags |= GLV_SHOWPLANE;

		}
		m_p3dWorld->gluRenderBegin( iFlags, 0, 0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	}

	C3DShowMode mode;
	mode.Initialize();
	mode.SetItem( SM3D::Points, C3DShowMode::On );
	mode.SetItem( SM3D::Triangles, C3DShowMode::On );
	mode.SetItem( SM3D::Colors, C3DShowMode::On );
	mode.SetItem( SM3D::BackfaceCulling, C3DShowMode::Off );
	mode.SetItem( SM3D::Boundbox, C3DShowMode::Off );
	mode.SetItem( SM3D::Gripper_Move, C3DShowMode::Off );
	mode.SetItem( SM3D::Gripper_Rotate, C3DShowMode::Off );
	mode.SetItem( SM3D::Gripper_Scale, C3DShowMode::Off );

	glEnable( GL_DEPTH_TEST ) ;

	if ( nullptr != m_pRenderEngine )
	{
		m_pRenderEngine->RenderScene( m_p3dScene, mode, RENDERMODE_NORMAL );
	}

	glDisable( GL_DEPTH_TEST ) ;

	glPopClientAttrib();

	painter.endNativePainting();

	if ( ViewMode::MEASURE_POINT_DISTANCE == m_eViewMode &&
		true == m_bEnableDistanceLine )
	{
		// Vermessungsline zeichnen
		drawMeasurementLine( painter, m_mesasureDistanceStartPoint.first );
	}
}

#pragma endregion 

//////////////////////////////////////////////////////////////////////////
#pragma region Maus

void C3DViewWidget::mousePressEvent( QMouseEvent * event )
{
	switch ( m_eViewMode )
	{
		case VIEW3D:
		{
			// Wenn linker oder rechter Maus-Button gedrueckt wurden, wird 
			// die Mausposition innerhalb des Fensters gespeichert
			// Verhindert Spruenge in der Darstellung
			if ( event->button() == Qt::LeftButton ||
				event->button() == Qt::RightButton )
			{
				// Alle Mausbewegungen werden nur noch hier behandelt
				setMouseTracking( true );

				// Mauszeiger verbergen
				::ShowCursor( false );

				// Speichern der Position an der die Maustaste gedrueckt wurde
				m_oldWidgetMousePos = event->pos();

				//Zum Zuruecksetzen des Mauszeigers die globale Position speichern
				m_oldGlobalMousePos = event->globalPos();

				event->accept();
			}
			else
			{
				event->ignore();
			}
			break;
		}
		case MEASURE_POINT_DISTANCE:
		{
			if ( event->button() == Qt::LeftButton )
			{
				// Alle Mausbewegungen werden nur noch hier behandelt
				setMouseTracking( true );
				event->accept();

				// Merken des ersten Klicks
				m_mesasureDistanceStartPoint.first = event->pos();
				m_mesasureDistanceStartPoint.second = get3DCoordinateFrom2DPixel( event->pos() );

				// Line zum MausCursor darf gezeichnet werden
				m_bEnableDistanceLine = true;

				setCursor(QCursor(Qt::CrossCursor));
			}
			else
			{
				event->ignore();
			}

			break;
		}
	}

	QOpenGLWidget::mousePressEvent( event );
}

void C3DViewWidget::mouseReleaseEvent( QMouseEvent * event )
{
	switch ( m_eViewMode )
	{
		case VIEW3D:
		{
			if ( event->button() == Qt::LeftButton ||
				event->button() == Qt::RightButton )
			{
				// Mausereignisse koennen auch wieder woanders bearbeitet werden
				setMouseTracking( false );

				// Mauszeiger wieder auf die globale Position zuruecksetzen
				QCursor::setPos( m_oldGlobalMousePos );

				// Mauszeiger wieder anzeigen
				::ShowCursor( true );

				event->accept();
			}
			else
			{
				event->ignore();
			}
			break;
		}
		case MEASURE_POINT_DISTANCE:
		{
			if ( event->button() == Qt::LeftButton )
			{
				m_bEnableDistanceLine = false;
				slSetViewMode( VIEW3D );
				setCursor( QCursor( Qt::ArrowCursor ) );

				double dDistance = m_mesasureDistanceStartPoint.second.distanceToPoint( get3DCoordinateFrom2DPixel( event->pos() ) );

				emit sigShowStatusBarMessage( QString::number ( dDistance, 'f', 2 ) );	

				update();
				event->accept();
			}
			else
			{
				event->ignore();
			}
			break;
		}
	}

	QOpenGLWidget::mouseReleaseEvent( event );
}

void C3DViewWidget::mouseMoveEvent( QMouseEvent * event )
{
	switch ( m_eViewMode )
	{
		case VIEW3D:
		{
			if ( event->buttons() == Qt::LeftButton || event->buttons() == Qt::RightButton )
			{
				// Beschleunigungsfaktor abhaengig von gedrueckter Taste
				double dFaktor = 0.2;
				if ( event->modifiers() == Qt::ShiftModifier )
				{
					// Langsamer
					dFaktor = 0.05;
				}
				else if ( event->modifiers() == Qt::ControlModifier )
				{
					// Schneller
					dFaktor = 2;
				}

				// Festellen der Aenderung der Mausposition
				double dDeltaMouseX = ( m_oldWidgetMousePos.x() - event->x() ) * dFaktor;
				double dDeltaMouseY = ( m_oldWidgetMousePos.y() - event->y() ) * dFaktor;

				m_oldWidgetMousePos = event->pos() ;

				// Bewegen der Kamera entsprechender der Mausaenderung
				if ( event->buttons() == Qt::RightButton )
				{
					camMove( dDeltaMouseX, dDeltaMouseY );
				}
				// Drehen der Ansicht
				else if ( event->buttons() == Qt::LeftButton )
				{
					camRotate( dDeltaMouseX, dDeltaMouseY );
				}

				// Ausgabefenster soll neu gezeichnet werden
				update();

				event->accept();
			}
			else
			{
				event->ignore();
			}
			break;
		}
		case MEASURE_POINT_DISTANCE:
		{
			if ( event->buttons() == Qt::LeftButton )
			{
				// Neuzeichnen nachdem die Maus bewegt wurde
				update();
			}
			break;
		}
	}

	QOpenGLWidget::mouseMoveEvent( event );
}

void C3DViewWidget::wheelEvent( QWheelEvent * event )
{
	switch ( m_eViewMode )
	{
		case VIEW3D:
		{
			// Abfragen des Werts der Mausraddrehung inkl. Richtung
			double zoom = ( (double) event->delta() ) / 120.0;
			zoomWorld( zoom );

			// Ausgabefenster soll neu gezeichnet werden
			update();

			event->accept();
			break;
		}
	}

	QOpenGLWidget::wheelEvent( event );
}

#pragma endregion 

//////////////////////////////////////////////////////////////////////////
#pragma region Szenensteuerung

void C3DViewWidget::camMove( double dDeltaX, double dDeltaY )
{
	if ( nullptr != m_p3dWorld )
	{
		// Wegstrecke des Verschiebens wird durch den Abstand zum Objekt veraendert
		double dist = m_p3dWorld->GetCamera().radius * 0.005;
		dDeltaX *= dist;
		dDeltaY *= dist;

		C3DCamera kam = m_p3dWorld->GetCamera();

		T3DVECF cp = kam.lookPoint;
		T3DVECF pos = kam.Position();

		C3DMatrix  matrix = m_p3dWorld->GetCamera().PositionMatrix();

		double x = 0.0;
		double y = dDeltaX;
		double z = -dDeltaY;

		// Verschiebungsvektor in Welt berechnen
		matrix.TransformVector( x, y, z );

		// Position der Kamera und der Lookpoint mueesen gleich verschoben werden
		kam.SetPosition(
			pos.x + x, pos.y + y, pos.z + z,
			cp.x + x, cp.y + y, cp.z + z );
		m_p3dWorld->SetCamera( kam );
	}
}

void C3DViewWidget::camRotate( double dDeltaX, double dDeltaY )
{
	if ( nullptr != m_p3dWorld )
	{
		C3DCamera kam = m_p3dWorld->GetCamera();

		kam.Rotate( dDeltaX, dDeltaY, 0, C3DCamera::RotationMode::RotateAboutLookPoint );

		m_p3dWorld->SetCamera( kam );
	}
}

void C3DViewWidget::zoomWorld( double dZoom )
{
	if ( nullptr != m_p3dWorld )
	{
		// Aktuellen Kamera Radius Abfragen
		C3DCamera kam = m_p3dWorld->GetCamera();
		double radius = kam.radius;

		// Zoomstufe 
		dZoom = radius * 0.15f * dZoom;

		// Veraendern des Kameraradiussses zum Zoomen auf das Objekt
		kam.SetRadius( dZoom, true );
		m_p3dWorld->SetCamera( kam );
	}
}

#pragma endregion 

bool C3DViewWidget::loadPointCloudToScene( const QString& fileName, QString& rObjName, int& rObjSceneID )
{
	if ( nullptr != m_p3dScene )
	{
		// 3D-Function-Objekt anlegen damit dieses die Daten aus der Datei laden kann
		auto pObj3D = std::make_unique<C3DFunctions>(this);

		connect(
			pObj3D.get(),
			SIGNAL( sigProgressPercent( int ) ),
			this,
			SIGNAL( sigProgressPercent( int ) ) );

		auto obj3D = pObj3D->loadPointCloud( fileName );

		disconnect(
			pObj3D.get(),
			SIGNAL( sigProgressPercent( int ) ),
			this,
			SIGNAL( sigProgressPercent( int ) ) );
		
		if ( nullptr != obj3D )
		{
			// Objekt wird der Szene uebergeben, diese uebernimmt die Herrschaft darueber
			if ( true == m_p3dScene->InsertObject( obj3D ) )
			{
				// ID des Objects in der Szene ermitteln
				rObjSceneID = obj3D->GetID();

				// Namen ermitteln 
				rObjName = obj3D->GetName();

				m_p3dScene->SetObjectActive( obj3D );				

				// Alles erfolgreich geladen
				return true;
			}
			else
			{
				delete obj3D;
				obj3D = nullptr;
			}
		}
	}

	// Werte fuer Fehler setzen
	rObjName = "";
	rObjSceneID = -1;
	return false;
}

QVector3D C3DViewWidget::get3DCoordinateFrom2DPixel( QPoint pos )
{
	QVector3D point;
	if ( nullptr != m_p3dScene )
	{
		// Aktueller OpenGL Context
		makeCurrent();

		// Modelviewmatrix fuer Sichtebenenprojektion
		GLdouble temp_mMatrix[ 16 ];
		glGetDoublev( GL_MODELVIEW_MATRIX, (GLdouble*) temp_mMatrix );

		// Projektionsmatrix fuer Sichtebenenprojektion
		GLdouble temp_pMatrix[ 16 ];
		glGetDoublev( GL_PROJECTION_MATRIX, (GLdouble*) temp_pMatrix );

		// Viewport fuer die Projektion
		GLint    temp_vMatrix[ 4 ];
		glGetIntegerv( GL_VIEWPORT, temp_vMatrix );

		GLdouble temp3DCoordinate[ 3 ];

		gluUnProject(
			pos.x(),
			pos.y(),
			0,
			temp_mMatrix,
			temp_pMatrix,
			temp_vMatrix,
			temp3DCoordinate,
			temp3DCoordinate+1,
			temp3DCoordinate+2 );

		point.setX( *temp3DCoordinate );
		point.setY( *( temp3DCoordinate + 1 ) );
		point.setZ( *( temp3DCoordinate + 2 ) );

		doneCurrent();
	}

	return point;
}

void C3DViewWidget::drawMeasurementLine( QPainter& rPainter, QPoint const & rStartPoint )
{
	// OpenGL Context aktuell setzen
	makeCurrent();

	// Sichern der alten Zustaende
	QPen oldPen = rPainter.pen();
	QBrush oldBrush = rPainter.brush();

	QPen drawPen( Qt::red );
	rPainter.setPen( drawPen );

	// Koordinaten in Widget transformieren
	QPoint cursorPos = mapFromGlobal( QCursor::pos() ) ;

	// zur aktuellen Mausposition zeichnen
	rPainter.drawLine(
		rStartPoint.x(),
		rStartPoint.y(),
		cursorPos.x(),
		cursorPos.y() );

	// Alten Zustand wiederherstellen
	rPainter.setPen( oldPen );
	rPainter.setBrush( oldBrush );
}
