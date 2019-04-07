#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include "databaseconnectionpool.h"

class ConnectionPool
{
private:
    static DatabaseConnectionPool* pool;
    DatabaseConnectionPool* poolRef;

public:
    ConnectionPool();
    ConnectionPool(const QString& configFilePath);

    void destroy();
    DatabaseConnection* getConnection();
};

#endif // CONNECTIONPOOL_H
