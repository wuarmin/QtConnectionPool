#ifndef ASYNCCONNECTIONUSER_H
#define ASYNCCONNECTIONUSER_H

#include <QRunnable>

#include "database/connection.h"

namespace QtConnectionPool {
    class AsyncConnectionUser : public QRunnable {
    private:
        QString name;
        QString result;
        bool success;
        Connection dbConnection;
        uint64_t waitTimeoutMS;

    public:
        explicit AsyncConnectionUser(const QString &name, uint64_t waitTimeoutMS = 0);

        void run();

        inline QString getResult() { return this->result; }

        inline bool getSuccess() { return this->success; }
    };
}

#endif // ASYNCCONNECTIONUSER_H
