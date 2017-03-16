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
				menuB->setMaximumSize(guiCfg::buttonSize);
		box->addWidget(menuB,0,0);
			QPushButton* printB=new QPushButton();
				printB->setIcon(QIcon::fromTheme("document-print",QIcon("://images/document-print.svg")));
				printB->setMaximumSize(guiCfg::buttonSize);
		box->addWidget(printB,0,1);
			QPushButton* addTabB=new QPushButton();
				addTabB->setIcon(QIcon("://images/tab-new.svg"));
				addTabB->setMaximumSize(guiCfg::buttonSize);
		box->addWidget(addTabB,0,2);
			m_pFindFiled=new QLineEdit();
				m_pFindFiled->setPlaceholderText(tr("Find text"));
				m_pFindFiled->setMaximumWidth(200);
		box->addWidget(m_pFindFiled,0,3);
			QPushButton* findB=new QPushButton();
				findB->setIcon(QIcon("://images/edit-find.svg"));

				findB->setMaximumSize(guiCfg::buttonSize);
		box->addWidget(findB,0,4);
		//body
			m_pTabs=new QTabWidget();
			m_pTabs->setTabsClosable(false);
			m_pTabs->setMovable(true);
			m_pTabs->setElideMode(Qt::ElideMiddle);
		box->addWidget(m_pTabs,1,0,1,10);
		//bottom
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
	if(index==-1) index=m_pTabs->currentIndex();
	if(m_pTabs->count()<1) return;
	TabWidget* tabWidget=static_cast<TabWidget*>(m_pTabs->widget(index));
	tabWidget->stop();
	tabWidget->deleteLater();
	m_pTabs->removeTab(index);
	if(m_pTabs->count()<2) m_pTabs->setTabsClosable(false);
}
void MainWindow::slot_newWindow(WebView **view)
{
	TabWidget* tabWidget=createTabWidget();
	(*view) = tabWidget->getView();
	m_pTabs->addTab(tabWidget,tr("New Tab"));
	if(m_pTabs->count()>1) m_pTabs->setTabsClosable(true);
}
void MainWindow::slot_findToNewTab()
{
	if(m_pFindFiled->text().isEmpty()) return;
	newTab("https://yandex.ru/yandsearch?text="+m_pFindFiled->text()+"&lr=25");
}
void MainWindow::newTab(QString url)
{
	if(url.isEmpty()) url=app::urlPreff+"://bookmarks";
	TabWidget* tabWidget=createTabWidget();
	tabWidget->actionUrl(url);
	m_pTabs->addTab(tabWidget,tr("New Tab"));
	if(m_pTabs->count()>1) m_pTabs->setTabsClosable(true);
}

TabWidget* MainWindow::createTabWidget()
{
	TabWidget* tabWidget=new TabWidget();
	connect(tabWidget,&TabWidget::signal_titleChanged,this,&MainWindow::slot_titleChanged);
	connect(tabWidget,&TabWidget::signal_createWindow,this,&MainWindow::slot_newWindow);
	return tabWidget;
}
void MainWindow::slot_titleChanged(QWidget *widget, const QString &title)
{
	for(int i=0;i<m_pTabs->count();i++){
		if(widget==m_pTabs->widget(i)){
			m_pTabs->setTabText(i,title);
			m_pTabs->setTabToolTip(i,title);
			break;
		}
	}
}
MainWindow::~MainWindow()
{

}
