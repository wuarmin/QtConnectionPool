#include <QSharedPointer>

#include "connectionpooltest.h"
#include "database/connectionpool.h"
#include "testHelpers/asyncconnectionuser.h"

void ConnectionPoolTest::initTestCase()
{
    QString configFilePath = QCoreApplication::applicationDirPath() + "/../Tests/etc/test_db.json";
    ConnectionPool pool(configFilePath);
}

void ConnectionPoolTest::testGetConnection()
{
	//this expect test_db.json.maxCon = 5
    Connection con1 = ConnectionPool().getConnection();
    Connection con2 = ConnectionPool().getConnection();
    Connection con3 = ConnectionPool().getConnection();
    Connection con4 = ConnectionPool().getConnection();
    Connection con5 = ConnectionPool().getConnection();
    Connection con6 = ConnectionPool().getConnection();

    QList<Connection*> validConnections;
    validConnections << &con1 << &con2 << &con3 << &con4 << &con5;
    QListIterator<Connection*> validConnectionsIterator(validConnections);
    while (validConnectionsIterator.hasNext()) {
        Connection* con = validConnectionsIterator.next();
        QCOMPARE(con->isInUse(), true);
        QCOMPARE(con->database().isOpen(), true);
    }

    QCOMPARE(con6.isInUse(), false);
    QCOMPARE(con6.database().isOpen(), false);
}

void ConnectionPoolTest::testAsynchronConnectionUsers()
{
	//this expect test_db.json.maxCon = 5
    for (int i = 0; i < 1000; ++i) {

        QList<QSharedPointer<AsyncConnectionUser> > asyncConnectionUsers;
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser1")));
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser2")));
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser3")));
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser4")));
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser5")));
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser6")));
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser7")));

        QThreadPool* threadPool = new QThreadPool();
        threadPool->setMaxThreadCount(7);

        foreach (QSharedPointer<AsyncConnectionUser> asyncConnectionUser, asyncConnectionUsers) {
            threadPool->start(asyncConnectionUser.data());
        }
        threadPool->waitForDone();

        int successCount = 0;
        int failCount = 0;
        foreach (QSharedPointer<AsyncConnectionUser> asyncConnectionUser, asyncConnectionUsers) {
            if (asyncConnectionUser->getSuccess()) {
                ++successCount;
            }
            else {
                ++failCount;
            }
        }

        QCOMPARE(successCount, 5);
        QCOMPARE(failCount, 2);
    }
}

void ConnectionPoolTest::cleanupTestCase()
{
    ConnectionPool().destroy();
}
