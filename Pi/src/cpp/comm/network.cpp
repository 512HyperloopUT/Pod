#include "comm/network.h"

#include "boost/array.hpp"

Network::Network() {
  udp_socket = new boost::asio::ip::udp::socket(service,
    boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0));
  udp_socket->set_option(boost::asio::socket_base::broadcast(true));

  endpoint = new boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::broadcast(), 8888);
}

Network::~Network() {
  udp_socket->close();
  free(udp_socket);
  free(endpoint);
}

bool Network::send(std::string str) {
  boost::system::error_code err;
  udp_socket->send_to(boost::asio::buffer(str), *endpoint, 0, err);
  return err == boost::system::errc::success;
}
