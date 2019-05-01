#ifndef CONNECTIONPRIVATE_H
#define CONNECTIONPRIVATE_H

#include <QSqlDatabase>

#include "databaseconfig.h"

class ConnectionPrivate
{
    Q_DISABLE_COPY(ConnectionPrivate)

private:
    int refCount;
    bool inUse;
    QString dbId;
    qint64 creationTime;
    QSqlDatabase db;

public:
    ConnectionPrivate(const DatabaseConfig &config);
    ~ConnectionPrivate();

    QSqlDatabase& database();
    qint64 getCreationTime() const;
    void refresh();
    void use();
    void unUse();
    bool isInUse() const;
    int getRefCount();
    void incrementRefCount();
    void decrementRefCount();
};

#endif // CONNECTIONPRIVATE_H
