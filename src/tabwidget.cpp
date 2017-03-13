#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent) : QWidget(parent)
{
	m_pWebView=new WebView();
	m_pProgressBar=new QProgressBar();
		m_pProgressBar->setMaximumSize(100,16);
	m_pPrefB=new QPushButton();
		m_pPrefB->setIcon(QIcon("://images/go-previous.svg"));
		m_pPrefB->setMaximumSize(gui::buttonSize);
		m_pPrefB->hide();
		connect(m_pPrefB,&QPushButton::clicked,m_pWebView,&WebView::back);
	m_pNextB=new QPushButton();
		m_pNextB->setIcon(QIcon("://images/go-next.svg"));
		m_pNextB->setMaximumSize(gui::buttonSize);
		m_pNextB->hide();
		connect(m_pNextB,&QPushButton::clicked,m_pWebView,&WebView::forward);
	m_pUrlField=new QLineEdit();
		connect(m_pUrlField,&QLineEdit::returnPressed,this,&TabWidget::slot_goToUrl);
	QPushButton* goB=new QPushButton();
		goB->setIcon(QIcon("://images/dialog-apply.svg"));
		goB->setMaximumSize(gui::buttonSize);
		connect(goB,&QPushButton::clicked,this,&TabWidget::slot_goToUrl);
	m_pReloadB=new QPushButton();
			m_pReloadB->setIcon(QIcon("://images/view-refresh.svg"));
			m_pReloadB->setMaximumSize(gui::buttonSize);
			connect(m_pReloadB,&QPushButton::clicked,m_pWebView,&WebView::reload);
	m_pStopB=new QPushButton();
			m_pStopB->setIcon(QIcon("://images/window-close.svg"));
			m_pStopB->setMaximumSize(gui::buttonSize);
			connect(m_pStopB,&QPushButton::clicked,m_pWebView,&WebView::stop);
	QPushButton* goHomeB=new QPushButton();
			goHomeB->setIcon(QIcon("://images/go-home.svg"));
			goHomeB->setMaximumSize(gui::buttonSize);
	m_pStatusLabel=new QLabel("123");


	QGridLayout* box=new QGridLayout(this);
	box->setContentsMargins(0,0,0,0);
		QHBoxLayout* boxTop=new QHBoxLayout();
			boxTop->addWidget(m_pPrefB);
			boxTop->addWidget(m_pNextB);
			boxTop->addWidget(m_pUrlField);
			boxTop->addWidget(goB);
			boxTop->addWidget(m_pReloadB);
			boxTop->addWidget(m_pStopB);
			boxTop->addWidget(goHomeB);
		box->addLayout(boxTop,0,0);


		box->addWidget(m_pWebView,1,0,1,3);

		QHBoxLayout* boxBottom=new QHBoxLayout();
			boxBottom->addWidget(m_pProgressBar);
			boxBottom->addWidget(m_pStatusLabel);
		box->addLayout(boxBottom,2,0);
	this->setLayout(box);
	connect(m_pWebView,&WebView::titleChanged,this,&TabWidget::slot_titleChanged);
	connect(m_pWebView,&WebView::loadFinished,this,&TabWidget::slot_loadFinished);
	connect(m_pWebView,&WebView::urlChanged,this,&TabWidget::slot_loadFinished);
	connect(m_pWebView,&WebView::loadProgress,this,&TabWidget::slot_loadProgress);
	connect(m_pWebView,&WebView::linkClicked,this,&TabWidget::slot_linkClicked);
}


void TabWidget::actionUrl(const QString &url)
{
	if(url=="://bookmarks"){
		qDebug()<<url;
	}
	auto tmp=url.split("//:");
	QString proto;
//	QString addr=url;
	if(tmp.size()>1){
		proto=tmp[0];
//		addr.remove(0,proto.length());
	}
	if(proto.contains("http",Qt::CaseInsensitive)){
		m_pWebView->load(QUrl(url));
		return;
	}
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
	emit signal_loadFinished(this);
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
