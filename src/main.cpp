#include "mainwindow.h"
#include "global.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	#if defined(Q_OS_WIN)
		app::conf.dataDir=QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
	#elif defined(Q_OS_UNIX)
		app::conf.dataDir=QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.MyBrowser";
	#endif

	app::conf.version = "0.3";
	app::conf.guiCfg.buttonSize = QSize(26,26);
	app::loadSettings();


	QString filename = QLocale::system().name();
	QTranslator translator(&a);
	if(translator.load(filename,"://lang/")) a.installTranslator(&translator);

	MainWindow w;
	w.show();

	return a.exec();
}
