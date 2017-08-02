#pragma once

#include <string>

int set_iface_attribs(int fd, int speed, int parity);

void set_blocking(int fd, bool should_block);

int open_com(std::string portname);

static int AVAL = -1;//for l-value default param overloading

std::string read_port(int com_id, int buf_size, int& read_cnt = AVAL);
