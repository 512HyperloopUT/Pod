#include "comm/network.h"

#include "boost/asio.hpp"
#include "boost/array.hpp"

namespace ip = boost::asio::ip;

boost::asio::io_service service;
ip::udp::socket* udp_socket;
ip::udp::endpoint* endpoint;

void initNetwork() {
  udp_socket = new ip::udp::socket(service,
    ip::udp::endpoint(ip::udp::v4(), 0));
  udp_socket->set_option(boost::asio::socket_base::broadcast(true));

  endpoint = new ip::udp::endpoint(ip::address_v4::broadcast(), 8888);
}

bool send(std::string str) {
  boost::system::error_code err;
  udp_socket->send_to(boost::asio::buffer(str), *endpoint, 0, err);
  return err == boost::system::errc::success;
}

void destroyNetwork() {
  udp_socket->close();
  free(udp_socket);
  free(endpoint);
}
