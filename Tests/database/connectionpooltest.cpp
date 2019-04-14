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

    con2->setUnUsed();
    con3->setUnUsed();
    con4->setUnUsed();
    con5->setUnUsed();
    con6->setUnUsed();
}

void ConnectionPoolTest::testAsynchronConnectionUsers()
{
    QList<QSharedPointer<AsyncConnectionUser> > asyncConnectionUsers;
    asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser1")));
    asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser2")));
    asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser3")));
    asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser4")));
    asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser5")));
    asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser6")));
    asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser7")));

    QThreadPool* threadPool = new QThreadPool();
    threadPool->setMaxThreadCount(10);

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

void ConnectionPoolTest::cleanupTestCase()
{
    ConnectionPool().destroy();
}
