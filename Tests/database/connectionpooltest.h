#ifndef CONNECTIONPOOLTEST_H
#define CONNECTIONPOOLTEST_H

#include <QObject>
#include <QtTest/QtTest>

class ConnectionPoolTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testGetConnection();
    void testAsynchronConnectionUsers();
    void testCheckConnectionPool();
    void cleanupTestCase();
};

#endif // CONNECTIONPOOLTEST_H
