#include "javascriptobj.h"

#include <QFileDialog>
#include <QStandardPaths>

JavaScriptObj::JavaScriptObj(QObject *parent) : QObject(parent)
{


}
void JavaScriptObj::importBookmarks()
{
	QString fileName = QFileDialog::getOpenFileName(nullptr
													,tr("Import bookmarks")
													,QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
													,tr("Bookmarks Files (*.json *.xml)")
													);
	app::loadBookmarksJSON(fileName);
}
void JavaScriptObj::changeDownloadPath()
{
	QString dirName = QFileDialog::getExistingDirectory(nullptr
													,tr("The path to save files")
													,QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
													,QFileDialog::ShowDirsOnly
													);
	if(!dirName.isEmpty()) app::setVal("downloadPath",dirName);
}

void JavaScriptObj::toggleVal(const QString &param)
{
	int val=(app::getVal(param)=="1" or app::getVal(param)=="true")?0:1;
	app::setVal(param,QString::number(val));
}
void JavaScriptObj::setVal(const QString &param, const QString &value)
{
	app::setVal(param,value);
}
