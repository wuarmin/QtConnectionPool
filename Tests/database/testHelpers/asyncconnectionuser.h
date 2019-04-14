#ifndef ASYNCCONNECTIONUSER_H
#define ASYNCCONNECTIONUSER_H

#include <QRunnable>

#include "database/databaseconnection.h"

class AsyncConnectionUser : public QRunnable
{
private:
    QString name;
    QString result;
    bool success;
    DatabaseConnection* dbConnection;

public:
    AsyncConnectionUser(const QString& name);
    ~AsyncConnectionUser();

    void run();

    inline QString getResult() { return this->result; }
    inline bool getSuccess() { return this->success; }
};

#endif // ASYNCCONNECTIONUSER_H
