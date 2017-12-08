#ifndef Progress3DLibRecv_H
#define Progress3DLibRecv_H
#include <BasicLib/Basic/ProgressReceiver.h>

#include <QObject>

class C3DLibQtWrapper : public QObject
{
	Q_OBJECT
public:		
	explicit C3DLibQtWrapper(QObject * parent = 0);
	~C3DLibQtWrapper();
signals:
	void sigProgressPercent( int iProgressPercent );

public slots:
	void SetProgressPercent( int iProgressPercent );
};


class Progress3DLibRecv : public ProgressReceiver 
{
public:
	Progress3DLibRecv(void);
	~Progress3DLibRecv(void);

	// Ueber dieses Objekt kann diese Klasse mittels QT-Singale kommunizieren
	C3DLibQtWrapper m_3DLibQtWraperObj;

private:
protected:
	void OnSetProgressPercent( int iProgressPercent );
public:
};
#endif // Progress3DLibRecv_H

