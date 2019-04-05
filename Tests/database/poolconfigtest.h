#ifndef POOLCONFIGTEST_H
#define POOLCONFIGTEST_H

#include <QObject>
#include <QtTest/QtTest>

#include "database/databaseconnection.h"

class PoolConfigTest : public QObject
{
    Q_OBJECT

private slots:
    void testIndividualValues();
    void testDefaultValues();
};

#endif // POOLCONFIGTEST_H
