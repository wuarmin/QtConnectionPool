#include <QThread>
#include <QSqlQuery>
#include <QVariant>

#include "asyncconnectionuser.h"
#include "database/connectionpool.h"
#include "helperfunctions.h"

namespace QtConnectionPool {
    AsyncConnectionUser::AsyncConnectionUser(const QString& name, uint64_t waitTimeoutMS)
    : QRunnable()
    , name(name)
    , result()
    , success(false)
    , dbConnection(nullptr)
    , waitTimeoutMS(waitTimeoutMS)
    {
        this->setAutoDelete(false);
    }

    void AsyncConnectionUser::run() {
        this->dbConnection = ConnectionPool().getConnection(waitTimeoutMS);
        if (!this->dbConnection || !this->dbConnection->database().isOpen()) {
            this->result = QString("%1: failed")
                    .arg(this->name);
            ConnectionPool().unBorrowConnection(this->dbConnection);
            return;
        }

        QSqlQuery query(this->dbConnection->database());
        const QString queryString = HelperFunctions::getTestSqlQueryForDriver(query.driver());
        this->success = query.exec(queryString);

        query.next();
        QString queryResult = query.value(0).toString();
        this->result = QString("%1: %2")
                .arg(this->name)
                .arg(queryResult);
        ConnectionPool().unBorrowConnection(this->dbConnection);
        //this->dbConnection = nullptr;
    }
}
