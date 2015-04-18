#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include "scatter.hpp"
#include "connection.hpp"
#include "c_queue.cpp"

#pragma once

using boost::asio::ip::tcp;

class tcp_server
{
    public:
      tcp_server(boost::asio::io_service& io_service);
      scatter<std::string>::q_ptr getQueue();

    private:
      scatter<std::string> scatt;
      void start_accept();
      void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error);
      tcp::acceptor acceptor_;
};

