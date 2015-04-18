#include <iostream>
#include <string>
#include <thread>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/thread/thread.hpp>

#include "point.pb.h"
#include "scatter.hpp"
#include "server.hpp"
#include "data_access.hpp"

void consumer_function(data_access &dAccess, scatter<std::string>::q_ptr ptr) {
    std::string str;
    while(true) 
    {
        ptr->wait_and_pop(str);
        dAccess.add("defef", 1);
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
        g.add_thread(new boost::thread(callback));
    }
    io_service.run();
  return 0;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
