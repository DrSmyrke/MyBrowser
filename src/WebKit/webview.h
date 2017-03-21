#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QAction>
#include <QtWebKit>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>
#include "webpage.h"
#include "myfunctions.h"

class WebView : public QWebView
{
	Q_OBJECT
public:
	explicit WebView(QWidget *parent = 0);
signals:
	void signal_linkHovered(const QString &link);
	void signal_createWindow(WebView** webView);
	void signal_printRequested(QWebFrame * frame);
private:
	WebPage* m_pWebPage;

	// QWebView interface
protected:
	QWebView *createWindow(QWebPage::WebWindowType type) override;
};

#endif // WEBVIEW_H
