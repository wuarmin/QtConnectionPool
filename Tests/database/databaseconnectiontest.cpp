#include <QSqlQuery>
#include <QThread>

#include "databaseconnectiontest.h"
#include "database/poolconfig.h"

using namespace QtConnectionPool;

DatabaseConnectionTest::DatabaseConnectionTest(QObject* parent) :
    QObject(parent)
{
    this->configFilePath = QCoreApplication::applicationDirPath() + "/../Tests/etc/test_db.json";
}

void DatabaseConnectionTest::initTestCase()
{
    PoolConfig poolConfig(this->configFilePath);
    this->connection = Connection(poolConfig.dbConfig);
}

void DatabaseConnectionTest::testConnection()
{
    QSqlQuery query(this->connection.database());
    bool ok = query.exec("select 'it works' from dual");
    QCOMPARE(ok, true);

    query.next();
    QString result = query.value(0).toString();
    QCOMPARE(result, QString("it works"));
}

void DatabaseConnectionTest::testInUse()
{
    QCOMPARE(this->connection.isInUse(), false);
    this->connection.use();
    QCOMPARE(this->connection.isInUse(), true);
}

void DatabaseConnectionTest::testComparison()
{
    Connection myConn;

    QCOMPARE((this->connection == myConn), false);

    myConn = this->connection;

    QCOMPARE((this->connection == myConn), true);
}

void DatabaseConnectionTest::testGetCreationTime()
{
    PoolConfig poolConfig(this->configFilePath);
    Connection myConn(poolConfig.dbConfig);

    QThread::msleep(10);

    QCOMPARE((myConn.getCreationTime() < QDateTime::currentMSecsSinceEpoch()), true);
}

void DatabaseConnectionTest::testRefresh()
{
    PoolConfig poolConfig(this->configFilePath);
    Connection myConn(poolConfig.dbConfig);

    qint64 initialTimeStamp = myConn.getCreationTime();

    QCOMPARE(myConn.database().open(), true);
    myConn.refresh();
    QCOMPARE(myConn.database().open(), true);
    QCOMPARE((myConn.getCreationTime() > initialTimeStamp), true);
}

