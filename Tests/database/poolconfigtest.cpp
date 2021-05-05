#include "poolconfigtest.h"
#include "database/poolconfig.h"

using namespace QtConnectionPool;

void PoolConfigTest::testDefaultValues()
{
    QString configFilePath = QCoreApplication::applicationDirPath() + "/../Tests/etc/empty_config.json";
    PoolConfig poolConfig(configFilePath);
    DatabaseConfig dbConfig = poolConfig.dbConfig;

    QCOMPARE(poolConfig.checkInterval, 10000);
    QCOMPARE(poolConfig.minConnections, 1);
    QCOMPARE(poolConfig.maxConnections, 3);
    QCOMPARE(poolConfig.connectionLifePeriod, 300000);

    QCOMPARE(dbConfig.host, QString());
    QCOMPARE(dbConfig.driver, QString("QPSQL"));
    QCOMPARE(dbConfig.port, 5432);
    QCOMPARE(dbConfig.database, QString());
    QCOMPARE(dbConfig.user, QString());
    QCOMPARE(dbConfig.password, QString());
}

void PoolConfigTest::testIndividualValues()
{
    QString configFilePath = QCoreApplication::applicationDirPath() + "/../Tests/etc/individual_config.json";
    PoolConfig poolConfig(configFilePath);
    DatabaseConfig dbConfig = poolConfig.dbConfig;

    QCOMPARE(poolConfig.checkInterval, 30000);
    QCOMPARE(poolConfig.minConnections, 2);
    QCOMPARE(poolConfig.maxConnections, 7);
    QCOMPARE(poolConfig.connectionLifePeriod, 600000);

    QCOMPARE(dbConfig.host, QString("happy-host.com"));
    QCOMPARE(dbConfig.driver, QString("OCI"));
    QCOMPARE(dbConfig.port, 9999);
    QCOMPARE(dbConfig.database, QString("happy_db"));
    QCOMPARE(dbConfig.user, QString("forrest_gump"));
    QCOMPARE(dbConfig.password, QString("xxxyyy"));
}
