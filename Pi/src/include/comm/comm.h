#pragma once

#include <string>

const char COMM_EOC = (char) 242;

void initComms();

void setRead(int id);

float readUART();

void write(bool val, int id);

std::string writeUpdate(int* data);
