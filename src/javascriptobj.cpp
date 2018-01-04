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
													,tr("Bookmarks Files (*.json *.html)")
													);
	//app::importBookmarks(fileName);
}
void JavaScriptObj::changeDownloadPath()
{
	QString dirName = QFileDialog::getExistingDirectory(nullptr
													,tr("The path to save files")
													,QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
													,QFileDialog::ShowDirsOnly
													);
	if(!dirName.isEmpty()) app::conf.downloadPath = dirName;
	emit signal_reload();
}

void JavaScriptObj::toggleVal(const QString &param)
{
	//int val=(app::getVal(param)=="1" or app::getVal(param)=="true")?0:1;
	//app::setVal(param,QString::number(val));
	if(param == "switchToTheTab") app::conf.switchToNewTab = !app::conf.switchToNewTab;
	if(param == "historySave") app::conf.historySave = !app::conf.historySave;
	if(param == "enableLocalStorage") app::conf.web.enableLocalStorage = !app::conf.web.enableLocalStorage;
	if(param == "enableJavaApplets") app::conf.web.enableJavaApplets = !app::conf.web.enableJavaApplets;
	if(param == "enableJavascript") app::conf.web.enableJavascript = !app::conf.web.enableJavascript;
	if(param == "autoLoadImages") app::conf.web.autoLoadImages = !app::conf.web.autoLoadImages;
	qDebug()<<"JavaScriptObj::toggleVal"<<param;
	emit signal_reload();
}

void JavaScriptObj::bookmarkRemove(const QString &array, const QString &param)
{
	if(array == "main"){
		for(auto elem = app::conf.bookmarks.begin(); elem != app::conf.bookmarks.end(); elem++){
			if( (*elem).first == param ){
				app::conf.bookmarks.erase(elem);
				break;
			}
		}
		emit signal_reload();
	}
}
void JavaScriptObj::bookmarkMoveTo(const QString &arrayTo, const QString &param)
{
	//app::moveToBookmark(arrayIn,arrayTo,param);
	qDebug()<<arrayTo<<param;
	emit signal_reload();
}
void JavaScriptObj::setVal(const QString &param, const QString &value)
{
	qDebug()<<"JavaScriptObj::setVal"<<param<<value;
	if(param == "homePage") app::conf.homePage = value;
	if(param == "openBrowser") app::conf.openBrowser = value;
}
