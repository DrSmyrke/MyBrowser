#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QtWebKit>
#include <QtWebKitWidgets/QWebView>

class WebView : public QWebView
{
	Q_OBJECT
public:
	explicit WebView(QWidget *parent = 0);
private:

};

#endif // WEBVIEW_H
