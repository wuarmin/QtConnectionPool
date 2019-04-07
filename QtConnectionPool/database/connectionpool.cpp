#include "connectionpool.h"

DatabaseConnectionPool* ConnectionPool::pool = 0;

ConnectionPool::ConnectionPool(const QString& configFilePath)
{
    if (!pool) {
        pool = new DatabaseConnectionPool(configFilePath, 0);
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

DatabaseConnection* ConnectionPool::getConnection()
{
    return pool->getConnection();
}
