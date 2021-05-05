#include <QThread>
#include <QSqlQuery>

#include "asyncconnectionuser.h"
#include "database/connectionpool.h"

AsyncConnectionUser::AsyncConnectionUser(const QString& name)
    : QRunnable(),
      name(name),
      success(false)
{
    this->setAutoDelete(false);
}

void AsyncConnectionUser::run()
{
    this->dbConnection = ConnectionPool().getConnection();
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
