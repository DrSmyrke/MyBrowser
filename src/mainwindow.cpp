#include "mainwindow.h"

#include <QTabBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle("MyBrowser v"+app::conf.version);
	setWindowIcon(QIcon("://index.ico"));
	setMinimumSize(640,480);

	QWidget* centrWidget=new QWidget(this);
		QGridLayout* box=new QGridLayout();
		box->setContentsMargins(0,3,0,0);
		//top panel
			QPushButton* menuB=new QPushButton();
				menuB->setIcon(QIcon("://images/system.png"));
				menuB->setMaximumSize(app::conf.guiCfg.buttonSize);
				menuB->setFlat(true);
		box->addWidget(menuB,0,0);
			QPushButton* printB=new QPushButton();
				printB->setIcon(QIcon("://images/print.png"));
				printB->setMaximumSize(app::conf.guiCfg.buttonSize);
				printB->setFlat(true);
				printB->setShortcut(QKeySequence::Print);
				connect(printB,&QPushButton::clicked,this,&MainWindow::slot_printPage);
		box->addWidget(printB,0,1);
			QPushButton* bookmarksB=new QPushButton();
				bookmarksB->setIcon(QIcon("://images/about.png"));
				bookmarksB->setMaximumSize(app::conf.guiCfg.buttonSize);
				bookmarksB->setFlat(true);
		box->addWidget(bookmarksB,0,2);
			QPushButton* downloadsB=new QPushButton();
				downloadsB->setIcon(QIcon("://images/save.png"));
				downloadsB->setMaximumSize(app::conf.guiCfg.buttonSize);
				downloadsB->setFlat(true);
		box->addWidget(downloadsB,0,3);
			QPushButton* addTabB=new QPushButton();
				addTabB->setIcon(QIcon("://images/tab-new.png"));
				addTabB->setMaximumSize(app::conf.guiCfg.buttonSize);
				addTabB->setFlat(true);
		box->addWidget(addTabB,0,4);
			m_pFindFiled=new QLineEdit();
				m_pFindFiled->setPlaceholderText(tr("Find text"));
				m_pFindFiled->setMaximumWidth(200);
		box->addWidget(m_pFindFiled,0,5);
			QPushButton* findB=new QPushButton();
				findB->setIcon(QIcon("://images/find.png"));
				findB->setMaximumSize(app::conf.guiCfg.buttonSize);
				findB->setFlat(true);
		box->addWidget(findB,0,6);
		//body
			m_pTabs=new QTabWidget();
			m_pTabs->setTabsClosable(false);
			m_pTabs->setMovable(true);
			m_pTabs->setElideMode(Qt::ElideMiddle);
			QAction* closeTab=new QAction(this);
				closeTab->setShortcut(QKeySequence::Close);
				connect(closeTab,&QAction::triggered,this,[this](){slot_closeTab(m_pTabs->currentIndex());});
			m_pTabs->addAction(closeTab);
		box->addWidget(m_pTabs,1,0,1,10);
		//bottom
	centrWidget->setLayout(box);

	setCentralWidget(centrWidget);

	//signals
	connect(m_pTabs,&QTabWidget::tabCloseRequested,this,&MainWindow::slot_closeTab);
	connect(addTabB,&QPushButton::clicked,this,[this](){ newTab("about:blank"); });
	connect(m_pFindFiled,&QLineEdit::returnPressed,this,&MainWindow::slot_findToNewTab);
	connect(findB,&QPushButton::clicked,this,&MainWindow::slot_findToNewTab);
	connect(menuB,&QPushButton::clicked,this,&MainWindow::slot_openMenu);
	connect(bookmarksB,&QPushButton::clicked,this,&MainWindow::slot_openBookmarks);

	if(app::conf.openBrowser == "homePage") newTab(app::conf.homePage);
	if(app::conf.openBrowser == "blank") newTab("about:blank");
	if(app::conf.openBrowser == "lastTime"){
		for(auto elem:app::conf.lastPages) newTab(elem);
	}

#ifndef QT_NO_OPENSSL
	if (!QSslSocket::supportsSsl()) {
	QMessageBox::information(0, "Demo Browser",
				 "This system does not support OpenSSL. SSL websites will not be available.");
	}
#endif
}
void MainWindow::slot_closeTab(int index)
{
	if(index==-1) index=m_pTabs->currentIndex();
	if(m_pTabs->count()<2) return;
	TabWidget* tabWidget=static_cast<TabWidget*>(m_pTabs->widget(index));
	//TODO: Надо ли?
	//if(tabWidget->getUrl()=="about:settings") app::saveSettings();
	//if(tabWidget->getUrl()=="about:bookmarks") app::saveBookmarks();
	tabWidget->stop();
	tabWidget->deleteLater();
	m_pTabs->removeTab(index);
	if(m_pTabs->count()<2) m_pTabs->setTabsClosable(false);
}
void MainWindow::slot_newWindow(WebView **view)
{
	TabWidget* tabWidget=createTabWidget();
	(*view) = tabWidget->getView();
	int index=m_pTabs->addTab(tabWidget,tr("New Tab"));
	if(m_pTabs->count()>1) m_pTabs->setTabsClosable(true);
	if(app::conf.switchToNewTab) m_pTabs->setCurrentIndex(index);
}
void MainWindow::slot_openMenu()
{
	for(int i=0;i<m_pTabs->count();i++){
		TabWidget* tabWidget=static_cast<TabWidget*>(m_pTabs->widget(i));
		if(tabWidget->getUrl()=="about:settings") return;
	}
	m_pTabs->setCurrentIndex(newTab("about:settings"));
}
void MainWindow::slot_openBookmarks()
{
	for(int i=0;i<m_pTabs->count();i++){
		TabWidget* tabWidget=static_cast<TabWidget*>(m_pTabs->widget(i));
		if(tabWidget->getUrl()=="about:bookmarks") return;
	}
	m_pTabs->setCurrentIndex(newTab("about:bookmarks"));
}
void MainWindow::slot_printPage()
{
	TabWidget* tabWidget=static_cast<TabWidget*>(m_pTabs->widget(m_pTabs->currentIndex()));
	slot_printRequested(tabWidget->getView()->page()->mainFrame());
}
void MainWindow::slot_printRequested(QWebFrame *frame)
{
	QPrinter printer;
	QPrintDialog *dialog = new QPrintDialog(&printer, this);
	dialog->setWindowTitle(tr("Print Document"));
	dialog->setLocale(QLocale::system());
	if (dialog->exec() != QDialog::Accepted)
		return;
	frame->print(&printer);
}
void MainWindow::slot_findToNewTab()
{
	if(m_pFindFiled->text().isEmpty()) return;
	newTab("https://yandex.ru/yandsearch?text="+m_pFindFiled->text()+"&lr=25");
}
int MainWindow::newTab(QString url)
{
	TabWidget* tabWidget=createTabWidget();
	tabWidget->actionUrl(url);
	int index=m_pTabs->addTab(tabWidget,tr("New Tab"));
	if(m_pTabs->count()>1) m_pTabs->setTabsClosable(true);
	if(app::conf.switchToNewTab) m_pTabs->setCurrentIndex(index);
	return index;
}

TabWidget* MainWindow::createTabWidget()
{
	TabWidget* tabWidget=new TabWidget();
	connect(tabWidget,&TabWidget::signal_titleChanged,this,&MainWindow::slot_titleChanged);
	connect(tabWidget,&TabWidget::signal_createWindow,this,&MainWindow::slot_newWindow);
	connect(tabWidget,&TabWidget::signal_printRequested,this,&MainWindow::slot_printRequested);
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
	if(app::conf.openBrowser == "lastTime"){
		app::conf.lastPages.clear();
		for(int i=0;i<m_pTabs->count();i++){
			TabWidget* tabWidget=static_cast<TabWidget*>(m_pTabs->widget(i));
			if(tabWidget->getUrl()=="about:settings") continue;
			app::conf.lastPages.push_back(tabWidget->getUrl());
		}
	}
	app::saveSettings();
}
