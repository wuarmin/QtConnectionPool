
#include "helperfunctions.h"

namespace QtConnectionPool {
    namespace HelperFunctions {

        QString getTestSqlQueryForDriver(const QSqlDriver* driver)
        {
            switch(driver->dbmsType()) {
            case QSqlDriver::DbmsType::Oracle :
                return QString("select 'it works' from dual");
                break;
            case QSqlDriver::DbmsType::PostgreSQL :
                return QString("select 'it works'");
                break;
              default:
                throw std::runtime_error("untested driver");
            }
        }
    }
}
