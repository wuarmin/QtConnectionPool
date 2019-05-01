#include "connection.h"

Connection::Connection()
    : databaseConnection(0)
{
}

Connection::Connection(const DatabaseConfig &config)
    : databaseConnection(new ConnectionPrivate(config))
{
}

Connection::Connection(const Connection& other)
    : databaseConnection(0)
{
    if (other.databaseConnection) {
        this->databaseConnection = other.databaseConnection;
        this->databaseConnection->incrementRefCount();
    }
}

Connection::~Connection()
{
    if (this->databaseConnection) {
        this->databaseConnection->decrementRefCount();

        const int refCount = this->databaseConnection->getRefCount();

        if (refCount == 0) {
            this->databaseConnection->unUse();
        }
        else if (refCount < 0) {
            delete this->databaseConnection;
        }
    }
}

Connection& Connection::operator=(const Connection& other)
{
    if (other.databaseConnection) {
        if (this->databaseConnection) {
            this->databaseConnection->decrementRefCount();
        }
        this->databaseConnection = other.databaseConnection;
        this->databaseConnection->incrementRefCount();
    }

    return *this;
}

bool Connection::operator==(const Connection& other)
{
    return this->databaseConnection == other.databaseConnection;
}

QSqlDatabase Connection::database()
{
    if (!this->databaseConnection) {
        return QSqlDatabase();
    }

    return this->databaseConnection->database();
}

void Connection::use()
{
    this->databaseConnection->use();
}

bool Connection::isInUse() const
{
    if (!this->databaseConnection) {
        return false;
    }

    return databaseConnection->isInUse();
}

void Connection::refresh()
{
    if (!this->databaseConnection) {
        return;
    }

    return databaseConnection->refresh();
}

qint64 Connection::getCreationTime() const
{
    if (!this->databaseConnection) {
        return 0;
    }

    return databaseConnection->getCreationTime();
}
