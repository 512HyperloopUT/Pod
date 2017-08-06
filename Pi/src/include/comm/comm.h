#pragma once

#include <string>

void initComms();

void setRead(int id);

float readUART();

void write(bool val, int id);

std::string writeUpdate(int* data);

void destroyComms();
