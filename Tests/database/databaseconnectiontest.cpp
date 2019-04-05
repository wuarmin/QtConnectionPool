#include <QSqlQuery>

#include "databaseconnectiontest.h"
#include "database/databaseconnection.h"
#include "database/poolconfig.h"

void DatabaseConnectionTest::initTestCase()
{
    QString configFilePath = QCoreApplication::applicationDirPath() + "/../Tests/etc/test_db.json";
    PoolConfig poolConfig(configFilePath);

    this->dbConnection = new DatabaseConnection(poolConfig.dbConfig);
}

void DatabaseConnectionTest::testConnection()
{
    QSqlQuery query(this->dbConnection->database());
    bool ok = query.exec("select 'it works'");
    QCOMPARE(ok, true);

    query.next();
    QString result = query.value(0).toString();
    QCOMPARE(result, QString("it works"));
}

void DatabaseConnectionTest::testUsage()
{
    this->dbConnection->setInUse();
    QCOMPARE(this->dbConnection->isInUse(), true);

    this->dbConnection->setUnUsed();
    QCOMPARE(this->dbConnection->isInUse(), false);
}

void DatabaseConnectionTest::cleanupTestCase()
{
    delete this->dbConnection;
}
