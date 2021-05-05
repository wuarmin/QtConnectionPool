#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>

namespace QtConnectionPool {
    class ConnectionPrivate;
    class DatabaseConfig;

    class Connection
    {

    private:
        ConnectionPrivate* databaseConnection;

    public:
        Connection();
        explicit Connection(const DatabaseConfig& config);
        Connection(const Connection& other);
        ~Connection();

        Connection& operator=(const Connection& other);
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
