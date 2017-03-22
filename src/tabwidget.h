#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QLineEdit>
#include <QLabel>
#include <QSplitter>
#include <QCheckBox>
#include <QWebInspector>
#include <QWebFrame>

#include "WebKit/webview.h"
#include "javascriptobj.h"
#include "myfunctions.h"
#include "genpages.h"

class TabWidget : public QWidget
{
	Q_OBJECT
public:
	explicit TabWidget(QWidget *parent = 0);
	void load(const QUrl &url);
	void actionUrl(const QString &url);
	WebView* getView(){ return m_pWebView; }
	void stop(){m_pWebView->stop();}
	QString getUrl(){return m_pWebView->url().toString();}
signals:
	void signal_titleChanged(QWidget* widget,const QString &text);
	void signal_createWindow(WebView** webView);
	void signal_printRequested(QWebFrame *frame);
private slots:
	void slot_titleChanged(const QString &title);
	void slot_urlChanged(const QUrl &url);
	void slot_linkClicked(const QUrl &url);
	void slot_loadFinished();
	void slot_loadProgress(int prz);
	void slot_goToUrl();
	void slot_linkHovered(const QString &link);
	void slot_fintText(const QString &text);
private:
	WebView* m_pWebView;
	QWebInspector* m_pInspector;
	QProgressBar* m_pProgressBar;
	QLineEdit* m_pUrlField;
	QLineEdit* m_pTextFind;
	QPushButton* m_pPrefB;
	QPushButton* m_pNextB;
	QPushButton* m_pStopB;
	QPushButton* m_pReloadB;
	QLabel* m_pStatusLabel;
	QWidget* m_pFindBox;
	QCheckBox* m_pFindAll;
	void titleChanged(const QString &title);
};

#endif // TABWIDGET_H
