#ifndef CONNECTIONPOOLPRIVATE_H
#define CONNECTIONPOOLPRIVATE_H

#include <QObject>
#include <QTimer>
#include <QMutex>

#include "connection.h"
#include "poolconfig.h"

class ConnectionPoolPrivate : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ConnectionPoolPrivate)

private:
    const PoolConfig config;
    QTimer checkTimer;
    QMutex mutex;
    QList<Connection> connectionPool;

public:
    explicit ConnectionPoolPrivate(const PoolConfig &config, QObject* parent);

    Connection getConnection(uint64_t waitTimeoutInMs=0);

private:
    void initPool();
    Connection createNewConnection();

private slots:
    void checkConnectionPool();
};

#endif // CONNECTIONPOOLPRIVATE_H
