#ifndef queue_H
#define queue_H

#include <stddef.h>
#include <stdbool.h>
#include "geometry.h"
#include "BST.h"

typedef struct {
    int start;
    int end;
    Node* node;
    bool segmentIsActive;
    bool segmentIsVisible;
} SegmentInfo;

typedef struct StatusData {
    EndPoint* start;
    EndPoint* end;
    Point point;
} StatusData;

StatusData createStatusData(EndPoint* start, EndPoint* end, Point point);

static double closestPointOnHorizontal(StatusData* line);

void computeEndPointIndices(EndPoints* endPoints, SegmentInfo* segmentArray, Point point);

int compareDistance(void* dataNew, void* dataOld, void* context);

void handleNewSegment(BST* bst, EndPoints* endPoints, SegmentInfo* segmentArray, EndPoint* endPoint, Point point);

void handleEndOfSegment(BST* bst, SegmentInfo* segmentArray, EndPoint* endPoint);

void insertSegmentsCrossingRay(EndPoints* endPoints, SegmentInfo* segmentArray, Point point, BST* tree);

#endif