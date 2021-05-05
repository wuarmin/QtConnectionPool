#include "connectionpool.h"

ConnectionPoolPrivate* ConnectionPool::pool = 0;

ConnectionPool::ConnectionPool(const QString& configFilePath)
: ConnectionPool(PoolConfig(configFilePath))
{}

ConnectionPool::ConnectionPool(const PoolConfig& poolConfig)
{
    if (!pool) {
        pool = new ConnectionPoolPrivate(poolConfig, nullptr);
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

Connection ConnectionPool::getConnection(uint64_t waitTimeoutInMs)
{
    return pool->getConnection(waitTimeoutInMs);
}
