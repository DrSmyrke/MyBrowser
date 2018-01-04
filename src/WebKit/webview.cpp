#include "webview.h"

WebView::WebView(QWidget *parent) : QWebView(parent)
{
	m_pWebPage= new WebPage(this);
		m_pWebPage->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled,true);

		if(app::conf.web.enableJavascript){
			m_pWebPage->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);
		}else{
			m_pWebPage->settings()->setAttribute(QWebSettings::JavascriptEnabled,false);
		}

		if(app::conf.web.autoLoadImages){
			m_pWebPage->settings()->setAttribute(QWebSettings::AutoLoadImages,true);
		}else{
			m_pWebPage->settings()->setAttribute(QWebSettings::AutoLoadImages,false);
		}

		if(app::conf.web.enableJavaApplets){
			m_pWebPage->settings()->setAttribute(QWebSettings::JavaEnabled,true);
		}else{
			m_pWebPage->settings()->setAttribute(QWebSettings::JavaEnabled,false);
		}

		if(app::conf.web.enableLocalStorage){
			m_pWebPage->settings()->setAttribute(QWebSettings::LocalStorageEnabled,true);
		}else{
			m_pWebPage->settings()->setAttribute(QWebSettings::LocalStorageEnabled,false);
		}
		//m_pWebPage->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
		connect(m_pWebPage,&WebPage::linkHovered,this,&WebView::signal_linkHovered);
		connect(m_pWebPage,&WebPage::printRequested,this,&WebView::signal_printRequested);
		connect(m_pWebPage,&WebPage::downloadRequested,this,&WebView::slot_downloadRequested);
		setPage(m_pWebPage);
}
void WebView::slot_downloadRequested(const QNetworkRequest &request)
{
	qDebug()<<request.url();
	//request.
	//request.
}
QWebView *WebView::createWindow(QWebPage::WebWindowType type)
{
	Q_UNUSED(type);

	WebView* webView=new WebView();
	emit signal_createWindow(&webView);
	return webView;
}
void WebView::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu* menu=this->page()->createStandardContextMenu();
		menu->addSeparator();
		menu->addAction(tr("Hide inspector"),this,SIGNAL(signal_hideInspector()));

		//menu->addAction(tr("Show source"),this,[this](){
		//	m_pWebPage->setContentEditable(true);
		//});
		menu->addAction(QIcon("://images/about.png"),tr("About"),this,SIGNAL(signal_goToAbout()));
		//qDebug()<<this->page()->currentFrame()->hitTestContent(event->pos()).linkUrl();
	menu->exec(event->globalPos());
}
