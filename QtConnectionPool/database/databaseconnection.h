#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QSqlDatabase>
#include <QSettings>

class DatabaseConnection
{
    Q_DISABLE_COPY(DatabaseConnection)

private:
    bool inUse;
    QString dbId;
    qint64 creationTime;
    QSqlDatabase db;

public:
    DatabaseConnection(const QSettings &settings);
    ~DatabaseConnection();

    QSqlDatabase& database();
    qint64 getCreationTime();
    void refresh();
    bool isInUse();
    void setInUse();
    void setUnUsed();
};

#endif // DATABASECONNECTION_H
