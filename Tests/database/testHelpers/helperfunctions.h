#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <QString>
#include <QSqlDriver>

namespace QtConnectionPool {
    namespace HelperFunctions {
        QString getTestSqlQueryForDriver(const QSqlDriver* driver);
    }
}
#endif // HELPERFUNCTIONS_H
