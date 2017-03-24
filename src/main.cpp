#include "mainwindow.h"
#include "myfunctions.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	#if defined(Q_OS_WIN)
		app::dataDir=QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
	#elif defined(Q_OS_UNIX)
		app::dataDir=QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/.MyBrowser";
	#endif
	app::chkDirs();
	QString appdir = a.applicationDirPath();
	QString filename = QLocale::system().name();
	QTranslator translator(&a);
	if(!translator.load(filename,appdir + "/lang/"))
		translator.load(filename,app::dataDir + "/lang/");
	a.installTranslator(&translator);

	MainWindow w;
	w.show();

	return a.exec();
}
