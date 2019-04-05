#ifndef DATABASECONNECTIONPOOL_H
#define DATABASECONNECTIONPOOL_H

#include <QObject>
#include <QTimer>
#include <QMutex>

#include "databaseconnection.h"
#include "poolconfig.h"

class DatabaseConnectionPool : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DatabaseConnectionPool)
private:
    const PoolConfig& config;
    QTimer checkTimer;
    QMutex mutex;
    QList<DatabaseConnection*> pool;

public:
    explicit DatabaseConnectionPool(const PoolConfig &config, QObject *parent);
    virtual ~DatabaseConnectionPool();

    DatabaseConnection* getConnection();

private:
    void initPool();
    DatabaseConnection* createNewConnection();

private slots:
    void checkPool();
};

#endif // DATABASECONNECTIONPOOL_H
