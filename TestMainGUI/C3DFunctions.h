#ifndef C3DFUNCTIONS_H
#define C3DFUNCTIONS_H

#include <QObject>

class C3DPointObj;

class C3DFunctions : public QObject
{
	Q_OBJECT

public:
	C3DFunctions( QObject *parent );
	~C3DFunctions();

	//////////////////////////////////////////////////////////////////////////
#pragma region Signale
signals :
	// Signalisieren von Fortschritten von Funktionen die von
	// der 3D-Lib abhaengig sind
	void sigProgressPercent( int iProgressPercent );

#pragma endregion 

	//////////////////////////////////////////////////////////////////////////
#pragma region Slots
public slots :

#pragma endregion 

	//////////////////////////////////////////////////////////////////////////
#pragma region Memberfunktionen

public:

	/********************************************************************
	Erstellt:		30.8.2016   10:56
	Autor:			M.Matern
	Beschreibung:	Pruefen ob CNK-Datei eine Punktwolke ist	
	Parameter:	
	IN  const QString& fileName - Dateiname
	Rueckgabe:	
	bool - Datei enthaelt gesuchten Datentyp = true
	*********************************************************************/
	static bool check3DChunkFilePointCloudTyp( const QString& fileName );

	/********************************************************************
	Erstellt:		30.8.2016   10:56
	Autor:			M.Matern
	Beschreibung:	Pruefen ob CNK-Datei ein Mesh ist	
	Parameter:	
	IN  const QString& fileName - Dateiname
	Rueckgabe:	
	bool - Datei enthaelt gesuchten Datentyp = true
	*********************************************************************/
	static bool check3DChunkFileTriangleMeshTyp( const QString& fileName );

	/********************************************************************
	Erstellt:		30.8.2016   10:56
	Autor:			M.Matern
	Beschreibung:	Pruefen ob CNK-Datei eine Gruppe ist
	Parameter:
	IN  const QString& fileName - Dateiname
	Rueckgabe:	
	bool - Datei enthaelt gesuchten Datentyp = true
	*********************************************************************/
	static bool check3DChunkFileGroupTyp( const QString& fileName );

	/********************************************************************
	Erstellt:		8:5:2015   12:30
	Autor:			M.Matern
	Beschreibung: Laden einer Punktewolke aus einer cnk-Datei oder fls-Datei
	Parameter:
	IN const QString& fileName - voll qualifizierter Dateiname
	Rueckgabe:
	C3DPointObj* - Zeiger auf das geladene Punktwolkenobjekt, nullptr bei Fehler
	*********************************************************************/
	C3DPointObj* loadPointCloud( const QString& fileName );


protected:
private:
	static bool C3DFunctions::check3DChunkFileTyp( const QString& fileName, int& riType );
	
	/********************************************************************
	Erstellt:		2.9.2015   16:41
	Autor:			M.Matern
	Beschreibung:	Template zum Laden eines 3D-Objekts aus einer cnk-Datei	
	Parameter:	
	IN const QString& fileName - voll qualifizierter Dateiname
	Rueckgabe:
	Typ* - Zeiger auf das erfolgreich geladenen Objekt, anonsten nullptr
	*********************************************************************/
	template<class Object3DType>  
	Object3DType* load3DObject( const QString& fileName );

	/********************************************************************
	Erstellt:		7.7.2017   15:36
	Autor:			M.Matern
	Beschreibung:	Zum Laden eines 3D-Objekts aus einer ASC-Datei	
	Parameter:	
	IN const QString& fileName - voll qualifizierter Dateiname
	Rueckgabe:
	Typ* - Zeiger auf das erfolgreich geladenen Objekt, anonsten nullptr
	*********************************************************************/
	C3DPointObj* loadPointCloudFromASC( const QString& fileName );

#pragma endregion 
		
};

#endif // C3DFUNCTIONS_H
