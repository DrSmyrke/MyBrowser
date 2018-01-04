#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent) : QWidget(parent)
{
	m_pWebView=new WebView(this);
		connect(m_pWebView,&WebView::signal_linkHovered,this,&TabWidget::slot_linkHovered);
		connect(m_pWebView,&WebView::titleChanged,this,&TabWidget::slot_titleChanged);
		connect(m_pWebView,&WebView::loadFinished,this,&TabWidget::slot_loadFinished);
		connect(m_pWebView,&WebView::urlChanged,this,&TabWidget::slot_urlChanged);
		connect(m_pWebView,&WebView::loadProgress,this,&TabWidget::slot_loadProgress);
		connect(m_pWebView,&WebView::linkClicked,this,&TabWidget::slot_linkClicked);
		connect(m_pWebView,&WebView::signal_createWindow,this,&TabWidget::signal_createWindow);
		connect(m_pWebView,&WebView::signal_printRequested,this,&TabWidget::signal_printRequested);
		connect(m_pWebView,&WebView::signal_goToAbout,this,[this](){actionUrl("about:me");});
		connect(m_pWebView,&WebView::signal_hideInspector,this,[this](){
			if(m_pInspector->isVisible()) m_pInspector->hide();
		});
		connect(m_pWebView->page()->mainFrame(),&QWebFrame::javaScriptWindowObjectCleared,this,&TabWidget::slot_addObj);


	m_pProgressBar=new QProgressBar();
		m_pProgressBar->setMaximumSize(150,16);
	m_pPrefB=new QPushButton();
		m_pPrefB->setIcon(QIcon("://images/previous.png"));
		m_pPrefB->setMaximumSize(app::conf.guiCfg.buttonSize);
		m_pPrefB->setFlat(true);
		m_pPrefB->hide();
		connect(m_pPrefB,&QPushButton::clicked,m_pWebView,&WebView::back);
	m_pNextB=new QPushButton();
		m_pNextB->setIcon(QIcon("://images/next.png"));
		m_pNextB->setMaximumSize(app::conf.guiCfg.buttonSize);
		m_pNextB->setFlat(true);
		m_pNextB->hide();
		connect(m_pNextB,&QPushButton::clicked,m_pWebView,&WebView::forward);
	m_pUrlField=new QLineEdit();
		m_pUrlField->setPlaceholderText(tr("url page"));
		connect(m_pUrlField,&QLineEdit::returnPressed,this,&TabWidget::slot_goToUrl);
	QPushButton* goB=new QPushButton();
		goB->setIcon(QIcon("://images/apply.png"));
		goB->setMaximumSize(app::conf.guiCfg.buttonSize);
		goB->setFlat(true);
		connect(goB,&QPushButton::clicked,this,&TabWidget::slot_goToUrl);
	m_pReloadB=new QPushButton();
		m_pReloadB->setIcon(QIcon("://images/refresh.png"));
		m_pReloadB->setMaximumSize(app::conf.guiCfg.buttonSize);
		m_pReloadB->setFlat(true);
		//connect(m_pReloadB,&QPushButton::clicked,m_pWebView,&WebView::reload);
		connect(m_pReloadB,&QPushButton::clicked,this,[this](){
			m_pWebView->stop();
			actionUrl(m_pWebView->url().toString());
		});
	m_pStopB=new QPushButton();
		m_pStopB->setIcon(QIcon("://images/close.png"));
		m_pStopB->setMaximumSize(app::conf.guiCfg.buttonSize);
		m_pStopB->setFlat(true);
		connect(m_pStopB,&QPushButton::clicked,m_pWebView,&WebView::stop);
	QPushButton* goHomeB=new QPushButton();
		goHomeB->setIcon(QIcon("://images/home.png"));
		goHomeB->setMaximumSize(app::conf.guiCfg.buttonSize);
		goHomeB->setFlat(true);
		connect(goHomeB,&QPushButton::clicked,this,[this](){m_pWebView->load(app::conf.homePage);});
	QPushButton* goFindB=new QPushButton();
		goFindB->setIcon(QIcon("://images/find.png"));
		goFindB->setMaximumSize(app::conf.guiCfg.buttonSize);
		goFindB->setFlat(true);
		goFindB->setShortcut(QKeySequence::Find);
		connect(goFindB,&QPushButton::clicked,this,[this](){
			if(!m_pFindBox->isVisible()) m_pFindBox->show();
			m_pTextFind->clear();
			m_pTextFind->setFocus();
			m_pFindAll->setChecked(false);
		});
	QPushButton* addBookmarksB=new QPushButton();
		addBookmarksB->setIcon(QIcon("://images/bookmark-add.png"));
		addBookmarksB->setMaximumSize(app::conf.guiCfg.buttonSize);
		addBookmarksB->setFlat(true);
		//addBookmarksB->setShortcut(QKeySequence::Find);
		connect(addBookmarksB,&QPushButton::clicked,this,[this](){
			app::conf.bookmarks[ m_pWebView->url().toString() ] = m_pWebView->title();
		});
	//find panel
	m_pFindBox=new QWidget();
		m_pFindBox->hide();
		QHBoxLayout* findBoxLayout=new QHBoxLayout();
			findBoxLayout->setSpacing(1);
			findBoxLayout->setContentsMargins(0,0,0,0);
				m_pTextFind=new QLineEdit(this);
					m_pTextFind->setPlaceholderText(tr("Search by page"));
					connect(m_pTextFind,&QLineEdit::textChanged,this,&TabWidget::slot_fintText);
			findBoxLayout->addWidget(m_pTextFind);
				QPushButton* findBackB=new QPushButton();
					findBackB->setIcon(QIcon("://images/previous.png"));
					findBackB->setMaximumSize(app::conf.guiCfg.buttonSize);
					findBackB->setShortcut(QKeySequence::FindPrevious);
					connect(findBackB,&QPushButton::clicked,this,[this](){
						m_pWebView->findText(m_pTextFind->text(),QWebPage::FindBackward);
					});
			findBoxLayout->addWidget(findBackB);
				QPushButton* findNextB=new QPushButton();
					findNextB->setIcon(QIcon("://images/next.png"));
					findNextB->setMaximumSize(app::conf.guiCfg.buttonSize);
					findNextB->setShortcut(QKeySequence::FindNext);
					connect(findNextB,&QPushButton::clicked,this,[this](){
						m_pWebView->findText(m_pTextFind->text());
					});
			findBoxLayout->addWidget(findNextB);
				m_pFindAll=new QCheckBox(tr("Select all found items"));
				connect(m_pFindAll,&QCheckBox::stateChanged,this,[this](int state){
					if(state==Qt::Checked){
						m_pWebView->findText(m_pTextFind->text(),QWebPage::HighlightAllOccurrences);
					}else{
						m_pWebView->findText("",QWebPage::HighlightAllOccurrences);
					}
				});
			findBoxLayout->addWidget(m_pFindAll);
				QPushButton* findCloseB=new QPushButton();
					findCloseB->setIcon(QIcon("://images/close.png"));
					findCloseB->setMaximumSize(app::conf.guiCfg.buttonSize);
					connect(findCloseB,&QPushButton::clicked,m_pFindBox,&QWidget::hide);
			findBoxLayout->addWidget(findCloseB);
	m_pFindBox->setLayout(findBoxLayout);

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
			boxTop->addWidget(goFindB);
			boxTop->addWidget(addBookmarksB);
		box->addLayout(boxTop);
		box->addWidget(m_pFindBox);


		box->addWidget(splitter);
		QHBoxLayout* boxBottom=new QHBoxLayout();
			boxBottom->addWidget(m_pProgressBar);
			boxBottom->addWidget(m_pStatusLabel);
		box->addLayout(boxBottom);
	this->setLayout(box);
}





void TabWidget::actionUrl(const QString &url)
{
	QString addr=url;
	if(url.isEmpty()) addr="about:blank";
	if(addr.toLower()=="about:blank"){
		JavaScriptObj* javaScriptObj=new JavaScriptObj();
		m_pWebView->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);
		m_pWebView->page()->settings()->setAttribute(QWebSettings::AutoLoadImages,true);
		m_pWebView->page()->mainFrame()->addToJavaScriptWindowObject("obj",javaScriptObj);
		m_pWebView->setHtml(app::getHtmlPage(tr("BLANK"),gp::getBlank()),QUrl(url));
		slot_titleChanged(tr("BLANK"));
		return;
	}
	if(addr.toLower()=="about:settings"){
		JavaScriptObj* javaScriptObj=new JavaScriptObj();
		m_pWebView->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);
		m_pWebView->page()->settings()->setAttribute(QWebSettings::AutoLoadImages,true);
		m_pWebView->page()->mainFrame()->addToJavaScriptWindowObject("obj",javaScriptObj);
		m_pWebView->setHtml(app::getHtmlPage(tr("SETTINGS"),gp::getSettings()),QUrl(url));
		slot_titleChanged(tr("SETTINGS"));
		return;
	}
	if(addr.toLower()=="about:bookmarks"){
		m_pWebView->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);
		m_pWebView->page()->settings()->setAttribute(QWebSettings::AutoLoadImages,true);
		m_pWebView->setHtml(app::getHtmlPage(tr("BOOKMARKS"),gp::getBookmarks()),QUrl(url));
		slot_titleChanged(tr("BOOKMARKS"));
		return;
	}
	if(addr.toLower()=="about:me"){
		m_pWebView->page()->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);
		m_pWebView->page()->settings()->setAttribute(QWebSettings::AutoLoadImages,true);
		m_pWebView->setHtml(app::getHtmlPage(tr("ABOUT"),gp::getAbout()),QUrl(url));
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
	if(proto.isEmpty())	m_pWebView->load(QUrl(QString("http://"+url)));
}

void TabWidget::slot_linkHovered(const QString &link)
{
	QFont font;
	QFontMetrics metrix(font);
	m_pStatusLabel->setText(metrix.elidedText(" "+link,Qt::ElideMiddle,m_pStatusLabel->geometry().width()));
}
void TabWidget::slot_fintText(const QString &text)
{
	if(m_pFindAll->checkState()==Qt::Checked){
		m_pWebView->findText("",QWebPage::HighlightAllOccurrences);
		m_pFindAll->setChecked(false);
	}
	m_pWebView->findText(text);
}
void TabWidget::slot_addObj()
{
	if(m_pWebView->url().toString()=="about:bookmarks"){
		JavaScriptObj* javaScriptObj=new JavaScriptObj();
			connect(javaScriptObj,&JavaScriptObj::signal_reload,this,[this](){
				actionUrl(m_pWebView->url().toString());
			});
		m_pWebView->page()->mainFrame()->addToJavaScriptWindowObject("obj",javaScriptObj);
	}
	if(m_pWebView->url().toString()=="about:settings"){
		JavaScriptObj* javaScriptObj=new JavaScriptObj();
			connect(javaScriptObj,&JavaScriptObj::signal_reload,this,[this](){
				actionUrl(m_pWebView->url().toString());
			});
		m_pWebView->page()->mainFrame()->addToJavaScriptWindowObject("obj",javaScriptObj);
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
	if(!m_pWebView->title().isEmpty()) slot_titleChanged(m_pWebView->title());
	m_pUrlField->setText(m_pWebView->url().toString());

	if(m_pPrefB->isVisible()) m_pPrefB->hide();
	if(m_pNextB->isVisible()) m_pNextB->hide();
	if(m_pWebView->history()->canGoBack()) m_pPrefB->show();
	if(m_pWebView->history()->canGoForward()) m_pNextB->show();

	if(app::conf.historySave){
		if(!QFile::exists(app::conf.dataDir+"/history")) QDir().mkdir(app::conf.dataDir+"/history");
		if(QFile::exists(app::conf.dataDir+"/history")){
			auto currentDate = QDate::currentDate().toString("yyyy/MM");
			auto currentDay = QDate::currentDate().toString("dd");
			auto time=QTime::currentTime().toString("hh:mm");
			if(!QFile::exists(app::conf.dataDir+"/history/"+currentDate)) QDir().mkpath(app::conf.dataDir+"/history/"+currentDate);
			if(QFile::exists(app::conf.dataDir+"/history/"+currentDate)){
				QFile file(app::conf.dataDir+"/history/"+currentDate+"/"+currentDay+".log");
				if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
					file.write(QString(time+"	"+m_pWebView->title()+"	"+m_pWebView->url().toString()+"\n").toStdString().c_str());
					file.close();
				}
			}
		}
	}
}
void TabWidget::slot_loadProgress(int prz)
{
	m_pProgressBar->setValue(prz);
	if(prz < 100){
		m_pProgressBar->setFormat(QString::number(prz)+"% [" + mf::getSize(m_pWebView->page()->bytesReceived()) + "/" + mf::getSize(m_pWebView->page()->totalBytes()) + "]");
	}else{
		m_pProgressBar->setFormat(QString::number(prz)+"% [" + mf::getSize(m_pWebView->page()->totalBytes()) + "]");
	}
}
void TabWidget::slot_urlChanged(const QUrl &url)
{
	m_pUrlField->setText(url.toString());
}
