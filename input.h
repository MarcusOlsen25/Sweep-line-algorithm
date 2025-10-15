#ifndef INPUT_H
#define INPUT_H

#include "geometry.h"

bool scanLineForSegment(char* line, EndPoints* endPoints);

bool scanLineForPoint(char* line, Point* point);

static int checkLine(char* line, int position);

#endif