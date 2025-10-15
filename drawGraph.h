#ifndef drawGraph_H
#define drawGraph_H

#include "geometry.h"
#include "queue.h"

void writePreamble(FILE* file, double scale);

void writeEnd(FILE* file);

void writeCoordinateSystem(FILE* file, int largestCoord);

void writePoint(FILE* file, Point* point);

void writeLines(FILE* file, EndPoints* endPoints, SegmentInfo* segmentArray);

void makeGraph(FILE* file, EndPoints* endPoints, SegmentInfo* segmentArray, Point* point);

#endif