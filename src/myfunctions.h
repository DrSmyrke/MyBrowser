#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

#include <QSize>
#include <QString>
//FIXME: remove
#include <QDebug>

namespace guiCfg {
	const extern QSize buttonSize;
}
namespace app{
	const extern QString version;
	extern QString themePage;
	extern QString dataDir;
	extern std::map<QString,QString> bookmarks;
	QString getHtmlPage(const QString &title, const QString &content);
	void chkDirs();
	void createDefaultTheme();
	void createConfig();
	void saveConf();
	void loadConf();
	void importBookmarks(const QString &file);
	void addUnsortBookmark(const QString &param, const QString &val);
	std::map<QString,QString>* getArrayBookmark(const QString &array);
	void loadBookmarks();
	void saveBookmarks();
	QString getVal(const QString &param);
	void setVal(const QString &param, const QString &val);
	QString getValInArray(const QString &array,const QString &param);
	void setValInArray(const QString &array,const QString &param, const QString &val);
	std::map<QString, QString> *getArray(const QString &array);
	const extern QString defaultThemeData;
}

#endif // MYFUNCTIONS_H
