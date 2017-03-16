#include "webview.h"

WebView::WebView(QWidget *parent) : QWebView(parent)
{
	m_pWebPage= new WebPage(this);
		m_pWebPage->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);
		m_pWebPage->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled,true);
		//m_pWebPage->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
		connect(m_pWebPage,&WebPage::linkHovered,this,&WebView::signal_linkHovered);
	setPage(m_pWebPage);
}
QWebView *WebView::createWindow(QWebPage::WebWindowType type)
{
	Q_UNUSED(type);

	WebView* webView=new WebView();
	emit signal_createWindow(&webView);
	return webView;
}
