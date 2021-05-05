#include <QThread>
#include <QSqlQuery>
#include <QVariant>

#include "asyncconnectionuser.h"
#include "database/connectionpool.h"

namespace QtConnectionPool {
    AsyncConnectionUser::AsyncConnectionUser(const QString &name, uint64_t waitTimeoutMS)
    : QRunnable()
    , name(name)
    , result()
    , success(false)
    , dbConnection()
    , waitTimeoutMS(waitTimeoutMS)
    {
        this->setAutoDelete(false);
    }

    void AsyncConnectionUser::run() {
        this->dbConnection = ConnectionPool().getConnection(waitTimeoutMS);
        if (!this->dbConnection.database().isOpen()) {
            this->result = QString("%1: failed")
                    .arg(this->name);
            return;
        }

        QSqlQuery query(this->dbConnection.database());
        this->success = query.exec("select 'it works' from dual");

        query.next();
        QString queryResult = query.value(0).toString();
        this->result = QString("%1: %2")
                .arg(this->name)
                .arg(queryResult);

    }
}
