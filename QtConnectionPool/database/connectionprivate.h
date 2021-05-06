#ifndef CONNECTIONPRIVATE_H
#define CONNECTIONPRIVATE_H

#include <QSqlDatabase>

#include "databaseconfig.h"

namespace QtConnectionPool {
    class ConnectionPrivate {
        Q_DISABLE_COPY(ConnectionPrivate)

    private:
        int refCount;
        bool inUse;
        bool valid;
        QString dbId;
        qint64 creationTime;
        qint64 lastUseTime;
        QSqlDatabase db;

    public:
        explicit ConnectionPrivate(const DatabaseConfig& config);

        ~ConnectionPrivate();

        QSqlDatabase &database();

        qint64 getCreationTime() const;

        qint64 getLastUseTime() const;

        void refresh();

        void use();

        void unUse();

        bool isInUse() const;

        bool isValid() const;

        int getRefCount();

        void incrementRefCount();

        void decrementRefCount();
    };
}

#endif // CONNECTIONPRIVATE_H
