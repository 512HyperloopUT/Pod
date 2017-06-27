#include "comm/comm.h"
#include "gpio/GPIOPin.h"

#include <vector>

using namespace std;

GPIOPin idata0(0);
GPIOPin idata1(0);
GPIOPin idata2(0);
GPIOPin idata3(0);
GPIOPin idata4(0);
GPIOPin idata5(0);
GPIOPin idata6(0);
GPIOPin idata7(0);
GPIOPin idata8(0);
GPIOPin idata9(0);

GPIOPin odata0(0);
GPIOPin odata1(0);
GPIOPin odata2(0);
GPIOPin odata3(0);
GPIOPin odata4(0);
GPIOPin odata5(0);
GPIOPin odata6(0);
GPIOPin odata7(0);
GPIOPin odata8(0);
GPIOPin odata9(0);

GPIOPin pistate(0);

GPIOPin tistate(0);

GPIOPin cmd0(0);
GPIOPin cmd1(0);
GPIOPin cmd2(0);
GPIOPin cmd3(0);

void initComms() {
	idata0.setDirection(INPUT);
	idata1.setDirection(INPUT);
	idata2.setDirection(INPUT);
	idata3.setDirection(INPUT);
	idata4.setDirection(INPUT);
	idata5.setDirection(INPUT);
	idata6.setDirection(INPUT);
	idata7.setDirection(INPUT);
	idata8.setDirection(INPUT);
	idata9.setDirection(INPUT);

	odata0.setDirection(OUTPUT);
	odata1.setDirection(OUTPUT);
	odata2.setDirection(OUTPUT);
	odata3.setDirection(OUTPUT);
	odata4.setDirection(OUTPUT);
	odata5.setDirection(OUTPUT);
	odata6.setDirection(OUTPUT);
	odata7.setDirection(OUTPUT);
	odata8.setDirection(OUTPUT);
	odata9.setDirection(OUTPUT);

	pistate.setDirection(OUTPUT);

	tistate.setDirection(INPUT);

	cmd0.setDirection(OUTPUT);
	cmd1.setDirection(OUTPUT);
	cmd2.setDirection(OUTPUT);
	cmd3.setDirection(OUTPUT);
}

void idataWrite(int val) {
	idata0.setValue(val & 0b0000000001);
	idata1.setValue(val & 0b0000000010);
	idata2.setValue(val & 0b0000000100);
	idata3.setValue(val & 0b0000001000);
	idata4.setValue(val & 0b0000010000);
	idata5.setValue(val & 0b0000100000);
	idata6.setValue(val & 0b0001000000);
	idata7.setValue(val & 0b0010000000);
	idata8.setValue(val & 0b0100000000);
	idata9.setValue(val & 0b1000000000);
}

int odataRead() {
	return odata0.getValue() |
		odata1.getValue() << 1 |
		odata2.getValue() << 2 |
		odata3.getValue() << 3 |
		odata4.getValue() << 4 |
		odata5.getValue() << 5 |
		odata6.getValue() << 6 |
		odata7.getValue() << 7 |
		odata8.getValue() << 8 |
		odata9.getValue() << 9;
}

void cmdWrite(int val) {
	cmd0.setValue(val & 0b0001);
	cmd1.setValue(val & 0b0010);
	cmd2.setValue(val & 0b0100);
	cmd3.setValue(val & 0b1000);
}

float read(int id) {
	//pi sends 0 - reset
	cmdWrite(0);

	//pi reads 0 - reset finished
	while (tistate.getValue() != false) {}

	//pi writes cmd - read cmd
	cmdWrite(1);

	//pi writes idata - sensor id
	idataWrite(id);

	//pi sends 1 - start command
	pistate.setValue(!pistate.getValue());

	//wait for cmd to finish
	while (pistate.getValue() != tistate.getValue()) {}

	//pi reads odata
	return odataRead();
}

void write(int val, int id) {
	//pi sends 0 - reset
	cmdWrite(0);

	//pi reads 0 - reset finished
	while (tistate.getValue() != false) {}

	//pi writes cmd - write cmd > 1
	cmdWrite(id);

	//pi writes idata - write value
	idataWrite(val);

	//pi sends 1 - start command
	pistate.setValue(!pistate.getValue());

	//wait for cmd to finish - NOT NECESSARY?
	while (pistate.getValue() != tistate.getValue()) {}
}

float b12tof(uint16_t x) {
	return (float)x/(1<<11) - 1.0;
}

