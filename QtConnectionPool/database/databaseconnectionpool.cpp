#include <QDateTime>
#include <QDebug>

#include "databaseconnectionpool.h"

DatabaseConnectionPool::DatabaseConnectionPool(const PoolConfig &config, QObject* parent)
    : QObject(parent),
      config(config)
{
    this->checkTimer.start(this->config.checkInterval);
    connect(&checkTimer, SIGNAL(timeout()), SLOT(checkPool()));

    this->initPool();
}

DatabaseConnectionPool::~DatabaseConnectionPool()
{
    foreach(DatabaseConnection* connection, pool) {
        delete connection;
    }
}

void DatabaseConnectionPool::initPool()
{
    const int minConnections =  this->config.minConnections;

    for(int i = 0; i < minConnections; ++i) {
        this->createNewConnection();
    }
}

DatabaseConnection* DatabaseConnectionPool::getConnection()
{
    DatabaseConnection* freshConnection = 0;
    this->mutex.lock();

    foreach(DatabaseConnection* connection, this->pool) {
        if(!connection->isInUse()) {
            freshConnection = connection;
            freshConnection->setInUse();
            break;
        }
    }

    if(!freshConnection) {
        const int maxConnections = this->config.maxConnections;
        if(this->pool.count() < maxConnections) {
            freshConnection = this->createNewConnection();
            freshConnection->setInUse();
        }
    }

    this->mutex.unlock();
    return freshConnection;
}

DatabaseConnection* DatabaseConnectionPool::createNewConnection()
{
    DatabaseConnection* freshConnection = new DatabaseConnection(this->config.dbConfig);
    pool.append(freshConnection);
    return freshConnection;
}

void DatabaseConnectionPool::checkPool()
{
    qDebug("DatabaseConnectionPool: pool size is now %i", pool.size());
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    const qint64 connectionLifePeriod = this->config.connectionLifePeriod;
    const int maxConnections = this->config.maxConnections;
    int excessCounter = 0;
    this->mutex.lock();
    foreach(DatabaseConnection* connection, pool) {
        if(!connection->isInUse()) {
            if(++excessCounter > maxConnections) {
                pool.removeOne(connection);
                delete connection;
                qDebug("DatabaseConnectionPool: Unused Connection removed, pool size is now %i", pool.size());
                break;
            }
            else if((now-connection->getCreationTime()) > connectionLifePeriod) {
                connection->refresh();
            }
        }
    }
    this->mutex.unlock();
}
