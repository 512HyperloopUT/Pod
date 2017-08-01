#include "comm/comm.h"
#include "gpio/GPIOPin.h"

#include <vector>
#include <string>
#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

/**********
internal code
**********/

//comm ports
vector<GPIOPin> id_block;//for writing sensor ID
GPIOPin r_pin;//for starting read command
GPIOPin e_pin;//for ending reading command

//anything else
vector<GPIOPin> dwrite_block;//for writing to digital outputs

//com api
int curr_fd;

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

int open_com() {
	string portname = "/dev/ttyUSB1";
	int fd = open(portname.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		printf("error %d opening %s: %s", errno, portname.c_str(), strerror(errno));
	}

	set_iface_attribs(fd, B115200, 0);
	set_blocking(fd, 0);
}

/**********
public code
**********/

void initComms() {
	id_block.push_back(GPIOPin(0));
	id_block.push_back(GPIOPin(0));
	id_block.push_back(GPIOPin(0));
	id_block.push_back(GPIOPin(0));
	id_block.push_back(GPIOPin(0));
	for_each(id_block.begin(), id_block.end(), [](GPIOPin& pin){
		pin.setDirection(OUTPUT);
	});

	r_pin.setPort(0);
	r_pin.setDirection(OUTPUT);

	e_pin.setPort(0);
	e_pin.setDirection(INPUT);

	curr_fd = open_com();
}

void setRead(int id) {
	int off = 0;
	for_each(id_block.begin(), id_block.end(), [id, off](GPIOPin& pin) {
		pin.setValue((id >> off) & 0x1);
	});
}

float read() {
	char buf[100];
	int n = read(curr_fd, buf, sizeof(buf));
	//TODO: 4-byte float read protocol (check for terminating character)
}

void write(bool val, int id) {
	if (id < 0 || id >= dwrite_block.size()) {
		return;
	}

	dwrite_block[id].setValue(val);
}
