#include "connectionpool.h"

ConnectionPoolPrivate* ConnectionPool::pool = 0;

ConnectionPool::ConnectionPool(const QString& configFilePath)
{
    if (!pool) {
        pool = new ConnectionPoolPrivate(configFilePath, 0);
    }
}

ConnectionPool::ConnectionPool()
{
    if (!pool) {
        qFatal("ConnectionPool: pool was not initialized");
    }
}

void ConnectionPool::destroy()
{
    if (pool) {
        delete pool;
    }
}

Connection ConnectionPool::getConnection()
{
    return pool->getConnection();
}
