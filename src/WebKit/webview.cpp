#include "webview.h"

WebView::WebView(QWidget *parent) : QWebView(parent)
{
	this->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
}

