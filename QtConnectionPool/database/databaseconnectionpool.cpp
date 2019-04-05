#include <QDateTime>

#include "databaseconnectionpool.h"

DatabaseConnectionPool::DatabaseConnectionPool(const QSettings& settings, QObject* parent)
    : QObject(parent),
      settings(settings)
{
    this->cleanUpTimer.start(this->settings.value("cleanUpInterval", 10000).toInt());
    connect(&cleanUpTimer, SIGNAL(timeout()), SLOT(checkPool()));

    this->initPool();
}

DatabaseConnectionPool::~DatabaseConnectionPool()
{
    foreach(DatabaseConnection* connection, pool) {
        delete connection;
    }
}

void DatabaseConnectionPool::initPool()
{
    const int minConnections =  this->settings.value("minConnections", 3).toInt();

    for(int i = 0; i < minConnections; ++i) {
        this->createNewConnection();
    }
}

DatabaseConnection* DatabaseConnectionPool::getConnection()
{
    DatabaseConnection* freshConnection = 0;
    this->mutex.lock();

    foreach(DatabaseConnection* connection, this->pool) {
        if(!connection->isInUse()) {
            freshConnection = connection;
            freshConnection->setInUse();
            break;
        }
    }

    if(!freshConnection) {
        const int maxConnections = this->settings.value("maxConnections", 5).toInt();
        if(this->pool.count() < maxConnections) {
            freshConnection = this->createNewConnection();
            freshConnection->setInUse();
        }
    }

    this->mutex.unlock();
    return freshConnection;
}

DatabaseConnection* DatabaseConnectionPool::createNewConnection()
{
    DatabaseConnection* freshConnection = new DatabaseConnection(this->settings);
    pool.append(freshConnection);
    return freshConnection;
}

void DatabaseConnectionPool::checkPool()
{
    qDebug("DatabaseConnectionPool: pool size is now %i", pool.size());
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    const qint64 connectionLifePeriod = this->settings.value("connectionLifePeriod", 1).toInt();
    const int maxConnections = this->settings.value("maxConnections", 5).toInt();
    int excessCounter = 0;
    this->mutex.lock();
    foreach(DatabaseConnection* connection, pool) {
        if(!connection->isInUse()) {
            if(++excessCounter > maxConnections) {
                pool.removeOne(connection);
                delete connection;
                qDebug("DatabaseConnectionPool: Unused Connection removed, pool size is now %i", pool.size());
                break;
            }
            else if((now-connection->getCreationTime()) > connectionLifePeriod) {
                connection->refresh();
            }
        }
    }
    this->mutex.unlock();
}
