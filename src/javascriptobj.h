#ifndef JAVASCRIPTOBJ_H
#define JAVASCRIPTOBJ_H

#include <QObject>
//FIXME: remove
#include <QDebug>

class JavaScriptObj : public QObject
{
	Q_OBJECT
public:
	explicit JavaScriptObj(QObject *parent = 0);
signals:

public slots:
	void openTest(const QString &test){qDebug()<<">"<<test;}
};

#endif // JAVASCRIPTOBJ_H
