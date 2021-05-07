#ifndef QTCONNECTIONPOOL_POOLSTATS_H
#define QTCONNECTIONPOOL_POOLSTATS_H

namespace QtConnectionPool {
    struct PoolStats {
        uint16_t _nbAvailable;
        uint16_t _nbBorrowed;
        PoolStats()
                : _nbAvailable(0)
                , _nbBorrowed(0)
        {}
    };
}

#endif //QTCONNECTIONPOOL_POOLSTATS_H
