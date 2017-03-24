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
	void signal_reload();
public slots:
	void importBookmarks();
	void changeDownloadPath();
	void toggleVal(const QString &param);
	void bookmarkMoveTo(const QString &arrayIn,const QString &arrayTo,const QString &param);
	void setVal(const QString &param, const QString &value);
};

#endif // JAVASCRIPTOBJ_H
