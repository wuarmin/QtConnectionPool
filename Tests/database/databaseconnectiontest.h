#ifndef DATABASECONNECTIONTEST_H
#define DATABASECONNECTIONTEST_H

#include <QObject>
#include <QtTest/QtTest>

#include "database/databaseconnection.h"

class DatabaseConnectionTest : public QObject
{
    Q_OBJECT
private:
    DatabaseConnection* dbConnection;

private slots:
    void initTestCase();
    void testConnection();
    void testUsage();
    void cleanupTestCase();
};

#endif // DATABASECONNECTIONTEST_H
