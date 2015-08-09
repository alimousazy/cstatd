#include <atomic>
#include <iostream>

#pragma once

class Heartbeat {
    public:
        static std::atomic_llong recived;
        static std::atomic_llong processed;
        static std::atomic_llong pushed;
        static void dump();
};
