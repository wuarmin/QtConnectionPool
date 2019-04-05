#include "database/databaseconnectiontest.h"
#include "database/poolconfigtest.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);

    int status = 0;
    status |= QTest::qExec(new DatabaseConnectionTest(), argc, argv);
    status |= QTest::qExec(new PoolConfigTest(), argc, argv);

    return 0;
}
