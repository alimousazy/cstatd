#include "server.hpp"

#pragma once

using boost::asio::ip::tcp;

tcp_server::tcp_server(boost::asio::io_service& io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 8888))
{
    start_accept();
}
void tcp_server::start_accept()
{
    tcp_connection::pointer new_connection = tcp_connection::create(acceptor_.get_io_service(), &scatt);

    acceptor_.async_accept(new_connection->socket(), 
            boost::bind(&tcp_server::handle_accept, 
            this, 
            new_connection,
            boost::asio::placeholders::error));
}
void tcp_server::handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)
{
    if (!error)
    {
      new_connection->start();
    }
    start_accept();
}
scatter<std::string>::q_ptr tcp_server::getQueue()
{
    return scatt.getQueue();
}


