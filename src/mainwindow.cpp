#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle("MyBrowser");
	setMinimumSize(600,480);

	QWidget* centrWidget=new QWidget(this);
		QGridLayout* box=new QGridLayout();
		box->setContentsMargins(0,3,0,0);
		//top panel
			QPushButton* menuB=new QPushButton();
				menuB->setIcon(QIcon("://images/applications-system.svg"));
				menuB->setMaximumSize(gui::buttonSize);
		box->addWidget(menuB,0,0);
			QPushButton* printB=new QPushButton();
				printB->setIcon(QIcon::fromTheme("document-print",QIcon("://images/document-print.svg")));
				printB->setMaximumSize(gui::buttonSize);
		box->addWidget(printB,0,1);
			m_pUrlField=new QLineEdit();
		box->addWidget(m_pUrlField,0,2);
			QPushButton* goB=new QPushButton();
				goB->setIcon(QIcon("://images/dialog-apply.svg"));
				goB->setMaximumSize(gui::buttonSize);
		box->addWidget(goB,0,3);
			QPushButton* reloadB=new QPushButton();
				reloadB->setIcon(QIcon("://images/view-refresh.svg"));
				reloadB->setMaximumSize(gui::buttonSize);
		box->addWidget(reloadB,0,4);
			QPushButton* stopB=new QPushButton();
				stopB->setIcon(QIcon("://images/window-close.svg"));
				stopB->setMaximumSize(gui::buttonSize);
		box->addWidget(stopB,0,5);
			QPushButton* goHomeB=new QPushButton();
				goHomeB->setIcon(QIcon("://images/go-home.svg"));
				goHomeB->setMaximumSize(gui::buttonSize);
		box->addWidget(goHomeB,0,6);
			QPushButton* addTabB=new QPushButton();
				addTabB->setIcon(QIcon("://images/tab-new.svg"));
				addTabB->setMaximumSize(gui::buttonSize);
		box->addWidget(addTabB,0,7);
			m_pFindFiled=new QLineEdit();
				m_pFindFiled->setMaximumWidth(200);
		box->addWidget(m_pFindFiled,0,8);
			QPushButton* findB=new QPushButton();
				findB->setIcon(QIcon("://images/edit-find.svg"));
				findB->setMaximumSize(gui::buttonSize);
		box->addWidget(findB,0,9);
		//body
			m_pTabs=new QTabWidget();
			m_pTabs->setTabsClosable(false);
		box->addWidget(m_pTabs,1,0,1,10);
	centrWidget->setLayout(box);

	setCentralWidget(centrWidget);

	//signals
	connect(m_pTabs,&QTabWidget::tabCloseRequested,this,&MainWindow::slot_closeTab);
	connect(addTabB,&QPushButton::clicked,this,&MainWindow::slot_newTab);
	connect(m_pFindFiled,&QLineEdit::returnPressed,this,&MainWindow::slot_findToNewTab);
	connect(findB,&QPushButton::clicked,this,&MainWindow::slot_findToNewTab);
}
void MainWindow::slot_closeTab(int index)
{
	m_pTabs->widget(index)->deleteLater();
	m_pTabs->removeTab(index);
	if(m_pTabs->count()<2) m_pTabs->setTabsClosable(false);
}
void MainWindow::slot_findToNewTab()
{
	if(m_pFindFiled->text().isEmpty()) return;
	newTab("https://yandex.ru/yandsearch?text="+m_pFindFiled->text()+"&lr=25");
}
void MainWindow::slot_newTab()
{
	newTab("");
}
void MainWindow::newTab(QString url)
{
	if(url.isEmpty()) url=appCfg::urlPreffics+"://bookmarks";
	QWebView *view = new QWebView();
	view->load(QUrl(url));
	m_pTabs->addTab(view,"New Tab");
	if(m_pTabs->count()>1) m_pTabs->setTabsClosable(true);
}
MainWindow::~MainWindow()
{

}
