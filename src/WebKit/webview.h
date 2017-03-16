#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QAction>
#include <QtWebKit>
#include <QtWebKitWidgets/QWebView>
#include "webpage.h"

class WebView : public QWebView
{
	Q_OBJECT
public:
	explicit WebView(QWidget *parent = 0);
signals:
	void signal_linkHovered(const QString &link);
	void signal_createWindow(WebView** webView);
private:
	WebPage* m_pWebPage;

	// QWebView interface
protected:
	QWebView *createWindow(QWebPage::WebWindowType type) override;
};

#endif // WEBVIEW_H
