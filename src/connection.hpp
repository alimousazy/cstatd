#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "c_queue.cpp"
#include "scatter.hpp"
#define BUFFER_SIZE 1024

#pragma once

using boost::asio::ip::tcp;

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
public:

  typedef boost::shared_ptr<tcp_connection> pointer;
  static pointer create(boost::asio::io_service& io_service, scatter<std::string>* queue);
  tcp::socket& socket();
  void start();
  tcp_connection(boost::asio::io_service& io_service, scatter<std::string>* queue);
  std::string msg[2];

private:
  scatter<std::string> *c_queue;
  tcp::socket socket_;
  std::vector<std::string> msg_list;
  static const int buffer_size = 1024;
  char message_[buffer_size];
  void parse_line(char *line);
  void do_read();
  size_t read_complete(const boost::system::error_code & err, size_t bytes);
  void handle_read(const boost::system::error_code& /*error*/, size_t bytes_transferred);
};

