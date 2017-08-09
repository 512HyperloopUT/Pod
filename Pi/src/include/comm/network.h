#pragma once

#include <string>

#include "boost/asio.hpp"

class Network {
private:
  boost::asio::io_service service;
  boost::asio::ip::udp::socket* udp_socket;
  boost::asio::ip::udp::endpoint* endpoint;
public:
  Network();
  ~Network();
  bool send(std::string str);
};
