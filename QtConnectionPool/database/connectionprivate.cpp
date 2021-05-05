#include <QUuid>
#include <QDateTime>
#include <QSqlError>

#include "connectionprivate.h"

namespace QtConnectionPool {
    ConnectionPrivate::ConnectionPrivate(const DatabaseConfig &config)
            : refCount(0)
            , inUse(false)
            , valid(false)
            , dbId(QUuid::createUuid().toString())
            , creationTime(QDateTime::currentMSecsSinceEpoch())
            , lastUseTime(0)
            , db()
    {
        this->db = QSqlDatabase::addDatabase(config.driver, dbId);
        this->db.setHostName(config.host);
        this->db.setPort(config.port);
        this->db.setDatabaseName(config.database);
        this->db.setUserName(config.user);
        this->db.setPassword(config.password);

        this->refresh();
    }

    ConnectionPrivate::~ConnectionPrivate() {
        this->db.close();
    }

    QSqlDatabase &ConnectionPrivate::database() {
        return this->db;
    }

    void ConnectionPrivate::refresh() {
        valid = false;
        if (this->db.isOpen()) {
            this->db.close();
        }
        this->db.open();

        if (this->db.isOpenError()) {
            qWarning("DatabaseConnection: Error at DatabaseConnection refresh(%s)",
                     qPrintable(this->db.lastError().text()));
        } else {
            valid = true;
            this->creationTime = QDateTime::currentMSecsSinceEpoch();
        }
    }

    qint64 ConnectionPrivate::getCreationTime() const {
        return this->creationTime;
    }

    qint64 ConnectionPrivate::getLastUseTime() const {
        return this->lastUseTime;
    }

    int ConnectionPrivate::getRefCount() {
        return this->refCount;
    }

    void ConnectionPrivate::use() {
        this->inUse = true;
        this->lastUseTime = QDateTime::currentMSecsSinceEpoch();
    }

    void ConnectionPrivate::unUse() {
        this->inUse = false;
    }

    bool ConnectionPrivate::isInUse() const {
        return this->inUse;
    }

    bool ConnectionPrivate::isValid() const {
        return this->valid;
    }

    void ConnectionPrivate::incrementRefCount() {
        ++this->refCount;
    }

    void ConnectionPrivate::decrementRefCount() {
        --this->refCount;
    }
}