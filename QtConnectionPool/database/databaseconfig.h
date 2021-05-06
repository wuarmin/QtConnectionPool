#ifndef DATABASECONFIG_H
#define DATABASECONFIG_H

#include <QString>
#include <QVariantMap>

namespace QtConnectionPool {
    class DatabaseConfig {
    public:
        QString driver;
        QString host;
        int port;
        QString database;
        QString user;
        QString password;

    public:
        DatabaseConfig();

        explicit DatabaseConfig(const QVariantMap& configMap);

    private:
        void readConfig(const QVariantMap &configMap);
    };
}

#endif // DATABASECONFIG_H
