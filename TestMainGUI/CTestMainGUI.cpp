#include "stdafx.h"
#include "ctestmaingui.h"
#include "CProgControl.h"

#include "C3DFunctions.h"
#include <memory>

CTestMainGUI::CTestMainGUI( QWidget *parent )
	: QMainWindow( parent ),
	m_pStatusProgress( nullptr )
{
	ui.setupUi( this );

	// Alten Zustand wiederherstellen
	QCoreApplication::setOrganizationName( "GFaI e.V." );
	QCoreApplication::setOrganizationDomain( "gfai.de" );
	QCoreApplication::setApplicationName( "Thuema-TestMainGIU" );

	QSettings settings;
	restoreGeometry( settings.value( "geometry" ).toByteArray() );
	restoreState( settings.value( "windowState" ).toByteArray() );
	m_strLastWorkingDirectory = settings.value( "LastWorkingDirectory" ).toString();

	// Statusbar mit Fortschrittsleiste
	if ( nullptr == m_pStatusProgress )
	{
		m_pStatusProgress = new QProgressBar( this );
		if ( nullptr != m_pStatusProgress )
		{
			m_pStatusProgress->setVisible( false );
			ui.statusBar->addPermanentWidget( m_pStatusProgress );
		}
	}

	connect(
		ui.openGLWidget3DView,
		SIGNAL( sigShowStatusBarMessage( QString const & ) ),
		this,
		SLOT( slShowStatusBarMessage( QString const & ) ) );


	addLogMessage( tr("Programmstart") );
}

CTestMainGUI::~CTestMainGUI()
{
	QSettings settings;
	settings.setValue( "geometry", saveGeometry() );
	settings.setValue( "windowState", saveState() );
	settings.setValue( "LastWorkingDirectory" , m_strLastWorkingDirectory );
}

int CTestMainGUI::initComp(std::string stXMLPfad)
{
	this->m_objProgCon = new CProgControl();
	int iVal = this->m_objProgCon->initSystem(stXMLPfad);
	if (PROGCONTROL_OK != iVal)
	{
		addLogMessage(tr("Fehler beim Initialisieren des Datenrecorders"));
	}

	return iVal;
}

//////////////////////////////////////////////////////////////////////////
#pragma region Slots

void CTestMainGUI::slStartScan( void )
{
	// Nur Funktionstests/-beispiele
	int iVal = this->m_objProgCon->startRecord();
	if (0 == iVal)
	{
		slShowStatusBarMessage("StartScan");
		addLogMessage("StartScan");
	}
	else
	{
		slShowStatusBarMessage("Fehler bei StartScan");
		addLogMessage("Fehler bei StartScan");
	}
	
}

void CTestMainGUI::slStopScan( void )
{
	int iVal = this->m_objProgCon->stopRecord();
	if (0 == iVal)
	{
		slShowStatusBarMessage("StopScan");
		addLogMessage("StopScan");
	}
	else
	{
		slShowStatusBarMessage("Fehler bei StopScan");
		addLogMessage("Fehler bei StopScan");
	}
}

void CTestMainGUI::slShowStatusBarMessage( const QString& rstrMessage )
{
	// Message in die Statusbar mit 10s Anzeigezeit
	ui.statusBar->showMessage( rstrMessage, 10000 );
}

void CTestMainGUI::slLoadPointCloud( void )
{
	// Filedialog ausfuehren/anzeigen
		// Die ausgeaehlten Dateinamen abfragen
	QStringList fileNames = QFileDialog::getOpenFileNames(
		this,
		tr( "Laden einer Punkwolken-Datei" ),
		m_strLastWorkingDirectory,
		tr( "3D-Punktwolke (*.cnk *.asc)" ) );

	if ( 0 < fileNames.count() )
	{
		// Fortschrittsmeldungen aus der 3D-Lib verbinden
		connectTo3DViewProgress();

		for ( auto iterFileNames = fileNames.begin() ; iterFileNames != fileNames.end() ; ++iterFileNames )
		{
			QString msg = *iterFileNames + " " + tr( "wird geladen." );

			slShowStatusBarMessage( msg );
			addLogMessage( msg );			

			QTime time;
			time.start();

			QFileInfo info( *iterFileNames );

			// Immer das letzte Verzeichnis merken
			m_strLastWorkingDirectory = info.absolutePath();

			// Fuer cnk.Dateien kann schon geprueft werden ob es eine Punktwolke ist
			if ( info.suffix() == QString( "cnk" ) )
			{
				if ( false == C3DFunctions::check3DChunkFilePointCloudTyp( *iterFileNames ) )
				{
					msg =  *iterFileNames + " " + tr( "ist keine Punktwolke." );
					slShowStatusBarMessage( msg );
					addLogMessage( msg );

					auto infoBox = std::make_unique<QMessageBox>(
							QMessageBox::Warning,
							tr( "Falscher Dateityp" ),
							msg,
							QMessageBox::Ok | QMessageBox::Abort,
							this );

					if ( nullptr != infoBox.get() )
					{
						if ( QMessageBox::Abort == infoBox->exec() )
						{
							// keine weitere Datei wird geladen
							break;
						}
						else
						{
							// Die Schleife weitelaufen -> naechste Datei
							continue;
						}
					}
				}		
			}

			QString strObjName;
			int iPointCloud3DSceneID = -1;

			// Nun werden die Daten geladen, wirklich
			bool bLoadSuccess = ui.openGLWidget3DView->loadPointCloudToScene(
				*iterFileNames,
				strObjName,
				iPointCloud3DSceneID );

			if ( true == bLoadSuccess )
			{
				msg =
					"Aus der Datei " + *iterFileNames  +
					" wurde \"" + strObjName +
					QString( "\" mit ID = %1" ).arg( iPointCloud3DSceneID ) +
					QString( " in der Zeit von %1ms" ).arg( time.elapsed() ) + " geladen. ";
			}				
			else
			{
				msg = 				
					*iterFileNames +
					" " +
					tr( "konnte nicht geladen werden." ) ;
			}

			slShowStatusBarMessage( msg );
			addLogMessage( msg );
		}

		// Fortschrittsmeldungen aus der 3D-Lib trennen
		disconnectTo3DViewProgress();

		// Szene neu zeichnen nachdem alles geladen wurde
		ui.openGLWidget3DView->update();
	}
	else
	{
		slShowStatusBarMessage( tr( "Keine Dateien ausgewaehlt." ) );
		addLogMessage( tr( "Keine Dateien ausgewaehlt." ) );
	}
}

#pragma endregion 

//////////////////////////////////////////////////////////////////////////
#pragma region Memberfunktionen

void CTestMainGUI::addLogMessage( const QString& rstrMessage )
{
	ui.lWLog->addItem(
		QTime::currentTime().toString( "hh:mm:ss.zzz" ) +
		" : " +
		rstrMessage );

	// Das letzte Item soll immer sichtbar sein
	ui.lWLog->scrollToBottom();
}

void CTestMainGUI::connectTo3DViewProgress( void )
{
	connect(
		ui.openGLWidget3DView,
		SIGNAL( sigProgressPercent( int ) ),
		m_pStatusProgress,
		SLOT( setValue( int ) ) );

	m_pStatusProgress->setVisible( true );
	m_pStatusProgress->setValue( 0 );
}

void CTestMainGUI::disconnectTo3DViewProgress( void )
{
	m_pStatusProgress->setVisible( false );
	disconnect(
		ui.openGLWidget3DView,
		SIGNAL( sigProgressPercent( int ) ),
		m_pStatusProgress,
		SLOT( setValue( int ) ) );
}

#pragma endregion 


