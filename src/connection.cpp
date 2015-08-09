#include "connection.hpp"

tcp_connection::tcp_connection(boost::asio::io_service& io_service, scatter<std::string> *queue) : socket_(io_service), c_queue(queue){};

tcp_connection::pointer tcp_connection::create(boost::asio::io_service& io_service, scatter<std::string> *queue)
{
    return tcp_connection::pointer(new tcp_connection(io_service, queue));
}

tcp::socket& tcp_connection::socket()
{
    return socket_;
}

void tcp_connection::do_read() 
{
    memset(message_, '\0', buffer_size);
    boost::asio::async_read(socket_, boost::asio::buffer(message_),
      boost::bind(&tcp_connection::read_complete, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred),
      boost::bind(&tcp_connection::handle_read, shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));

}
void tcp_connection::start()
{
    do_read();
}
size_t tcp_connection::read_complete(const boost::system::error_code & err, size_t bytes) {
	if (err) 
		return 0;
	long size = 1;
	if(strstr(message_, "-"))
	{
		char *end_ptr;
		size = strtol(message_, &end_ptr, 10);
		if(size == 0 && *end_ptr == '\0')
		{
			size = 1;
		}
	}
	if(bytes > 3)
	{
		size = 0;
	}
	return size;
}

void  tcp_connection::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
	if(error)
		return;
	char *pos = strstr(message_, "-");
	if(pos && *++pos)
	{
		Heartbeat::pushed++;
	  c_queue->push(pos);
	}
	do_read();
}

