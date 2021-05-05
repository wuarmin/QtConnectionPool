#ifndef POOLCONFIG_H
#define POOLCONFIG_H

#include <QSettings>
#include <QJsonDocument>

#include "databaseconfig.h"

class PoolConfig
{
public:
    int checkInterval;
    int minConnections;
    int maxConnections;
    int connectionLifePeriod;
    int inactivityPeriod;
    DatabaseConfig dbConfig;

public:
    PoolConfig(const QString &configFilePath);

private:
    QJsonDocument readConfigFile(const QString& configFilePath);
    void readJsonConfig(const QJsonDocument &jsonConfig);
};

#endif // POOLCONFIG_H
