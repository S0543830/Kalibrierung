#include "stdafx.h"
#include "ctestmaingui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CTestMainGUI w;
	w.initComp("Thuema_Config_Release.xml");
	w.show();
	return a.exec();
}
