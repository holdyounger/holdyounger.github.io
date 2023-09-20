#pragma once
#include <QObject>
#include <QtSql/QtSql>
#include <QCoreApplication>
#include <QString>
#include <QtSql/qsqldatabase.h>

#include "QLogEngine.h"

/*
	1. �����¾����ݿ�
		
	2. �޸Ĳ�����������ݿ������
			QSQLITE_CREATE_KEY,
			QSQLITE_REMOVE_KEY,
			QSQLITE_UPDATE_KEY,
			QSQLITE_BUSY_TIMEOUT,
			QSQLITE_OPEN_READONLY,
			QSQLITE_OPEN_URI,
			QSQLITE_ENABLE_SHARED_CACHE

	3. ���ݿ���

	4. ɾ��ԭ���ݿ�
*/

class QUpdateDB : public QObject
{
	Q_OBJECT
private:

	QUpdateDB();
	QUpdateDB(QString Path);

public:
	bool createKey(QString passwd);
	bool updatekey(QString oldPasswd, QString newPasswd);
	bool removekey(QString passwd);
	
	// ����
	bool open();

	// ����ת��
	bool copyFiletoNew(QString newPath, bool coverFileIfExist);
private:
	void Init();

public:
	static QUpdateDB* GetInstance();

private:
	QString m_dbPath;
	QSqlDatabase* m_database;
	static QUpdateDB* s_pInstance;

};

