#ifndef CONNECTIONPOOLTEST_H
#define CONNECTIONPOOLTEST_H

#include <QObject>
#include <QtTest/QtTest>

#include "database/databaseconnection.h"

class ConnectionPoolTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testGetConnection();
    void cleanupTestCase();
};

#endif // CONNECTIONPOOLTEST_H
