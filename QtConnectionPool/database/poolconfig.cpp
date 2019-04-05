#include <QFile>
#include <QJsonObject>

#include "poolconfig.h"

PoolConfig::PoolConfig(const QString& configFilePath)
{
    QJsonDocument jsonConfig = readConfigFile(configFilePath);
    readJsonConfig(jsonConfig);
}

QJsonDocument PoolConfig::readConfigFile(const QString& configFilePath)
{
    QJsonDocument jsonConfig;

    QFile file(configFilePath);
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning("PoolConfig: Config file '%s' cannot be opened", qPrintable(configFilePath));
        jsonConfig = QJsonDocument();
    }
    else {
        jsonConfig = QJsonDocument::fromJson(file.readAll());
    }
    file.close();

    return jsonConfig;
}


void PoolConfig::readJsonConfig(const QJsonDocument &jsonConfig)
{
    QVariantMap configMap = jsonConfig.object().toVariantMap();
    QVariantMap connectionPoolConfig = configMap.value("connectionPool", QVariantMap()).toMap();

    this->checkInterval = connectionPoolConfig.value("checkInterval", 10000).toInt();
    this->minConnections = connectionPoolConfig.value("minConnections", 1).toInt();
    this->maxConnections = connectionPoolConfig.value("maxConnections", 3).toInt();
    this->connectionLifePeriod = connectionPoolConfig.value("connectionLifePeriod", 300000).toInt();
    this->dbConfig = DatabaseConfig(connectionPoolConfig.value("database").toMap());
}
