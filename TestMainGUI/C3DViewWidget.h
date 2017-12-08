#ifndef C3DVIEWWIDGET_H
#define C3DVIEWWIDGET_H

#include <QOpenGLWidget>
#include "ui_C3DViewWidget.h"

// Forward declaration
class C3DRenderEngine;	// Rendern der 3D-Elemente 
class OGLWorld;			// OGL Context der 3D-Lib
class C3DScene;			// 3D Elemente der 3D-Lib


class C3DViewWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	enum ViewMode
	{
		VIEW3D = 0, // Normale 3D Anzeige
		MEASURE_POINT_DISTANCE = 1 // MessTool
	};


public:
	C3DViewWidget(QWidget *parent = 0);
	~C3DViewWidget();

	//////////////////////////////////////////////////////////////////////////
#pragma region Signale
signals:
   // Signalisieren von Fortschritten
   void sigProgressPercent( int iProgressPercent );
   void sigMeasuredDistance( double dMeasuredDistance );
   void sigShowStatusBarMessage( QString const & rLogMessage );

#pragma endregion 


	//////////////////////////////////////////////////////////////////////////
#pragma region Slots
public slots :

	void slMemorizeUserView( void );
	void slResetToUserView( void );
	void slResetToDefaultView( void );
	void slZoomToObject( void );
	void slSetViewMode( ViewMode viewMode );
	void slMeasureDistance( void );

#pragma endregion 

	//////////////////////////////////////////////////////////////////////////
#pragma region Membervariablen
public:
protected:
private:
	Ui::C3DViewWidget ui;
	
	C3DRenderEngine* m_pRenderEngine;	// Rendern der 3D-Elemente 
	OGLWorld* m_p3dWorld;			// OGL Context der 3D-Lib
	C3DScene* m_p3dScene;			// 3D Elemente der 3D-Lib

	QPoint m_oldWidgetMousePos;	// alte Position der Maus innerhalb des Widgets
	QPoint m_oldGlobalMousePos;	// alte globale Position der Maus 

	// Hier wird die Ansicht hinterlegt die als Standardansicht definiert ist
	double m_DefaultView[ 6 ];
	double m_UserView[ 6 ];

	// Aktueller ViewMode der 3D Anzeige
	ViewMode m_eViewMode;

	// Zur Abstandsmessung auf dem Bildschirm, mit Entsprechung im 3D-Raum
	QPair<QPoint, QVector3D>  m_mesasureDistanceStartPoint;
	
	// Ermoeglicht es eine Linie zum Cursor zu zeichnen
	bool m_bEnableDistanceLine;
	
#pragma endregion 


	//////////////////////////////////////////////////////////////////////////
#pragma region Memberfunktionen

public:
	/********************************************************************
	Erstellt:		8:5:2015   12:30
	Autor:			M.Matern
	Beschreibung: Laden einer Punktewolke aus einer cnk-Dateio der fls-Datei
	Parameter:
	IN const QString& fileName - voll qualifizierter Dateiname
	OUT QString& rObjName - Objektname, wird der Datei entnommen
	OUT int& rObjSceneID - ID des Objekts in der 3D-Szene
	Rueckgabe:
	bool - laden erfolgreich = true
	*********************************************************************/
	bool loadPointCloudToScene( const QString& fileName, QString& rObjName, int& rObjSceneID );

protected:
private:
	/********************************************************************
	Erstellt:		8.5.2015   14:37
	Autor:			M.Matern
	Beschreibung:	Initialisieren des OpenGL-Fensters	
	Parameter:		
	Rueckgabe:		
	*********************************************************************/
	void initializeGL();

	/********************************************************************
	Erstellt:		8.5.2015   14:38
	Autor:			M.Matern
	Beschreibung:	Bei veraenderung der Fenstergroesse muessen einige 
	Parameter angepasst werden
	Parameter:	
	IN int w - neue Fensterbreite
	IN int h - neue Fensterhoehe
	Rueckgabe:		
	*********************************************************************/
	void resizeGL(int w, int h);

	/********************************************************************
	Erstellt:		8.5.2015   14:40
	Autor:			M.Matern
	Beschreibung:	Neuzeichnen des OpenGL-Fensters	
	Parameter:		
	Rueckgabe:		
	*********************************************************************/
	void paintGL();

	/********************************************************************
	Erstellt:		8.5.2015   14:42
	Autor:			M.Matern
	Beschreibung:	Reagieren auf das druecken einer Maustaste	
	Parameter:	
	IN QMouseEvent * event - Enthaehlt die Informationen des Events
	Rueckgabe:		
	*********************************************************************/
	void mousePressEvent  ( QMouseEvent * event );

	/********************************************************************
	Erstellt:		8.5.2015   14:42
	Autor:			M.Matern
	Beschreibung:	Reagieren auf das bewegen der Maus
	Parameter:	
	IN QMouseEvent * event - Enthaehlt die Informationen des Events
	Rueckgabe:		
	*********************************************************************/
	void mouseMoveEvent   ( QMouseEvent * event );

	/********************************************************************
	Erstellt:		8.5.2015   14:42
	Autor:			M.Matern
	Beschreibung:	Reagieren auf das bewegen des Mausrades
	Parameter:	
	IN QMouseEvent * event - Enthaehlt die Informationen des Events
	Rueckgabe:		
	*********************************************************************/
	void wheelEvent       ( QWheelEvent * event );

	/********************************************************************
	Erstellt:		8.5.2015   14:42
	Autor:			M.Matern
	Beschreibung:	Reagieren auf das loslassen einer Maustaste	
	Parameter:	
	IN QMouseEvent * event - Enthaehlt die Informationen des Events
	Rueckgabe:		
	*********************************************************************/
	void mouseReleaseEvent( QMouseEvent * event );
	
	/********************************************************************
	Erstellt:		8.5.2015   14:54
	Autor:			M.Matern
	Beschreibung:	Bewegen der virtuellen 3D-Kamera auf der Bildschirmebene
	Parameter:	
	IN double dDeltaX - Translation der Kamera in x-Achse 
	IN double dDeltaY - Translation der Kamera in y-Richtung
	Rueckgabe:		
	*********************************************************************/
	void camMove  ( double dDeltaX,	double dDeltaY );

	/********************************************************************
	Erstellt:		8.5.2015   14:57
	Autor:			M.Matern
	Beschreibung:	Rotieren der virtuellen 3D-Kamera um den Sichtpunkt	um die Achsen
	der Bildschirmebene
	Parameter:		
	IN double dDeltaX - Rotation der Kamera um x-Achse
	IN double dDeltaY - Rotation der Kamera um y-Achse
	Rueckgabe:		
	*********************************************************************/
	void camRotate( double dDeltaX,	double dDeltaY );

	/********************************************************************
	Erstellt:		8.5.2015   14:59
	Autor:			M.Matern
	Beschreibung:	Zoomen der viruellen Kamera auf den aktuell gesetzten Sichtpunkt	
	Parameter:	
	IN double dZoom - Aenderung der Zoomstufe, wird in den Abstander zum 
	Sichtpunkt gewandelt
	Rueckgabe:		
	*********************************************************************/
	void zoomWorld( double dZoom );
	
	/********************************************************************
		Erstellt:		13.7.2017   9:07
		Autor:			M.Matern
		Beschreibung:	Wandelt eine Bildschirmkoordiante in einen 3D Koordinate	
		Parameter:	
		IN	QPoint pos - Bildschirmkoordinate
		Rueckgabe:		
			QVector3D const & - 3D-Koordinate
	*********************************************************************/
	QVector3D get3DCoordinateFrom2DPixel( QPoint pos );
	
	/********************************************************************
		Erstellt:		13.7.2017   8:29
		Autor:			M.Matern
		Beschreibung:	Zeichnet eine Linie vom Startpunkt zur Cursor-Position
		Parameter:		
		IN QPainter& rPainte - aktiver Painter des Widgets
		IN QPoint const & rStartPoint- Startpunkt
	*********************************************************************/
	void drawMeasurementLine( QPainter& rPainter, QPoint const & rStartPoint );

#pragma endregion 

	 //////////////////////////////////////////////////////////////////////////
#pragma region TEST
public:
	// Hier kommen nur Testfunktionen und Variablen hin, sobald sie bestaetigt 
	// sind werden diese in entsprechende Bereiche verschoben

#pragma endregion 

};

#endif // C3DVIEWWIDGET_H
