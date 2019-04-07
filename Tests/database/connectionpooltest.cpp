#include "connectionpooltest.h"
#include "database/connectionpool.h"

void ConnectionPoolTest::initTestCase()
{
    QString configFilePath = QCoreApplication::applicationDirPath() + "/../Tests/etc/test_db.json";
    ConnectionPool pool(configFilePath);
}

void ConnectionPoolTest::testGetConnection()
{
    DatabaseConnection* con1 = ConnectionPool().getConnection();
    DatabaseConnection* con2 = ConnectionPool().getConnection();
    DatabaseConnection* con3 = ConnectionPool().getConnection();
    DatabaseConnection* con4 = ConnectionPool().getConnection();
    DatabaseConnection* con5 = ConnectionPool().getConnection();

    QCOMPARE(con1->database().isOpen(), true);
    QCOMPARE(con2->database().isOpen(), true);
    QCOMPARE(con3->database().isOpen(), true);
    QCOMPARE(con4->database().isOpen(), true);
    QCOMPARE(con5->database().isOpen(), true);
    con1->setUnUsed();
    DatabaseConnection* con6 = ConnectionPool().getConnection();
    QCOMPARE(con6->database().isOpen(), true);
}

void ConnectionPoolTest::cleanupTestCase()
{
    ConnectionPool().destroy();
}
