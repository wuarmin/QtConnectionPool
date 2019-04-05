#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QSqlDatabase>

#include "databaseconfig.h"

class DatabaseConnection
{
    Q_DISABLE_COPY(DatabaseConnection)

private:
    bool inUse;
    QString dbId;
    qint64 creationTime;
    QSqlDatabase db;

public:
    DatabaseConnection(const DatabaseConfig &config);
    ~DatabaseConnection();

    QSqlDatabase& database();
    qint64 getCreationTime() const;
    void refresh();
    bool isInUse() const;
    void setInUse();
    void setUnUsed();
};

#endif // DATABASECONNECTION_H
