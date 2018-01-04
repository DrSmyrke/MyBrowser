#include "genpages.h"

namespace gp {
	const QString SPACE="&#160;&#160;&#160;";
	QString getSettings()
	{
		std::map<QString,QString> atStarting;
			atStarting[QObject::tr("Show home page")]="homePage";
			atStarting[QObject::tr("Show blank page")]="blank";
			atStarting[QObject::tr("Show tabs opened last time")]="lastTime";
		QString atStartingVals;
		for(auto elem:atStarting){
			QString selected=(app::conf.openBrowser == elem.second)?" selected":"";
			atStartingVals+="<option label=\""+elem.first+"\" value=\""+elem.second+"\""+selected+">"+elem.first+"</option>";
		}
		QString switchToTheTab=(app::conf.switchToNewTab)?"bon":"boff";
		QString enableJavascript=(app::conf.web.enableJavascript)?"bon":"boff";
		QString autoLoadImages=(app::conf.web.autoLoadImages)?"bon":"boff";
		QString enableJavaApplets=(app::conf.web.enableJavaApplets)?"bon":"boff";
		QString enableLocalStorage=(app::conf.web.enableLocalStorage)?"bon":"boff";
		QString historySave=(app::conf.historySave)?"bon":"boff";
		QString content="<div class=\"cbox\">\n"
						"<b>"+QObject::tr("Starting")+"</b><br>\n"
						+SPACE+QObject::tr("At startup:")+" <select onChange=\"obj.setVal('openBrowser',this.value);\">"+atStartingVals+"</select><br>\n"
						+SPACE+QObject::tr("Home page:")+" <input type=\"text\" size=\"30\" onChange=\"obj.setVal('homePage',this.value);\" onKeyUp=\"obj.setVal('homePage',this.value);\" value=\""+app::conf.homePage+"\"><br>\n"
						"<b>"+QObject::tr("Downloads")+"</b><br>\n"
						+SPACE+QObject::tr("The path to save files:")+" <input type=\"text\" size=\"30\" readonly value=\""+app::conf.downloadPath+"\"> <input type=\"button\" value=\""+QObject::tr("Change")+"\" onClick=\"obj.changeDownloadPath();\"><br>\n"
						"<b>"+QObject::tr("Tabs")+"</b><br><table>\n"
						"<tr><td><div class=\""+switchToTheTab+"\" onClick=\"obj.toggleVal('switchToTheTab');this.className=(this.className=='bon')?'boff':'bon';\"></div><span style=\"font-size:12pt;\"></td><td>"+QObject::tr("Switch to the tab that opens")+"</span></td></tr>\n"
						"</table>"
						"<b>"+QObject::tr("Browser")+"</b><br><table>\n"
						"<tr><td><div class=\""+autoLoadImages+"\" onClick=\"obj.toggleVal('autoLoadImages');this.className=(this.className=='bon')?'boff':'bon';\"></div><span style=\"font-size:12pt;\"></td><td>"+QObject::tr("Load images automatically")+"</span></td></tr>\n"
						"<tr><td><div class=\""+enableJavascript+"\" onClick=\"obj.toggleVal('enableJavascript');this.className=(this.className=='bon')?'boff':'bon';\"></div><span style=\"font-size:12pt;\"></td><td>"+QObject::tr("Enable Javascript")+"</span></td></tr>\n"
						"<tr><td><div class=\""+enableJavaApplets+"\" onClick=\"obj.toggleVal('enableJavaApplets');this.className=(this.className=='bon')?'boff':'bon';\"></div><span style=\"font-size:12pt;\"></td><td>"+QObject::tr("Support for the Java applet tag")+"</span></td></tr>\n"
						"<tr><td><div class=\""+enableLocalStorage+"\" onClick=\"obj.toggleVal('enableLocalStorage');this.className=(this.className=='bon')?'boff':'bon';\"></div><span style=\"font-size:12pt;\"></td><td>"+QObject::tr("Local Storage Support")+"</span></td></tr>\n"
						"<tr><td><div class=\""+historySave+"\" onClick=\"obj.toggleVal('historySave');this.className=(this.className=='bon')?'boff':'bon';\"></div><span style=\"font-size:12pt;\"></td><td>"+QObject::tr("Saving history")+"</span></td></tr>\n"
						"</table>"


						"</div>\n";



		return content;
	}
	QString getAbout()
	{
		QString content="<script type=\"text/javascript\">var line=\"Developer: Dr.Smyrke [Smyrke2005@yandex.ru]<br>Engine: QT/Webkit<br>Language: C++<br>Application version: "
				+app::conf.version+"<br>\";var speed=100;var i=0;function init(){if(i++<line.length){document.getElementById(\"text\").innerHTML=line.substring(0,i);setTimeout(\'init()\',speed);}}</script>"
				"<div class=\"cbox\" onselectstart=\"return false\" onmousedown=\"return false\"><span id=\"text\" class=\"valgreen\" style=\"font-family:\'Courier New\',\'Terminus\',\'Monospace\'\"></span></div>"
				"<script type=\"text/javascript\">init();</script>";
		return content;
	}
	QString getBookmarks()
	{
		QString content;
//		if(app::getArrayBookmark("favorites").size()>0){
//			content+="<b>"+QObject::tr("Favorites")+"</b><br>\n";
			content+="<table class=\"bbox\">\n";
			for(auto elem:app::conf.bookmarks){
				content+="<tr><td><a href=\""+elem.first+"\" target=\"_blank\">"+elem.second+"</a></td><td>"
				"<a href=\"javascript:obj.bookmarkRemove('main','"+elem.first+"');\">["+QObject::tr("Remove")+"]</a><br>"
				"<a href=\"javascript:obj.bookmarkMoveTo('vip','"+elem.first+"');\">["+QObject::tr("Move to VIP")+"]</a><br>"
				"</td></tr>\n";
			}
			content+="</table>\n";
//		}
//		if(app::getArrayBookmark("other").size()>0){
//			content+="<b>"+QObject::tr("Others")+"</b><br>\n";
//			content+="<table class=\"bbox\">\n";
//			for(auto elem:app::getArrayBookmark("other")){
//				content+="<tr><td><a href=\""+elem.first+"\" target=\"_blank\">"+elem.second+"</a></td><td>"
//				//"<a href=\"#\">["+QObject::tr("Remove")+"]</a><br>"
//				//"<a href=\"javascript:obj.moveToFavor('unsort','"+elem.first+"');\">["+QObject::tr("Move to favorites")+"]</a><br>"
//				//"<a href=\"#\">["+QObject::tr("Move to others")+"]</a>"
//				"</td></tr>\n";
//			}
//			content+="</table>\n";
//		}
//		if(app::getArrayBookmark("unsort").size()>0){
//			content+="<b>"+QObject::tr("Unsorting")+"</b><br>\n";
//			content+="<table class=\"bbox\">\n";
//			for(auto elem:app::getArrayBookmark("unsort")){
//				content+="<tr><td><a href=\""+elem.first+"\" target=\"_blank\">"+elem.second+"</a></td><td>"
//				"<a href=\"#\">["+QObject::tr("Remove")+"]</a><br>"
//				"<a href=\"javascript:obj.bookmarkMoveTo('unsort','favorites','"+elem.first+"');\">["+QObject::tr("Move to favorites")+"]</a><br>"
//				"<a href=\"javascript:obj.bookmarkMoveTo('unsort','other','"+elem.first+"');\">["+QObject::tr("Move to others")+"]</a>"
//				"</td></tr>\n";
//			}
//			content+="</table>\n";
//		}
		content+="<div class=\"cbox\">\n"
				"<a href=\"javascript:button.importBookmarks();\">[Импорт закладок]</a>"
				"</div>\n";
		return content;
	}

	QString getBlank()
	{
		QString content="123";
		return content;
	}

}
