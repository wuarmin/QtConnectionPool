#ifndef CONNECTIONPOOLTEST_H
#define CONNECTIONPOOLTEST_H

#include <QObject>
#include <QtTest/QtTest>

class ConnectionPoolTest : public QObject
{
    Q_OBJECT

public:
    struct Handler;
    Handler* _dataHandler;

private slots:
    void initTestCase();
    void testExplicitUnBorrow();
    void testImplicitUnBorrow();
    void testAsynchronConnectionUsers();
    void testCheckGetConnectionWithWait();
    void testCheckConnectionPool();
    void cleanupTestCase();
};

#endif // CONNECTIONPOOLTEST_H
