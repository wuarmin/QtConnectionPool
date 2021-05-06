#include "connection.h"

#include <QDateTime>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QThread>
#include <QUuid>

#include "databaseconfig.h"

namespace QtConnectionPool {
    class ConnectionPrivate {
        Q_DISABLE_COPY(ConnectionPrivate)

    private:
        bool inUse;
        bool valid;
        QString dbId;
        qint64 creationTime;
        qint64 lastUseTime;
        QSqlDatabase db;

    public:
        explicit ConnectionPrivate(const DatabaseConfig& config);

        QSqlDatabase &database();

        qint64 getCreationTime() const;

        qint64 getLastUseTime() const;

        void refresh();

        void use();

        void unUse();

        bool isInUse() const;

        bool isValid() const;
    };

    ConnectionPrivate::ConnectionPrivate(const DatabaseConfig& config)
    : inUse(false)
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

        valid = true;
        this->refresh();
    }

    QSqlDatabase& ConnectionPrivate::database() {
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
            //qDebug("ConnectionPrivate::refresh opened db successfully");
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

    void ConnectionPrivate::use() {
        //qDebug("ConnectionPrivate reserved by threadID=%p",QThread::currentThreadId());
        this->inUse = true;
        this->lastUseTime = QDateTime::currentMSecsSinceEpoch();
    }

    void ConnectionPrivate::unUse() {
        //qDebug("ConnectionPrivate release by threadID=%p",QThread::currentThreadId());
        this->inUse = false;
    }

    bool ConnectionPrivate::isInUse() const {
        return this->inUse;
    }

    bool ConnectionPrivate::isValid() const {
        return this->valid;
    }


    Connection::Connection()
    : databaseConnection(0) {
    }

    Connection::Connection(const DatabaseConfig& config)
    : databaseConnection(new ConnectionPrivate(config)) {
        qDebug("new databaseConnection");
    }

    bool Connection::operator==(const Connection& other) {
        return this->databaseConnection == other.databaseConnection;
    }

    QSqlDatabase Connection::database() {
        if (!this->databaseConnection) {
            return QSqlDatabase();
        }
        return this->databaseConnection->database();
    }

    void Connection::use() {
        if (!this->databaseConnection) {
            return;
        }
        this->databaseConnection->use();
    }

    bool Connection::isInUse() const {
        if (!this->databaseConnection) {
            return false;
        }

        return databaseConnection->isInUse();
    }

    bool Connection::isValid() const {
        if (!this->databaseConnection) {
            return false;
        }

        return databaseConnection->isValid();
    }

    void Connection::refresh() {
        if (!this->databaseConnection) {
            return;
        }

        return databaseConnection->refresh();
    }

    qint64 Connection::getCreationTime() const {
        if (!this->databaseConnection) {
            return 0;
        }

        return databaseConnection->getCreationTime();
    }

    qint64 Connection::getLastUseTime() const {
        if (!this->databaseConnection) {
            return 0;
        }

        return databaseConnection->getLastUseTime();
    }
}