#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include "connectionpoolprivate.h"

class ConnectionPool
{
private:
    static ConnectionPoolPrivate* pool;
    ConnectionPoolPrivate* poolRef;

public:
    ConnectionPool();
    ConnectionPool(const QString& configFilePath);

    Connection getConnection();
    void destroy();
};

#endif // CONNECTIONPOOL_H
