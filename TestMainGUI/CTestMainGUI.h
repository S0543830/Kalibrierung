#ifndef CTESTMAINGUI_H
#define CTESTMAINGUI_H

#include <QtWidgets/QMainWindow>
#include "ui_ctestmaingui.h"
#include "CProgControl.h"

class CTestMainGUI : public QMainWindow
{
	Q_OBJECT

public:
	CTestMainGUI(QWidget *parent = 0);
	~CTestMainGUI();

	int initComp(std::string stXMLPfad);

	//////////////////////////////////////////////////////////////////////////
#pragma region Signale

signals :
#pragma endregion 


	//////////////////////////////////////////////////////////////////////////
#pragma region Slots
public slots :

	void slStartScan( void );
	void slStopScan( void );
	void slShowStatusBarMessage( const QString& rstrMessage );

	void slLoadPointCloud( void );

#pragma endregion 
	
	//////////////////////////////////////////////////////////////////////////
#pragma region Memberfunktionen
public:
protected:
private:	

	void addLogMessage( const QString& rstrMessage );
	void connectTo3DViewProgress( void );
	void disconnectTo3DViewProgress( void );

#pragma endregion 

	 //////////////////////////////////////////////////////////////////////////
#pragma region Membervariablen
public:
protected:
private:
	Ui::CTestMainGUIClass ui;
	
	// Fortschrittsbalken der unten im Hauptfenster bei Bedarf eingeblendet wird
	QProgressBar* m_pStatusProgress;

	QString m_strLastWorkingDirectory;

	CProgControl* m_objProgCon;
#pragma endregion 


};

#endif // CTESTMAINGUI_H
