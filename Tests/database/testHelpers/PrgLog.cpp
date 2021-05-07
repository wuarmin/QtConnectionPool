#include "PrgLog.h"
#include <QThread>

#include <iostream>

namespace QtConnectionPool {
    void PrgLog::qtMessageOutput(QtMsgType msgtype, const QMessageLogContext &context, const QString &msg) {
        Q_UNUSED(msgtype);
        Q_UNUSED(context);

        //to console
        std::cerr << QThread::currentThreadId() << " : " << msg.toStdString().c_str() << std::endl;
    }
}