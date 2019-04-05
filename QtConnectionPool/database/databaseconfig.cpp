#include "databaseconfig.h"

DatabaseConfig::DatabaseConfig()
{
}

DatabaseConfig::DatabaseConfig(const QVariantMap &configMap)
{
    this->readConfig(configMap);
}

void DatabaseConfig::readConfig(const QVariantMap &configMap)
{
    this->driver = configMap.value("driver", "QPSQL").toString();
    this->host = configMap.value("host").toString();
    this->port = configMap.value("port", 5432).toInt();
    this->database = configMap.value("database").toString();
    this->user = configMap.value("user").toString();
    this->password = configMap.value("password").toString();
}
