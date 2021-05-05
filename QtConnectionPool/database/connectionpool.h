#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include "connection.h"

namespace QtConnectionPool {
    class PoolConfig;

    class ConnectionPoolPrivate;

    class ConnectionPool {
    private:
        static ConnectionPoolPrivate *pool;

    public:
        ConnectionPool();

        explicit ConnectionPool(const QString &configFilePath);

        explicit ConnectionPool(const PoolConfig &poolConfig);

        Connection getConnection(uint64_t waitTimeoutInMs = 0);

        int getNbCon() const;

        void destroy();
    };
}

#endif // CONNECTIONPOOL_H
