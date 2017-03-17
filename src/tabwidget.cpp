#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent) : QWidget(parent)
{
	m_pWebView=new WebView(this);
		connect(m_pWebView,&WebView::signal_linkHovered,this,&TabWidget::slot_linkHovered);
		connect(m_pWebView,&WebView::titleChanged,this,&TabWidget::slot_titleChanged);
		connect(m_pWebView,&WebView::loadFinished,this,&TabWidget::slot_loadFinished);
		connect(m_pWebView,&WebView::urlChanged,this,&TabWidget::slot_loadFinished);
		connect(m_pWebView,&WebView::loadProgress,this,&TabWidget::slot_loadProgress);
		connect(m_pWebView,&WebView::linkClicked,this,&TabWidget::slot_linkClicked);
		connect(m_pWebView,&WebView::signal_createWindow,this,&TabWidget::signal_createWindow);

	m_pProgressBar=new QProgressBar();
		m_pProgressBar->setMaximumSize(100,16);
	m_pPrefB=new QPushButton();
		m_pPrefB->setIcon(QIcon("://images/go-previous.svg"));
		m_pPrefB->setMaximumSize(guiCfg::buttonSize);
		m_pPrefB->hide();
		connect(m_pPrefB,&QPushButton::clicked,m_pWebView,&WebView::back);
	m_pNextB=new QPushButton();
		m_pNextB->setIcon(QIcon("://images/go-next.svg"));
		m_pNextB->setMaximumSize(guiCfg::buttonSize);
		m_pNextB->hide();
		connect(m_pNextB,&QPushButton::clicked,m_pWebView,&WebView::forward);
	m_pUrlField=new QLineEdit();
		m_pUrlField->setPlaceholderText(tr("url page"));
		connect(m_pUrlField,&QLineEdit::returnPressed,this,&TabWidget::slot_goToUrl);
	QPushButton* goB=new QPushButton();
		goB->setIcon(QIcon("://images/dialog-apply.svg"));
		goB->setMaximumSize(guiCfg::buttonSize);
		connect(goB,&QPushButton::clicked,this,&TabWidget::slot_goToUrl);
	m_pReloadB=new QPushButton();
			m_pReloadB->setIcon(QIcon("://images/view-refresh.svg"));
			m_pReloadB->setMaximumSize(guiCfg::buttonSize);
			connect(m_pReloadB,&QPushButton::clicked,m_pWebView,&WebView::reload);
	m_pStopB=new QPushButton();
			m_pStopB->setIcon(QIcon("://images/window-close.svg"));
			m_pStopB->setMaximumSize(guiCfg::buttonSize);
			connect(m_pStopB,&QPushButton::clicked,m_pWebView,&WebView::stop);
	QPushButton* goHomeB=new QPushButton();
			goHomeB->setIcon(QIcon("://images/go-home.svg"));
			goHomeB->setMaximumSize(guiCfg::buttonSize);
	m_pStatusLabel=new QLabel();

	m_pInspector=new QWebInspector(this);
		m_pInspector->setPage(m_pWebView->page());
		m_pInspector->hide();
	QSplitter* splitter=new QSplitter(Qt::Vertical,this);
		splitter->addWidget(m_pWebView);
		splitter->addWidget(m_pInspector);

	QVBoxLayout* box=new QVBoxLayout();
	box->setSpacing(1);
	box->setContentsMargins(0,0,0,0);
		QHBoxLayout* boxTop=new QHBoxLayout();
			boxTop->addWidget(m_pPrefB);
			boxTop->addWidget(m_pNextB);
			boxTop->addWidget(m_pUrlField);
			boxTop->addWidget(goB);
			boxTop->addWidget(m_pReloadB);
			boxTop->addWidget(m_pStopB);
			boxTop->addWidget(goHomeB);
		box->addLayout(boxTop);


		box->addWidget(splitter);
		QHBoxLayout* boxBottom=new QHBoxLayout();
			boxBottom->addWidget(m_pProgressBar);
			boxBottom->addWidget(m_pStatusLabel);
		box->addLayout(boxBottom);
	this->setLayout(box);
}





void TabWidget::actionUrl(const QString &url)
{
	QString SPACE="&#160;&#160;&#160;";
	QString addr=url;
	if(url.isEmpty()) addr="about:blank";
	if(addr.toLower()=="about:settings"){
		JavaScriptObj* obj=new JavaScriptObj();
		m_pWebView->page()->mainFrame()->addToJavaScriptWindowObject("settings",obj);
		std::map<QString,QString> atStarting;
			atStarting[tr("Show home page")]="homePage";
			atStarting[tr("Show blank page")]="blank";
			atStarting[tr("Show tabs opened last time")]="lastTime";
		QString atStartingVals;
		for(auto elem:atStarting){
			QString selected=(app::getVal("openBrowser")==elem.second)?" selected":"";
			atStartingVals+="<option label=\""+elem.first+"\" value=\""+elem.second+"\""+selected+">"+elem.first+"</option>";
		}
		QString switchToTheTab=(app::getVal("switchToTheTab")=="1" or app::getVal("switchToTheTab")=="true")?"bon":"boff";
		QString content="<div class=\"cbox\">\n"
				"<b>"+tr("Starting")+"</b><br>\n"
				+SPACE+tr("At startup:")+" <select onChange=\"settings.setVal('openBrowser',this.value);\">"+atStartingVals+"</select><br>\n"
				+SPACE+tr("Home page:")+" <input type=\"text\" size=\"30\" onChange=\"settings.setVal('homePage',this.value);\" onKeyUp=\"settings.setVal('homePage',this.value);\" value=\""+app::getVal("homePage")+"\"><br>\n"
				"<b>"+tr("Downloads")+"</b><br>\n"
				+SPACE+tr("The path to save files:")+" <input type=\"text\" size=\"30\" readonly value=\""+app::getVal("downloadPath")+"\"> <input type=\"button\" value=\""+tr("Change")+"\" onClick=\"settings.changeDownloadPath();\"><br>\n"
				"<b>"+tr("Tabs")+"</b><br>\n"
				+SPACE+"<div class=\""+switchToTheTab+"\" onClick=\"settings.toggleVal('switchToTheTab');this.className=(this.className='bon')?'boff':'bon';\"></div><span style=\"font-size:12pt;\">"+tr("Switch to the tab that opens")+"</span><br>\n"

				"</div>\n";
		m_pWebView->setHtml(app::getHtmlPage(tr("SETTINGS"),content),QUrl(url));
		slot_titleChanged(tr("SETTINGS"));
		return;
	}
	if(addr.toLower()=="about:bookmarks"){
		JavaScriptObj* obj=new JavaScriptObj();
		m_pWebView->page()->mainFrame()->addToJavaScriptWindowObject("button",obj);
		QString content="<div class=\"cbox\">\n"
				"<a href=\"javascript:button.importBookmarks();\">[Импорт закладок]</a>"
				"</div>\n";
		m_pWebView->setHtml(app::getHtmlPage(tr("BOOKMARKS"),content),QUrl(url));
		slot_titleChanged(tr("BOOKMARKS"));
		return;
	}
	if(addr.toLower()=="about:me"){
		QString content="<script type=\"text/javascript\">var line=\"Developer: Dr.Smyrke [Smyrke2005@yandex.ru]<br>Engine: QT/Webkit<br>Language: C++<br>Application version: "+app::appVersion+"<br>\";var speed=100;var i=0;function init(){if(i++<line.length){document.getElementById(\"text\").innerHTML=line.substring(0,i);setTimeout(\'init()\',speed);}}</script><div class=\"cbox\"><span id=\"text\" class=\"valgreen\" style=\"font-family:\'Courier New\',\'Terminus\',\'Monospace\'\"></span></div><script type=\"text/javascript\">init();</script>";
		m_pWebView->setHtml(app::getHtmlPage(tr("ABOUT"),content),QUrl(url));
		slot_titleChanged(tr("ABOUT"));
		return;
	}
	auto tmp=url.split("://");
	QString proto;
	if(tmp.size()>1){
		proto=tmp[0];
		addr.remove(0,proto.length()+3);
	}
	if(proto.contains("http",Qt::CaseInsensitive) or proto.contains("https",Qt::CaseInsensitive)){
		m_pWebView->load(QUrl(url));
	}
}

void TabWidget::slot_linkHovered(const QString &link)
{
	QFont font;
	QFontMetrics metrix(font);
	m_pStatusLabel->setText(metrix.elidedText(link,Qt::ElideMiddle,m_pStatusLabel->geometry().width()));
}
void TabWidget::slot_goToUrl()
{
	actionUrl(m_pUrlField->text());
}
void TabWidget::load(const QUrl &url)
{
	m_pWebView->load(url);
}
void TabWidget::slot_linkClicked(const QUrl &url)
{
	m_pWebView->load(url);
}
void TabWidget::slot_titleChanged(const QString &title)
{
	if(title.isEmpty()) return;
	emit signal_titleChanged(this,title);
}
void TabWidget::slot_loadFinished()
{
	if(!m_pWebView->title().isEmpty()) slot_titleChanged(m_pWebView->title());
	m_pUrlField->setText(m_pWebView->url().toString());

	if(m_pPrefB->isVisible()) m_pPrefB->hide();
	if(m_pNextB->isVisible()) m_pNextB->hide();
	if(m_pWebView->history()->canGoBack()) m_pPrefB->show();
	if(m_pWebView->history()->canGoForward()) m_pNextB->show();
}
void TabWidget::slot_loadProgress(int prz)
{
	m_pProgressBar->setValue(prz);
}
void TabWidget::slot_urlChanged(const QUrl &url)
{
	m_pUrlField->setText(m_pWebView->url().toString());
}
void TabWidget::reload()
{
	m_pWebView->reload();
}
