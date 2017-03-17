#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTabWidget>

#include "myfunctions.h"
#include "tabwidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void slot_newTab(){newTab("");}
	void slot_closeTab(int index);
	void slot_findToNewTab();
	void slot_titleChanged(QWidget *widget, const QString &title);
	void slot_newWindow(WebView **view);
	void slot_openMenu();
private:
	QLineEdit* m_pFindFiled;
	QTabWidget* m_pTabs;
	int newTab(QString url);
	TabWidget *createTabWidget();
};

#endif // MAINWINDOW_H
