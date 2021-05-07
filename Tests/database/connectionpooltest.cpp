#include <QSharedPointer>
//#include <QApplication>

#include "database/poolconfig.h"
#include "database/connectionpool.h"

#include "connectionpooltest.h"
#include "testHelpers/asyncconnectionuser.h"
#include "testHelpers/PrgLog.h"

using namespace QtConnectionPool;

struct ConnectionPoolTest::Handler {
    QEventLoop loop;
    ConnectionPool pool;

    Handler(PoolConfig conf)
    : loop()
    , pool(conf)
    {}
};

void ConnectionPoolTest::initTestCase()
{
    qInstallMessageHandler(PrgLog::qtMessageOutput);
    PoolConfig conf(":configs/test_db.json");
    conf.minConnections = 2; //force this to 2 for test
    conf.maxConnections = 5; //force this to 5 for test
    conf.connectionLifePeriod = 25000; //force this to 25s for test
    conf.inactivityPeriod = 3000; //force this to 10s for test
    conf.checkInterval = 1000; //force this to 500ms
    _dataHandler = new Handler(conf);
}

void ConnectionPoolTest::testGetConnection()
{
	//this expect test_db.json.maxCon = 5
    QSharedPointer<Connection> con1 = ConnectionPool().getConnection();
    QSharedPointer<Connection> con2 = ConnectionPool().getConnection();
    QSharedPointer<Connection> con3 = ConnectionPool().getConnection();
    QSharedPointer<Connection> con4 = ConnectionPool().getConnection();
    QSharedPointer<Connection> con5 = ConnectionPool().getConnection();
    QSharedPointer<Connection> con6 = ConnectionPool().getConnection();

    QList<QSharedPointer<Connection>> validConnections;
    validConnections << con1 << con2 << con3 << con4 << con5;
    QListIterator<QSharedPointer<Connection>> validConnectionsIterator(validConnections);
    while (validConnectionsIterator.hasNext()) {
        QSharedPointer<Connection> con = validConnectionsIterator.next();
        QCOMPARE(con && con->isInUse(), true);
        QCOMPARE(con && con->database().isOpen(), true);
    }
    QCOMPARE(con6 && con6->isInUse(), false);
    QCOMPARE(con6 && con6->database().isOpen(), false);

    //explicit unborrow
    QListIterator<QSharedPointer<Connection>> validConnectionsIterator2(validConnections);
    while (validConnectionsIterator2.hasNext()) {
        ConnectionPool().unBorrowConnection(validConnectionsIterator2.next());
    }
}

void ConnectionPoolTest::testNoUnBorrow()
{
    //this expect test_db.json.maxCon = 5
    QSharedPointer<Connection> con1 = ConnectionPool().getConnection();
    QSharedPointer<Connection> con2 = ConnectionPool().getConnection();
    QSharedPointer<Connection> con3 = ConnectionPool().getConnection();
    QSharedPointer<Connection> con4 = ConnectionPool().getConnection();
    QSharedPointer<Connection> con5 = ConnectionPool().getConnection();
    QSharedPointer<Connection> con6 = ConnectionPool().getConnection();

    QList<QSharedPointer<Connection>> validConnections;
    validConnections << con1 << con2 << con3 << con4 << con5;
    QListIterator<QSharedPointer<Connection>> validConnectionsIterator(validConnections);
    while (validConnectionsIterator.hasNext()) {
        QSharedPointer<Connection> con = validConnectionsIterator.next();
        QCOMPARE(con && con->isInUse(), true);
        QCOMPARE(con && con->database().isOpen(), true);
    }

    QCOMPARE(con6 && con6->isInUse(), false);
    QCOMPARE(con6 && con6->database().isOpen(), false);
}

void ConnectionPoolTest::testAsynchronConnectionUsers()
{
    for (int i = 0; i < 20; ++i) {

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

//testing release via inactivity and refresh behaviour
void ConnectionPoolTest::testCheckGetConnectionWithWait()
{
    uint64_t waitTimout = 20000; //will wait 20s

    for (int i = 0; i < 20; ++i) {
        QList<QSharedPointer<AsyncConnectionUser> > asyncConnectionUsers;
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser1",waitTimout)));
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser2",waitTimout)));
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser3",waitTimout)));
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser4",waitTimout)));
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser5",waitTimout)));
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser6",waitTimout)));
        asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser7",waitTimout)));

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
        QCOMPARE(successCount, 7); //all con should have succeed, we wait the release with waitTimeout
        QCOMPARE(failCount, 0);
    }
}

void ConnectionPoolTest::testCheckConnectionPool() {
    //this expect test_db.json.maxCon = 5
    //this expect test_db.json.minCon = 2 //inactivityPeriod
    //this expect inactivityPeriod = 10000
    uint64_t waitTimout = 20000; //will wait 20s
    QList<QSharedPointer<AsyncConnectionUser> > asyncConnectionUsers;
    asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser1",waitTimout)));
    asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser2",waitTimout)));
    asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser3",waitTimout)));
    asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser4",waitTimout)));
    asyncConnectionUsers.append(QSharedPointer<AsyncConnectionUser>(new AsyncConnectionUser("asyncUser5",waitTimout)));

    QThreadPool* threadPool = new QThreadPool();
    threadPool->setMaxThreadCount(5);

    //we created 5con, after inactivityPeriod, all con > minCon should be dropped.
    foreach (QSharedPointer<AsyncConnectionUser> asyncConnectionUser, asyncConnectionUsers) {
        threadPool->start(asyncConnectionUser.data());
    }
    //on pourrait check si mnBorrowe>0 ici mais non reliable
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    const qint64 startWait = now;
    auto st = ConnectionPool().getPoolStats();
    while(!(static_cast<uint64_t>(now - startWait) > waitTimout)
        && (st._nbAvailable < 2) ){
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        now = QDateTime::currentMSecsSinceEpoch();
        st = ConnectionPool().getPoolStats();
    }
    //QThread::usleep(11*1000*1000);
    //_dataHandler->loop.exec();
    threadPool->waitForDone();
    //at this point we should have 2 con active only
    QVERIFY( st._nbAvailable >= 2 );
}

void ConnectionPoolTest::cleanupTestCase()
{
    ConnectionPool().destroy();
    delete _dataHandler;
}
