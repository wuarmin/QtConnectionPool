#ifndef ASYNCCONNECTIONUSER_H
#define ASYNCCONNECTIONUSER_H

#include <QRunnable>

#include "database/connection.h"

class AsyncConnectionUser : public QRunnable
{
private:
    QString name;
    QString result;
    bool success;
    Connection dbConnection;

public:
    AsyncConnectionUser(const QString& name);

    void run();

    inline QString getResult() { return this->result; }
    inline bool getSuccess() { return this->success; }
};

#endif // ASYNCCONNECTIONUSER_H
