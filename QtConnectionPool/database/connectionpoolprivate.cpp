#include <QDateTime>
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

Connection ConnectionPoolPrivate::getConnection()
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
    return Connection();
}

Connection ConnectionPoolPrivate::createNewConnection()
{
    return Connection(this->config.dbConfig);
}

void ConnectionPoolPrivate::checkConnectionPool()
{
    qDebug("ConnectionPoolPrivate: pool size is now %i", connectionPool.size());
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    const qint64 connectionLifePeriod = this->config.connectionLifePeriod;
    const int maxConnections = this->config.maxConnections;
    int excessCounter = 0;
    this->mutex.lock();

    QMutableListIterator<Connection> connectionsIterator(this->connectionPool);
    while (connectionsIterator.hasNext()) {
        Connection& connection = connectionsIterator.next();
        if (!connection.isInUse()) {
            if (++excessCounter > maxConnections) {
                connectionPool.removeOne(connection);
                qDebug("ConnectionPoolPrivate: Unused Connection removed, pool size is now %i", connectionPool.size());
                break;
            }
            else if ((now-connection.getCreationTime()) > connectionLifePeriod) {
                connection.refresh();
            }
        }
    }
    this->mutex.unlock();
}
