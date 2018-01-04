#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSize>
#include <QString>
//FIXME: remove
#include <QDebug>

struct GUIconf{
	QSize buttonSize;
};
struct WebKitConf{
	bool autoLoadImages = true;
	bool enableLocalStorage = true;
	bool enableJavaApplets = false;
	bool enableJavascript = true;
};

struct Settings{
	QString version;
	QString dataDir;
	QString theme = "default";
	QString openBrowser = "blank";
	QString homePage = "http://drsmyrke-home.pskovline.ru";
	QString downloadPath;
	bool switchToNewTab = false;
	bool historySave = false;
	WebKitConf web;
	GUIconf guiCfg;
	QString themePage;
	std::vector<QString> lastPages;
	std::map<QString,QString> bookmarks;
};

namespace app{
	extern Settings conf;

	void loadSettings();
	void saveSettings();
	QString getHtmlPage(const QString &title, const QString &content);
	void chkDirs();
	void createDefaultTheme();
	const extern QString defaultThemeData;
}
namespace mf {
	QString getSize(long val);
}

#endif // GLOBAL_H
