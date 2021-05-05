#include <QDateTime>
#include <QThread>
#include <QMutableListIterator>

#include "connectionpoolprivate.h"

ConnectionPoolPrivate::ConnectionPoolPrivate(const PoolConfig &config, QObject* parent)
    : QObject(parent),
      config(config)
{
    this->initPool();

    this->checkTimer.start(this->config.checkInterval);
    connect(&checkTimer, SIGNAL(timeout()), SLOT(checkConnectionPool()));
}

void ConnectionPoolPrivate::initPool()
{
    const int minConnections =  this->config.minConnections;

    for (int i = 0; i < minConnections; ++i) {
        this->connectionPool.append(this->createNewConnection());
    }
}

Connection ConnectionPoolPrivate::getConnection(uint64_t waitTimeoutInMs)
{
    this->mutex.lock();

    QMutableListIterator<Connection> connectionsIterator(this->connectionPool);
    while (connectionsIterator.hasNext()) {
        Connection& connection = connectionsIterator.next();
        if (!connection.isInUse()) {
            connection.use();
            this->mutex.unlock();
            return connection;
        }
    }

    if (this->connectionPool.count() < this->config.maxConnections) {
        Connection newConnection = this->createNewConnection();
        this->connectionPool.append(newConnection);
        newConnection.use();
        this->mutex.unlock();
        return newConnection;
    }
    this->mutex.unlock();

    //else we need to wait a thread has finish to have a working connection
    if(waitTimeoutInMs > 0){
        const qint64 startWait = QDateTime::currentMSecsSinceEpoch();
        while(1){
            auto con = getConnection(0);
            if(con.isValid()){
                return con;
            }
            if( (QDateTime::currentMSecsSinceEpoch() - startWait) > waitTimeoutInMs ){ //timeout expired
                break;
            }
            QThread::yieldCurrentThread();
        }
    }

    return Connection();
}

Connection ConnectionPoolPrivate::createNewConnection()
{
    return Connection(this->config.dbConfig);
}

void ConnectionPoolPrivate::checkConnectionPool()
{
    qDebug("Starting checkConnectionPool, ConnectionPoolPrivate: pool size is now %i", connectionPool.size());
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    const qint64 connectionLifePeriod = this->config.connectionLifePeriod;
    const qint64 inactivityPeriod = this->config.inactivityPeriod;
    const int minConnections = this->config.minConnections;
    
    int excessCounter = 0;
    this->mutex.lock();

    QMutableListIterator<Connection> connectionsIterator(this->connectionPool);
    while (connectionsIterator.hasNext()) {
        Connection& connection = connectionsIterator.next();
        if (!connection.isInUse()) {
            if (++excessCounter > minConnections) {
                if( (now-connection.getLastUseTime()) > inactivityPeriod) {
                    qDebug("Removing inactive connection id="); //show ID
                    connectionPool.removeOne(connection);
                }
                break;
            }
            else if ((now-connection.getCreationTime()) > connectionLifePeriod) {
                connection.refresh();
            }
        }
    }
    qDebug("End checkConnectionPool, ConnectionPoolPrivate: Unused Connection removed, pool size is now %i", connectionPool.size());
    this->mutex.unlock();
}
