#ifndef ASYNCCONNECTIONUSER_H
#define ASYNCCONNECTIONUSER_H

#include <QRunnable>
#include <QSharedPointer>

#include "database/connection.h"

namespace QtConnectionPool {
    class AsyncConnectionUser : public QRunnable {
    private:
        QString name;
        QString result;
        bool success;
        QSharedPointer<Connection> dbConnection;
        uint64_t waitTimeoutMS;
        uint64_t queryExecTimeMs;

    public:
        explicit AsyncConnectionUser(const QString& name, uint64_t waitTimeoutMS = 0, uint64_t queryExecTimeMs = 0);

        void run();

        inline QString getResult() { return this->result; }

        inline bool getSuccess() { return this->success; }
    };
}

#endif // ASYNCCONNECTIONUSER_H
