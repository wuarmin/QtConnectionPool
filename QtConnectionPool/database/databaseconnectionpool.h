#ifndef DATABASECONNECTIONPOOL_H
#define DATABASECONNECTIONPOOL_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QSettings>

#include "databaseconnection.h"

class DatabaseConnectionPool : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DatabaseConnectionPool)
private:
    const QSettings& settings;
    QTimer cleanUpTimer;
    QMutex mutex;
    QList<DatabaseConnection*> pool;

public:
    explicit DatabaseConnectionPool(const QSettings &settings, QObject *parent);
    virtual ~DatabaseConnectionPool();

    DatabaseConnection* getConnection();

private:
    void initPool();
    DatabaseConnection* createNewConnection();

private slots:
    void checkPool();
};

#endif // DATABASECONNECTIONPOOL_H
