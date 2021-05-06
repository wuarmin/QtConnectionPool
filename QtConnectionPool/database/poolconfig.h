#ifndef POOLCONFIG_H
#define POOLCONFIG_H

#include <QSettings>
#include <QJsonDocument>
#include <QString>

#include "databaseconfig.h"

namespace QtConnectionPool {
    class PoolConfig {
    public:
        int checkInterval;
        int minConnections;
        int maxConnections;
        int connectionLifePeriod;
        int inactivityPeriod;
        DatabaseConfig dbConfig;

    public:
        explicit PoolConfig(const QString& configFilePath);

    private:
        QJsonDocument readConfigFile(const QString& configFilePath);

        void readJsonConfig(const QJsonDocument& jsonConfig);
    };
}

#endif // POOLCONFIG_H
