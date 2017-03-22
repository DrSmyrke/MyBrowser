#include "webview.h"

WebView::WebView(QWidget *parent) : QWebView(parent)
{
	m_pWebPage= new WebPage(this);
		m_pWebPage->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled,true);

		if(app::getVal("enableJavascript")=="1" or app::getVal("enableJavascript")=="true"){
			m_pWebPage->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);
		}else{
			m_pWebPage->settings()->setAttribute(QWebSettings::JavascriptEnabled,false);
		}

		if(app::getVal("autoLoadImages")=="1" or app::getVal("autoLoadImages")=="true"){
			m_pWebPage->settings()->setAttribute(QWebSettings::AutoLoadImages,true);
		}else{
			m_pWebPage->settings()->setAttribute(QWebSettings::AutoLoadImages,false);
		}

		if(app::getVal("enableJavaApplets")=="1" or app::getVal("enableJavaApplets")=="true"){
			m_pWebPage->settings()->setAttribute(QWebSettings::JavaEnabled,true);
		}else{
			m_pWebPage->settings()->setAttribute(QWebSettings::JavaEnabled,false);
		}

		if(app::getVal("enableLocalStorage")=="1" or app::getVal("enableLocalStorage")=="true"){
			m_pWebPage->settings()->setAttribute(QWebSettings::LocalStorageEnabled,true);
		}else{
			m_pWebPage->settings()->setAttribute(QWebSettings::LocalStorageEnabled,false);
		}
		//m_pWebPage->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
		connect(m_pWebPage,&WebPage::linkHovered,this,&WebView::signal_linkHovered);
		connect(m_pWebPage,&WebPage::printRequested,this,&WebView::signal_printRequested);
	setPage(m_pWebPage);
}
QWebView *WebView::createWindow(QWebPage::WebWindowType type)
{
	Q_UNUSED(type);

	WebView* webView=new WebView();
	emit signal_createWindow(&webView);
	return webView;
}
