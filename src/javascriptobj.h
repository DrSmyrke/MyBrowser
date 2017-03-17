#ifndef JAVASCRIPTOBJ_H
#define JAVASCRIPTOBJ_H

#include <QObject>
#include "myfunctions.h"
//FIXME: remove
#include <QDebug>

class JavaScriptObj : public QObject
{
	Q_OBJECT
public:
	explicit JavaScriptObj(QObject *parent = 0);
signals:

public slots:
	void importBookmarks();
	void changeDownloadPath();
	void toggleVal(const QString &param);
	void setVal(const QString &param, const QString &value);
};

#endif // JAVASCRIPTOBJ_H
