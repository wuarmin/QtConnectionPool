#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSharedPointer>

namespace QtConnectionPool {
    class ConnectionPrivate;
    class DatabaseConfig;

    class Connection
    {

    private:
        QSharedPointer<ConnectionPrivate> databaseConnection;

    public:
        Connection();
        explicit Connection(const DatabaseConfig& config);

        bool operator==(const Connection& other);

        QSqlDatabase database();
        void use();
        bool isInUse() const;
        bool isValid() const;

        qint64 getCreationTime() const;
        qint64 getLastUseTime() const;
        void refresh();
    };
}


#endif // CONNECTION_H
