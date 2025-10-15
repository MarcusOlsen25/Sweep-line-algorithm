#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "geometry.h"

SegmentInfo* init_segmentArray(size_t size) {
    SegmentInfo* segmentArray = malloc(size);
    for (int i = 0; i < size; i ++) {
        segmentArray[i].start = -1;
    }
}

StatusData createStatusData(EndPoint* start, EndPoint* end, Point point) {
    return (StatusData){ .start = start, .end = end, .point = point };
}

void computeEndPointIndices(EndPoints* endPoints, SegmentInfo* segmentArray, Point point) {
    for (int i = 0; i < endPoints->count; i++) {
        EndPoint* p = &endPoints->endPointsArray[i];
        if (p->startPoint) {
            segmentArray[p->id].start = i; 
        } else {
            segmentArray[p->id].end = i;
            segmentArray[p->id].segmentIsActive = false;
            segmentArray[p->id].segmentIsVisible = false;
        }
    }
}

static double closestPointOnHorizontal(StatusData* line) {
    if (line->start->distanceFromP < line->end->distanceFromP) {
        return line->start->distanceFromP;
    } else {
        return line->end->distanceFromP;
    }
}

int compareDistance(void* dataNew, void* dataOld, void* context) {
    StatusData* new = (StatusData*)dataNew;
    StatusData* old = (StatusData*)dataOld;
    double angle = *(double*)context;
    double newDistance = lineIntersectionDistance(new->start->point, new->end->point, new->point, angle);
    if (newDistance == -1.0) {
        newDistance = closestPointOnHorizontal(new);
    }
    double oldDistance = lineIntersectionDistance(old->start->point, old->end->point, old->point, angle);
    if (oldDistance == -1.0) {
        oldDistance = closestPointOnHorizontal(old);
    }
    if (newDistance > oldDistance) {
        return 1;
    }
    return 0;
}

void handleNewSegment(BST* bst, EndPoints* endPoints, SegmentInfo* segmentArray, EndPoint* endPoint, Point point) {
    int segmentIndex = segmentArray[endPoint->id].end;
    StatusData newLineSegment = createStatusData(endPoint, &endPoints->endPointsArray[segmentIndex], point);
    Node* nodeInserted = insert(&newLineSegment, bst, &endPoint->angle);
    segmentArray[endPoint->id].segmentIsActive = true;
    segmentArray[endPoint->id].node = nodeInserted;
}

void handleEndOfSegment(BST* bst, SegmentInfo* segmentArray, EndPoint* endPoint) {
    int segmentIndex = endPoint->id;
    delete(segmentArray[segmentIndex].node, bst);
}

void insertSegmentsCrossingRay(EndPoints* endPoints, SegmentInfo* segmentArray, Point point, BST* tree) {
    for (int i = 0; i < endPoints->count / 2; i++) {
        int startIndex = segmentArray[i].start;
        int endIndex = segmentArray[i].end;
        EndPoint* start = &endPoints->endPointsArray[startIndex];
        EndPoint* end = &endPoints->endPointsArray[endIndex];
        double diff = fabs(start->angle - end->angle);
        double angle = 0.0;
        if (diff > PI) {
            StatusData newLineSegment = createStatusData(start, end, point);
            Node* node = insert(&newLineSegment, tree, &angle);
            segmentArray[i].node = node;
            printf("Pre start segment: id: %d, %d %d\n", newLineSegment.start->id, newLineSegment.start->point.x, newLineSegment.start->point.y);
        }
    }
}