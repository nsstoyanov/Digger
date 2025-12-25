#pragma once
#include "Map.h"

extern const char LEVEL1[MAP_HEIGHT][MAP_WIDTH + 1];
extern const char LEVEL2[MAP_HEIGHT][MAP_WIDTH + 1];
//extern - the variable exist somewhere,trust me

const char (*getLevel(int index))[MAP_WIDTH + 1];