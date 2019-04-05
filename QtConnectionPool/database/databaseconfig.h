#ifndef DATABASECONFIG_H
#define DATABASECONFIG_H

#include <QVariantMap>

class DatabaseConfig
{
public:
    QString driver;
    QString host;
    int port;
    QString database;
    QString user;
    QString password;

public:
    DatabaseConfig();
    DatabaseConfig(const QVariantMap& configMap);

private:
    void readConfig(const QVariantMap& configMap);
};

#endif // DATABASECONFIG_H
