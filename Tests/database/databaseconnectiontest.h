#ifndef DATABASECONNECTIONTEST_H
#define DATABASECONNECTIONTEST_H

#include <QObject>
#include <QtTest/QtTest>

#include "database/connection.h"

class DatabaseConnectionTest : public QObject
{
    Q_OBJECT
private:
    QString configFilePath;
    QtConnectionPool::Connection connection;

public:
    DatabaseConnectionTest(QObject* parent);

private slots:
    void initTestCase();
    void testConnection();
    void testInUse();
    void testComparison();
    void testGetCreationTime();
    void testRefresh();
};

#endif // DATABASECONNECTIONTEST_H
