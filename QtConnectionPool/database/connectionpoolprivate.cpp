#include <QDateTime>
#include <QThread>
#include <QMutableListIterator>
#include <QMutexLocker>
#include <QDebug>

#include "connectionpoolprivate.h"

namespace QtConnectionPool {
    namespace {
        PoolConfig g_config;
        QObject* g_parent;

        static void unBorrowDeleteHandler(Connection* obj) {
            //qDebug("Entering unBorrowDeleteHandler obj=%p", obj);
            ConnectionPoolPrivate& privatePoolInstance = ConnectionPoolPrivate::getInstance();
            if (!privatePoolInstance.stop) {
                privatePoolInstance.unBorrowConnection(QSharedPointer<Connection>(obj));
            }
        }
    }

    ConnectionPoolPrivate& ConnectionPoolPrivate::getInstance() {
        static ConnectionPoolPrivate instance(g_config, g_parent);
        return instance;
    }

    void ConnectionPoolPrivate::setupPool(const PoolConfig &config, QObject *parent) {
        g_config = config;
        g_parent = parent;
    }

    ConnectionPoolPrivate::ConnectionPoolPrivate(const PoolConfig& config, QObject* parent)
            : QObject(parent)
            , stat()
            , config(config)
            , checkTimer()
            , mutex(QMutex::Recursive)
            , connectionPool()
            , stop(false)
    {
        this->initPool();

        qDebug("Scheduling checkConnectionPool in =%d",this->config.checkInterval);
        this->checkTimer.start(this->config.checkInterval);
        connect(&checkTimer, SIGNAL(timeout()), SLOT(checkConnectionPool()));
    }

    void ConnectionPoolPrivate::initPool() {
        qDebug("ConnectionPoolPrivate::initPool");
        QMutexLocker locker(&mutex);
        const int minConnections = this->config.minConnections;
        for (int i = 0; i < minConnections; ++i) {
            this->connectionPool.append(this->createNewConnection());
        }
        stat._nbAvailable = this->connectionPool.size();
    }

    PoolStats ConnectionPoolPrivate::getPoolStats() const {
        QMutexLocker locker(&mutex);
        return stat;
    }

    void ConnectionPoolPrivate::unBorrowConnection(QSharedPointer<Connection> con) {
        //qDebug("ConnectionPoolPrivate::unBorrowConnection received unborrow notification");
        if (!con.isNull()) {
            QMutexLocker locker(&mutex);
            this->connectionPool.push_back(con);
            ++stat._nbAvailable;
            --stat._nbBorrowed;
            if(!this->checkTimer.isActive()){
                qDebug("Reactivate timer for =%d",this->config.checkInterval);
                this->checkTimer.start(this->config.checkInterval);
            }
        }
    }

    QSharedPointer<Connection> ConnectionPoolPrivate::getConnection(uint64_t waitTimeoutInMs) {
        //qDebug("ConnectionPoolPrivate::getConnection waitTimeoutInMs=%lu",waitTimeoutInMs);
        {
            QMutexLocker locker(&mutex);
            //qDebug("ConnectionPoolPrivate::getConnection _nbAvailable=%d, _nbBorrowed=%d",stat._nbAvailable,stat._nbBorrowed);
            if(stat._nbAvailable > 0){
                QSharedPointer<Connection> connection = connectionPool.front();
                this->connectionPool.pop_front();
                --stat._nbAvailable; //upd _nbAvailable size
                ++stat._nbBorrowed;
                connection->use();
                return connection;
            } else if(stat._nbBorrowed < this->config.maxConnections) {
                //let create a new and borrow it right away
                QSharedPointer<Connection> newConnection = this->createNewConnection();
                ++stat._nbBorrowed; //upd _nbBorrowed size
                newConnection->use();
                return newConnection;
            }
        }

        //else we need to wait a thread has finish to have a working connection
        if (waitTimeoutInMs > 0) {
            const qint64 startWait = QDateTime::currentMSecsSinceEpoch();
            qDebug("ConnectionPoolPrivate::getConnection waiting for connection with waitTimeoutInMs=%lu, startWait=%llu",waitTimeoutInMs,startWait);
            while (1) {
                auto con = getConnection(0);
                if (con && con->isValid()) {
                    return con;
                }
                const qint64 now = QDateTime::currentMSecsSinceEpoch();
                if (static_cast<uint64_t>(now - startWait) > waitTimeoutInMs) { //timeout expired
                    qDebug("ConnectionPoolPrivate::getConnection timeout=%lu expired, now=%llu",waitTimeoutInMs,now);
                    break;
                }
                QThread::usleep(10*1000);
                //QThread::yieldCurrentThread();
            }
        }

        return QSharedPointer<Connection>(nullptr); //invalid con
    }

    QSharedPointer<Connection> ConnectionPoolPrivate::createNewConnection() {
        return QSharedPointer<Connection>(new Connection(this->config.dbConfig), unBorrowDeleteHandler );
    }

    //manage the pool size
    void ConnectionPoolPrivate::checkConnectionPool() {
        qDebug("Starting checkConnectionPool");
        const qint64 now = QDateTime::currentMSecsSinceEpoch();
        const qint64 connectionLifePeriod = this->config.connectionLifePeriod;
        const qint64 inactivityPeriod = this->config.inactivityPeriod;
        const int minConnections = this->config.minConnections;

        int excessCounter=0;
        { //mutex scope
            QMutexLocker locker(&mutex);
            if(stat._nbAvailable) {
                qDebug("Starting checkConnectionPool, ConnectionPoolPrivate: pool size is now %d=%d", this->connectionPool.size(), stat._nbAvailable);
                QMutableListIterator<QSharedPointer<Connection>> connectionsIterator(this->connectionPool);
                while (connectionsIterator.hasNext()) { //they are all unused here
                    QSharedPointer<Connection>& connection = connectionsIterator.next();
                    if (++excessCounter > minConnections) {
                        if ((now - connection->getLastUseTime()) > inactivityPeriod) {
                            qDebug("Removing inactive connection id="); //show ID
                            connection.reset(); //remove the custom deleter
                            this->connectionPool.removeOne(connection);
                        }
                        break;
                    } else if ((now - connection->getCreationTime()) > connectionLifePeriod) {
                        connection->refresh();
                    }
                }
                stat._nbAvailable = this->connectionPool.size(); //maj nb available

                qDebug("End checkConnectionPool, ConnectionPoolPrivate: Unused Connection removed, pool size is now %d=%d", connectionPool.size(),stat._nbAvailable);
                if(!stop && stat._nbAvailable>0){
                    qDebug("Rescheduling timer for =%d",this->config.checkInterval);
                    this->checkTimer.start(this->config.checkInterval);
                }
            }
        }

    }



}
