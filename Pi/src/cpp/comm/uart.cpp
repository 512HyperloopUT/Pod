#include "comm/uart.h"

#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int set_iface_attribs(int fd, int speed, int parity) {
	struct termios tty;
	memset(&tty, 0, sizeof(tty));
	if (tcgetattr(fd, &tty) != 0) {
		printf("error %d from tcgetattr\n", errno);
		return -1;
	}

	cfsetospeed(&tty, speed);
	cfsetispeed(&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;//8 bit characters
	tty.c_iflag &= ~IGNBRK;//disable ignore_break
	tty.c_lflag = 0;//no signaling chars, no echo
	tty.c_oflag = 0;//no remapping, no delays
	tty.c_cc[VMIN] = 0;//read doesn't block
	tty.c_cc[VTIME] = 5;//0.5 second read timeout
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);//shut off xon/xoff control
	tty.c_cflag |= CLOCAL | CREAD;//ignore modem controls, enable reading
	tty.c_cflag &= ~(PARENB | PARODD);//shut off parity
	tty.c_cflag |= parity;//set parity
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		printf("error %d from tcsetattr\n", errno);
		return -1;
	}
	return 0;
}

void set_blocking(int fd, bool should_block) {
	struct termios tty;
	memset(&tty, 0, sizeof(tty));
	if (tcgetattr(fd, &tty) != 0) {
		printf("error %d from tcgetattr\n", errno);
	}

	tty.c_cc[VMIN] = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;

	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		printf("error %d setting termios attrs\n", errno);
	}
}

int open_com(std::string portname) {
	int fd = open(portname.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		printf("error %d opening %s: %s", errno, portname.c_str(), strerror(errno));
	}

	set_iface_attribs(fd, B115200, 0);
	set_blocking(fd, 0);
}

std::string read_uart(int com_id, int buf_size, int& read_cnt) {
	char buf[buf_size];
	int cnt = read(com_id, buf, buf_size);
	if (read_cnt != AVAL) {
		read_cnt = cnt;
	}
	return std::string(buf, buf_size);
}
