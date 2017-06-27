#pragma once

#include "comm/half.h"

void initComms();

float read(int id);

void write(float val, int id);
