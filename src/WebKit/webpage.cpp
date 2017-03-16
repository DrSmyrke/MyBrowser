#include "webpage.h"

WebPage::WebPage(QObject *parent) : QWebPage(parent)
{
	QMenu* menu=new QMenu();
		menu->addAction("Hide");
		menu->addAction("Show");
	this->action(QWebPage::InspectElement)->setMenu(menu);
}
