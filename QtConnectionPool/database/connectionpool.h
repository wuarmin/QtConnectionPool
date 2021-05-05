#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include "connectionpoolprivate.h"

class PoolConfig;

class ConnectionPool
{
private:
    static ConnectionPoolPrivate* pool;
    ConnectionPoolPrivate* poolRef;

public:
    ConnectionPool();
    explicit ConnectionPool(const QString& configFilePath);
    explicit ConnectionPool(const PoolConfig& poolConfig);

    Connection getConnection(uint64_t waitTimeoutInMs=0);
    void destroy();
};

#endif // CONNECTIONPOOL_H
