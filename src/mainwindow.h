#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTabWidget>
#include <QtWebKit>
#include <QtWebKitWidgets/QWebView>

#include "myfunctions.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void slot_newTab();
	void slot_closeTab(int index);
	void slot_findToNewTab();
private:
	QLineEdit* m_pUrlField;
	QLineEdit* m_pFindFiled;
	QTabWidget* m_pTabs;
	void newTab(QString url);
};

#endif // MAINWINDOW_H
