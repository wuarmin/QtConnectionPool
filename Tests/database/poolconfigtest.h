#ifndef POOLCONFIGTEST_H
#define POOLCONFIGTEST_H

#include <QObject>
#include <QtTest/QtTest>

class PoolConfigTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testIndividualValues();
    void testDefaultValues();
};

#endif // POOLCONFIGTEST_H
