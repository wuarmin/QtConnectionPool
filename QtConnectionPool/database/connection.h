#ifndef CONNECTION_H
#define CONNECTION_H

#include "connectionprivate.h"
#include "databaseconfig.h"

class Connection
{

private:
    ConnectionPrivate* databaseConnection;

public:
    Connection();
    Connection(const DatabaseConfig& config);
    Connection(const Connection& other);
    ~Connection();

    Connection& operator=(const Connection& other);
    bool operator==(const Connection& other);

    QSqlDatabase database();
    void use();
    bool isInUse() const;
    qint64 getCreationTime() const;
    qint64 getLastUseTime() const;
    void refresh();
};

#endif // CONNECTION_H
