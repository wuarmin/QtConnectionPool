#ifndef QTCONNECTIONPOOL_PRGLOG_H
#define QTCONNECTIONPOOL_PRGLOG_H

#include <QtMsgHandler>
#include <QString>

namespace QtConnectionPool {
    namespace PrgLog {
        void qtMessageOutput(QtMsgType msgtype, const QMessageLogContext &context, const QString &msg);
    }
}

#endif //QTCONNECTIONPOOL_PRGLOG_H
