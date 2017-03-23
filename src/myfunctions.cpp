#include "myfunctions.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>

namespace guiCfg {
	const QSize buttonSize(30,30);
}
namespace app{
	bool confEditState=false;
	const QString version="0.2";
	QString themePage;
	QString dataDir;
	QString confFile;
	QString bookmarksFile;
	QJsonObject confObj;
	QJsonObject bookmarksObj;

	QString getHtmlPage(const QString &title,const QString &content)
	{
		QString data=themePage;
		data.replace("===TITLE===",title);
		data.replace("===CONTENT===",content);
		return data;
	}
	void chkDirs()
	{
		confFile=dataDir+"/config";
		bookmarksFile=dataDir+"/bookmarks";
		if(!QFile::exists(dataDir)) QDir().mkdir(dataDir);
		if(!QFile::exists(dataDir+"/themes")) QDir().mkdir(dataDir+"/themes");
		if(!QFile::exists(dataDir+"/themes/default")) createDefaultTheme();
		loadConf();
		if(QFile::exists(bookmarksFile)) loadBookmarks();
		if(getVal("theme")=="default"){
			themePage=defaultThemeData;
		}else{
			themePage="";
			QFile file(dataDir+"/themes/"+getVal("theme"));
			if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
				while(!file.atEnd()) themePage+=file.readLine();
				file.close();
			}

		}
	}
	void removeConfig()
	{
		QFile::remove(confFile) ;
	}
	void saveConf()
	{
		if(!confEditState) return;
		QFile file(confFile);
		if (!file.open(QIODevice::WriteOnly)){
			QMessageBox::warning(nullptr,QObject::tr("Warning"),QObject::tr("Cannot save config file!"));
			return;
		}
		QJsonDocument saveDoc(confObj);
		file.write(saveDoc.toJson());
		file.close();
		confEditState=false;
	}
	void createConfig()
	{
		setVal("theme","default");
		setVal("switchToTheTab","0");
		setVal("autoLoadImages","1");
		setVal("enableLocalStorage","1");
		setVal("historySave","0");
		setVal("enableJavaApplets","0");
		setVal("enableJavascript","1");
		setVal("openBrowser","blank");
		setVal("homePage","http://drsmyrke-home.pskovline.ru");
		setVal("downloadPath",QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
		saveConf();
	}
	void loadConf()
	{
		if(!QFile::exists(confFile)) createConfig();
		QFile file(confFile);
		if (!file.open(QIODevice::ReadOnly)){
			QMessageBox::warning(nullptr,QObject::tr("Warning"),QObject::tr("Unable to open config file"));
			return;
		}
		auto confData = file.readAll();
		file.close();
		QJsonDocument loadDoc(QJsonDocument::fromJson(confData));
		confObj=loadDoc.object();
	}
	//установка в конфиг параметр=значение
	void setVal(const QString &param, const QString &val)
	{
		if(param.isEmpty() or val.isEmpty()) return;
		confObj[param]=val;
		confEditState=true;
	}
	QString getVal(const QString &param)
	{
		QString value="N/A";
		if(param.isEmpty()) return value;
		value=confObj[param].toString();
		return value;
	}
	//установка в конфиг массива параметр=значение
	void setValInArray(const QString &array,const QString &param, const QString &val)
	{
		QJsonObject inObj=confObj[array].toObject();
		inObj[param]=val;
		confObj[array]=inObj;
		confEditState=true;
	}
	QString getValInArray(const QString &array,const QString &param)
	{
		QJsonObject inObj=confObj[array].toObject();
		return inObj[param].toString();
	}
	std::map<QString,QString>* getArray(const QString &array)
	{
		static std::map<QString,QString> data;
		QJsonObject inObj=confObj[array].toObject();
		for(auto key:inObj.keys()){
			QString value=inObj[key].toString();
			data[key]=value;
		}
		return &data;
	}
	void importBookmarks(const QString &file)
	{
		if(!QFile::exists(confFile)) return;
		QFile f(file);
		if(!f.open(QIODevice::ReadOnly)){
			QMessageBox::warning(nullptr,QObject::tr("Warning"),QObject::tr("Unable to open file"));
			return;
		}
		auto data = QString(f.readAll());
		f.close();
		data.replace("\n","");
		data.replace("  "," ");
		data.replace("	","");
		if(data.contains("\"uri\":",Qt::CaseInsensitive)){	//JSON file
			QString title;
			QString uri;
			for(auto elem:data.split(",")){
				auto tmp=elem.split("\":\"");
				if(tmp[0].indexOf("{",Qt::CaseInsensitive)==0) tmp[0].remove(0,1);
				if(tmp[0].indexOf("\"",Qt::CaseInsensitive)==0) tmp[0].remove(0,1);
				if(tmp[0].contains("title",Qt::CaseInsensitive) and tmp.size()>1){
					if(tmp[1].indexOf("\"",Qt::CaseInsensitive)==tmp[1].length()-1) tmp[1].remove(tmp[1].length()-1,1);
					auto tmp2=tmp[1].split("\"}");
					title=tmp2[0];
				}
				if(tmp[0].contains("uri",Qt::CaseInsensitive) and tmp.size()>1){
					if(tmp[1].indexOf("\"",Qt::CaseInsensitive)==tmp[1].length()-1) tmp[1].remove(tmp[1].length()-1,1);
					auto tmp2=tmp[1].split("\"}");
					uri=tmp2[0];
					addUnsortBookmark(uri,title);
				}
			}
			saveBookmarks();
		}
	}
	void addUnsortBookmark(const QString &param, const QString &val)
	{
		if(param.isEmpty() or val.isEmpty()) return;
		QJsonObject inObj=bookmarksObj["unsort"].toObject();
		inObj[param]=val;
		bookmarksObj["unsort"]=inObj;
	}
	std::map<QString,QString>* getArrayBookmark(const QString &array)
	{
		static std::map<QString,QString> data;
		QJsonObject inObj=bookmarksObj[array].toObject();
		for(auto key:inObj.keys()){
			QString value=inObj[key].toString();
			data[key]=value;
		}
		return &data;
	}
	void loadBookmarks()
	{
		if(!QFile::exists(bookmarksFile)) return;
		QFile file(bookmarksFile);
		if (!file.open(QIODevice::ReadOnly)){
			QMessageBox::warning(nullptr,QObject::tr("Warning"),QObject::tr("Unable to open bookmarks file"));
			return;
		}
		auto data = file.readAll();
		file.close();
		QJsonDocument loadDoc(QJsonDocument::fromJson(data));
		bookmarksObj=loadDoc.object();
	}
	void saveBookmarks()
	{
		//if(!confEditState) return;
		QFile file(bookmarksFile);
		if (!file.open(QIODevice::WriteOnly)){
			QMessageBox::warning(nullptr,QObject::tr("Warning"),QObject::tr("Cannot save config file!"));
			return;
		}
		QJsonDocument saveDoc(bookmarksObj);
		file.write(saveDoc.toJson());
		file.close();
		//confEditState=false;
	}
	void createDefaultTheme(){
		QFile file(dataDir+"/themes/default");
		if(file.open(QIODevice::WriteOnly)){
			file.write(defaultThemeData.toStdString().c_str());
			file.close();
		}
	}
	const QString defaultThemeData="<!DOCTYPE html><html lang=\"ru\"><head>\n<meta charset=\"utf-8\"/>\n<META http-equiv=\"Pragma\" content=\"no-cache\">\n"
								   "<style>\n"
								   "	body{\n"
								   "		font-family:Arial,Sans,Microsoft Sans Serif,consolas;\n"
								   "		font-size:10pt;\n"
								   "		text-shadow: 0px 1px 0px black;\n"
								   "		color:white;\n"
								   "		background-image:url(data:image/gif;base64,R0lGODlhPABCANUAACssLiYnKSMlJiUmJyUnKCEiJCAhIiQlJi8xMyIkJSgpKx8gIT9BQzM1Nx4fITU3OTs9PyIjJSEhIygqLD0/QiAhIx4fICcoKiosLUBCRCgqKzAyNC0uLyEjJC8wMjo7Pi0uMDk7PSkqLAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAAAAAAALAAAAAA8AEIAAAb/QIBwKDwIBoGkcqlhEJ9QI5LJdEKvAKoWAcFetVSu9zsFhx7jaFl7vloMB4Ua3KAgiMY3XP4k+Ld2eAZ6Dh18fWEZDUMEg4ULj4d4iYtFjpASFZAYTwh3F6AbH4GWmAWnmgacRJ5ZoaOVWaaoCagenQ8QDLsPn7JwtUfCcLesubsQvYyZtGuqvmm/ws7P0csR00vCaNbSf9oFAdzW2NngCV3dzdrf6eTB7EtW72vx89Hh9ez3aVLx8t1c6Vsipts/bQXpHVTQJuBCJA0NDvRTB1oePZKKfENIytWlOHPYVbz2KBIZJiNLlSyUEcSEbwBEdWw0i5mqTtBkxqKZitZN/1afkCkbYjObLVy6eFkEZpQYUiz+okqpZpCpPqoKN/4JNw5fPq1IuAZc98+dV6ngzPYzZ28s27QO31aJO3EKvzEPkSSMllfAXr51GXYFvDAiEYwhRc68uCcxx6WEDJ0kqIjkrJYvFe/8aHPVkFagYsKy7PPn55yjVRY9aixpMsjwhvlFKhQ2W6x8rVLjAG1tuavi3MrdJrwuuuIHj6uLXRb5wbtevi6EDnW4XbrJ/+K17lft9sCGCR8M7/tfSo+IHVOGnH5SvPM8L09WAv+SKcwwdZL26VkI6Fcz1dQZTlnUtp9ss7VmIFG6yVbMZ7lU99tuvX03HG/OsSPWctwNNtMGWed4Fx1zIWaoIXVucKdcVs9hN52L5okoYXYyzliWhxbemCJIiMTIXmM9vreYfZKpV99KLM1HUWWq8UdgaPo16SRQrogWoICsQejaUN40laCWC3bZFIZVTegMmSzWs2Ga5+Bo43gmwsUhWADNmRyK6rUlkXFNwCikn4oBygR5OcZDqBcPHdmee0L+yCOjKDH5C5L4abZfkiFZuRmWpvmH2pU9DUjlBWEqsBqCD/p3jFIHOvjUFw2WgVt5aGGD5lngxblVcHbC2auGK+J6p651PhEEADs=);\n"
								   "		background-attachment:fixed;\n"
								   "	}\n"
								   "	a{\n"
								   "		outline:none;-webkit-transition-property:color;-webkit-transition-duration:1s;transition-property:color;\n"
								   "		transition-duration:1s;color:#999999;text-decoration:none;font-size:10pt;\n"
								   "	}\n"
								   "	input[type=text]{\n"
								   "		padding:5px;\n"
								   "	}\n"
								   "	a:active{position:relative;top:1px;left:1px;}\n"
								   "	b,.b{text-shadow:0px 0px 5px #FFCF7D;}\n"
								   "	a:hover{color:#ff9900;text-shadow: 0 0 3px #FFCC80;}\n"
								   "	.valgreen{text-shadow:0px 0px 5px #61FF62;color:#00FF03;}\n"
								   "	.valorange{text-shadow:0px 0px 5px #FFB67B;color:#FF7200;}\n"
								   "	.valyellow{text-shadow:0px 0px 5px #EEEE73;color:#F3F200;}\n"
								   "	.valfiol{text-shadow:0px 0px 5px #D197FF;color:#AD6BE1;}\n"
								   "	.valred{text-shadow:0px 0px 5px #FF6E6B;color:#FF0600;}\n"
								   "	.cbox{width:90%;margin:auto;margin-top:10px;border-top:1px solid silver;border-collapse:collapse;padding-top:5px;}\n"
								   "	.bbox{width:80%;margin:auto;margin-top:10px;border-top:1px solid silver;border-collapse:collapse;}\n"
								   "	.bbox tr:nth-child(even){background:rgba(255,255,255,0.05);}\n"
								   "	.downloadItem{background:rgba(255,255,255,0.05);border:1px solid silver;border-radius:5px;padding:5px;margin-bottom:5px;}\n"
								   "	.komment{font-size:8pt;color:silver;}\n"
								   "	.val{font-size:7pt;width:120px;}\n"
								   "	.boff{cursor:pointer;width:50px;height:25px;background-image:url(data:image/gif;base64,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);}\n"
								   "	.bon{cursor:pointer;width:50px;height:25px;background-image:url(data:image/gif;base64,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);}\n"
								   "</style>\n"
								   "<title>===TITLE===</title></head><body>\n"
								   "<center><h2><b>Welcome to MyBrowser by Dr.Smyrke</b></h2></center>\n"
								   "===CONTENT===\n"
								   "<br><center style=\"color:gray;\">Made by <a href=\"http://drsmyrke-home.pskovline.ru/my\" target=\"_blank\">Dr.Smyrke</a></center>\n"
								   "</body></html>";
}
