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
	auto tmp=url.split("://");
	QString proto;
	QString addr=url;
	if(tmp.size()>1){
		proto=tmp[0];
		addr.remove(0,proto.length()+3);
	}
	if(proto.contains("http",Qt::CaseInsensitive) or proto.contains("https",Qt::CaseInsensitive)){
		m_pWebView->load(QUrl(url));
		return;
	}
	proto=proto.toLower();
	if(proto==app::urlPreff){
		if(addr=="bookmarks"){
			JavaScriptObj* obj=new JavaScriptObj();
			m_pWebView->page()->mainFrame()->addToJavaScriptWindowObject("button",obj);
			QString content="<div class=\"cbox\">\n"
					"<a href=\"javascript:button.openTest('http://ya.ru');\">[Импорт закладок]</a>"
					"</div>\n";
			m_pWebView->setHtml(app::getHtmlPage(tr("BOOKMARKS"),content),QUrl(url));
			slot_titleChanged(tr("BOOKMARKS"));
		}
		if(addr=="about"){
			QString content="<script type=\"text/javascript\">var line=\"Developer: Dr.Smyrke [Smyrke2005@yandex.ru]<br>Engine: QT/Webkit<br>Language: C++<br>Application version: "+app::appVersion+"<br>\";var speed=100;var i=0;function init(){if(i++<line.length){document.getElementById(\"text\").innerHTML=line.substring(0,i);setTimeout(\'init()\',speed);}}</script><div class=\"cbox\"><span id=\"text\" class=\"valgreen\" style=\"font-family:\'Courier New\',\'Terminus\',\'Monospace\'\"></span></div><script type=\"text/javascript\">init();</script>";
			m_pWebView->setHtml(app::getHtmlPage(tr("ABOUT"),content),QUrl(url));
			slot_titleChanged(tr("ABOUT"));
		}
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
	qDebug()<<url;
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
