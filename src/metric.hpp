#include <atomic>
#include <iostream>

#pragma once

class Metric {
    private:
        static Metric self;
    public:
        std::atomic_llong recived;
        std::atomic_llong processed;
        std::atomic_llong pushed;
        static Metric& getMetric() 
        {
            return self;
        }
        void dump() 
        {
            std::cerr<<std::endl<<"Stats ----------------------------"<<std::endl;
            std::cerr<<"Number of metric recived"<<std::endl;
            std::cerr<<"Number of metric processed"<<std::endl;
            std::cerr<<"Number of metric pushed"<<std::endl;
            std::cerr<<std::endl<<"Stats END ------------------------"<<std::endl;
        }
};
