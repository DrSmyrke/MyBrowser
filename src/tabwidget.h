#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QLineEdit>
#include <QLabel>
#include <QSplitter>
#include <QWebInspector>
#include <QWebFrame>

#include "WebKit/webview.h"
#include "javascriptobj.h"
#include "myfunctions.h"

class TabWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TabWidget(QWidget *parent = 0);
	void load(const QUrl &url);
	void reload();
	void actionUrl(const QString &url);
	WebView* getView(){ return m_pWebView; }
	void stop(){m_pWebView->stop();}
signals:
	void signal_titleChanged(QWidget* widget,const QString &text);
	void signal_createWindow(WebView** webView);
private slots:
	void slot_titleChanged(const QString &title);
	void slot_urlChanged(const QUrl &url);
	void slot_linkClicked(const QUrl &url);
	void slot_loadFinished();
	void slot_loadProgress(int prz);
	void slot_goToUrl();
	void slot_linkHovered(const QString &link);
private:
	WebView* m_pWebView;
	QWebInspector* m_pInspector;
	QProgressBar* m_pProgressBar;
	QLineEdit* m_pUrlField;
	QPushButton* m_pPrefB;
	QPushButton* m_pNextB;
	QPushButton* m_pStopB;
	QPushButton* m_pReloadB;
	QLabel* m_pStatusLabel;
	void titleChanged(const QString &title);
};

#endif // TABWIDGET_H
