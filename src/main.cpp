#include <iostream>
#include <string>
#include <thread>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/thread/thread.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <chrono>
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include<time.h>

#include "scatter.hpp"
#include "server.hpp"
#include "data_access.hpp"

void snap_shot(data_access &dAccess) 
{
	lua_State *L;
	L = luaL_newstate();
	luaL_openlibs(L);
	int status = luaL_loadfile(L, "script.lua");
    if (status) {
        fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
        exit(1);
    }
    if (lua_pcall(L, 0, 0, 0))
	{
        fprintf(stderr, "Lua script has an issue: %s\n", lua_tostring(L, -1));
	}    
	while(true) 
	{
		
		auto start = std::chrono::high_resolution_clock::now();
		rocksdb::ReadOptions readOptions;
		readOptions.snapshot = dAccess.GetSnapshot();
		auto* snapshotIterator = dAccess.NewIterator(readOptions);
		Heartbeat::dump();
		for(snapshotIterator->SeekToFirst(); snapshotIterator->Valid(); snapshotIterator->Next()) 
		{
			auto  key = snapshotIterator->key().ToString();
			lua_getglobal(L, "process");
			lua_pushstring(L, key.c_str());
			lua_pushnumber(L, data_access::encoder::decode(snapshotIterator->value().ToString().c_str()));
			if (lua_pcall(L, 2, 0, 0) != 0)
			{
				fprintf(stderr, "error running function `process' : %s", lua_tostring(L, -1));
			}
			dAccess.delete_key(key);
		}
		dAccess.ReleaseSnapshot(readOptions.snapshot);
		delete snapshotIterator;
		/* should moved to function */
		time_t current;
		struct tm * ptm;
		time(&current);
		/*Not thread safe should be replaced with gmtime_r*/
    ptm = gmtime(&current);
		std::chrono::seconds dur(61 - ptm->tm_sec);
		std::this_thread::sleep_for(dur);
	}
}



void consumer_function(data_access &dAccess, scatter<std::string>::q_ptr ptr) 
{
    std::string metric[3];
    std::string str;
    while(true) 
    {
			ptr->wait_and_pop(str);
			Heartbeat::processed++;
			boost::tokenizer<> tok(str);
			int cont = 0;
			for(auto beg = tok.begin(); 
					beg != tok.end(); 
					cont++,
					++beg)
			{
				metric[cont] = *beg;
			}
			if(cont == 3)
			{
				try {
					std::string st; 
					st += metric[0] + "," + metric[1];
					time_t t = boost::lexical_cast<long>(metric[0]);
					long	 x = boost::lexical_cast<long>(metric[2]);
					dAccess.add(st.c_str(), x);
				} catch( boost::bad_lexical_cast const& ) {
					std::cout << "Error: input string was not valid" << std::endl;
				}
			}
			else
			{
				fprintf(stderr, "Invalid metric it should be in this format time,metric_name,value metric_name should not include any special charachter");
			}
    }
}

int main()
{
  try
  {
    data_access dAccess;
    boost::asio::io_service io_service;
    tcp_server server(io_service);
    boost::thread_group g;
    for(int i = 0; i <  std::thread::hardware_concurrency(); i++)
    {
        auto callback = std::bind(consumer_function, boost::ref(dAccess), server.getQueue());
				std::cout<<"num"<<i<<std::endl;
				std::cout.flush();
        g.add_thread(new boost::thread(callback));
    }
		auto callback = std::bind(snap_shot, boost::ref(dAccess));
		g.add_thread(new boost::thread(callback));
		io_service.run();
		return 0;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
