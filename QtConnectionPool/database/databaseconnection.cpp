#include <QUuid>
#include <QDateTime>

#include "databaseconnection.h"

DatabaseConnection::DatabaseConnection(const QSettings& settings)
    : inUse(false),
      dbId(QUuid::createUuid().toString()),
      creationTime(QDateTime::currentMSecsSinceEpoch())
{    
    this->db = QSqlDatabase::addDatabase("QPSQL", dbId);

    this->db.setHostName(settings.value("host").toString());
    this->db.setDatabaseName(settings.value("dwh").toString());
    this->db.setUserName(settings.value("user").toString());
    this->db.setPassword(settings.value("passwd").toString());

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
}

qint64 DatabaseConnection::getCreationTime()
{
    return this->creationTime;
}

bool DatabaseConnection::isInUse()
{
    return this->inUse;
}

void DatabaseConnection::setInUse()
{
    this->inUse = true;
}

void DatabaseConnection::setUnUsed()
{
    this->inUse = false;
}
