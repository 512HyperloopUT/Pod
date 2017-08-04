#pragma once

#ifndef SPACEXNET_H
#define SPACEXNET_H

/****************************UDP/GUI classes****************************/
/*

<code>master_packet</code> is a packet that is sent to the computer with a GUI. It is an abstract class.

<code>master</code> is the computer that handles the GUI. It is an abstract class.

*/
class master_packet {
public:
	master_packet(std::string ptype);
	virtual ~master_packet();
public:
	std::string type;
};
class master {
public:
	virtual void send(master_packet &packet) = 0;
	virtual master_packet receive(void) = 0;
};

#endif