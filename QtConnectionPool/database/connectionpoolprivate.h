#ifndef CONNECTIONPOOLPRIVATE_H
#define CONNECTIONPOOLPRIVATE_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QSharedPointer>

#include "connection.h"
#include "poolconfig.h"
#include "poolStats.h"

namespace QtConnectionPool {
    class ConnectionPoolPrivate : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(ConnectionPoolPrivate)

    private:
        PoolStats stat;
        const PoolConfig config;
        QTimer checkTimer;
        mutable QMutex mutex;
        QList<QSharedPointer<Connection>> connectionPool; //available

    public:
        bool stop;
        QSharedPointer<Connection> getConnection(uint64_t waitTimeoutInMs = 0);
        void unBorrowConnection(QSharedPointer<Connection> con);
        PoolStats getPoolStats() const;

        static ConnectionPoolPrivate& getInstance();
        static void setupPool(const PoolConfig &config, QObject *parent);

    private:
        explicit ConnectionPoolPrivate(const PoolConfig &config, QObject *parent);
        void initPool();
        QSharedPointer<Connection> createNewConnection();

    private slots:
        void checkConnectionPool();
    };
}

#endif // CONNECTIONPOOLPRIVATE_H
