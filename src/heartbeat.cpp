#include "heartbeat.hpp"

std::atomic_llong Heartbeat::recived;
std::atomic_llong Heartbeat::processed;
std::atomic_llong Heartbeat::pushed;
void Heartbeat::dump() 
{
		std::cerr<<std::endl<<"Stats ----------------------------"<<std::endl;
		std::cerr<<"Number of connection recived"<<Heartbeat::recived<<std::endl;
		std::cerr<<"Number of metric processed"<<Heartbeat::processed<<std::endl;
		std::cerr<<"Number of metric pushed"<<Heartbeat::pushed<<std::endl;
		std::cerr<<std::endl<<"Stats END ------------------------"<<std::endl;
}

