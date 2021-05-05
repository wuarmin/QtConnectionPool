#include <QUuid>
#include <QDateTime>
#include <QSqlError>

#include "databaseconnection.h"

DatabaseConnection::DatabaseConnection(const DatabaseConfig& config)
    : inUse(false)
    , dbId(QUuid::createUuid().toString())
    , creationTime(QDateTime::currentMSecsSinceEpoch())
    , lastUseTime(0)
{    
    this->db = QSqlDatabase::addDatabase(config.driver, dbId);
    this->db.setHostName(config.host);
    this->db.setPort(config.port);
    this->db.setDatabaseName(config.database);
    this->db.setUserName(config.user);
    this->db.setPassword(config.password);

    this->refresh();
}

DatabaseConnection::~DatabaseConnection()
{
    this->db.close();
}

QSqlDatabase &DatabaseConnection::database()
{
    return this->db;
}

void DatabaseConnection::refresh()
{
    if(this->db.isOpen()) {
        this->db.close();
    }
    this->db.open();

    if(this->db.isOpenError()) {
        qWarning("DatabaseConnection: Error at DatabaseConnection refresh(%s)", qPrintable(this->db.lastError().text()));
    }
    else {
        this->creationTime = QDateTime::currentMSecsSinceEpoch();
    }
}

qint64 DatabaseConnection::getCreationTime() const
{
    return this->creationTime;
}

qint64 DatabaseConnection::getLastUseTime() const
{
    return this->lastUseTime;
}

bool DatabaseConnection::isInUse() const
{
    return this->inUse;
}

void DatabaseConnection::setInUse()
{
    this->inUse = true;
    this->lastUseTime = QDateTime::currentMSecsSinceEpoch();
}

void DatabaseConnection::setUnUsed()
{
    this->inUse = false;
}
