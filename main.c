#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "geometry.h"
#include "BST.h"
#include "drawGraph.h"
#include "queue.h"
#include "input.h"

void printVal(StatusData *data) {
    StatusData pointData = *(StatusData*) data;
    printf("id: %d, %d %d\n", pointData.start->id, pointData.start->point.x, pointData.start->point.y);
}

int main() {

    FILE* file = fopen("input.txt", "r");
    if (!file) {
        printf("Error opening file\n");
        return 1;
    }

    //Array of line segment end points
    EndPoints endPoints = init_endPoints();

    //Point
    Point point = { 0 };

    //Buffer to store input line
    char line[256];

    int largestCoord;

    //Read input
    while (fgets(line, sizeof(line), file)) {
        //Add line segment
        bool lineSegmentAdded = scanLineForSegment(line, &endPoints);
        if (lineSegmentAdded) continue;

        //Add point
        bool pointAdded = scanLineForPoint(line, &point);
        if (pointAdded) continue;

        //Line does not match line segment or point
        printf("Invalid input\n");
        free(endPoints.endPointsArray);
        return 1;
    }

    BST* status = init_BST(sizeof(EndPoint), compareDistance);

    computeLineSegmentDirections(&endPoints, point, status);

    //Sort array of end points based on angle
    qsort(endPoints.endPointsArray, endPoints.count, sizeof(EndPoint), compareEndPointAngles);

    //Array storing the index of endpoints in the original array endPoints
    SegmentInfo* segmentArray = calloc(endPoints.count / 2 * sizeof(SegmentInfo), sizeof(SegmentInfo));

    //Set indices of segmentArray
    computeEndPointIndices(&endPoints, segmentArray, point);

    //Insert initial line segments that crosses the starting horizontal ray from the point
    insertSegmentsCrossingRay(&endPoints, segmentArray, point, status);

    //Line segment closest to point after initial insertions
    StatusData* visibleSegment = (StatusData*) getSmallestNodeData(status);
        if (visibleSegment) {
            segmentArray[visibleSegment->start->id].segmentIsVisible = true;
            printf("Currently visible: id %d, %d %d\n\n", visibleSegment->start->id, visibleSegment->start->point.x, visibleSegment->start->point.y);
        }
    

    EndPoint* queueTail = endPoints.endPointsArray + endPoints.count;
    EndPoint* queueHead = endPoints.endPointsArray;
    while (queueHead != queueTail) {
        EndPoint* pop = queueHead;
        if (pop->startPoint) {
            printf("New Segment: id: %d, %d %d\n", pop->id, pop->point.x, pop->point.y);
            handleNewSegment(status, &endPoints, segmentArray, pop, point);
        } else {
            printf("End segment: id %d, %d %d\n", pop->id, pop->point.x, pop->point.y);
            handleEndOfSegment(status, segmentArray, pop);
        }

        visibleSegment = (StatusData*) getSmallestNodeData(status);
        if (visibleSegment) {
            segmentArray[visibleSegment->start->id].segmentIsVisible = true;
            printf("Currently visible: id %d, %d %d\n", visibleSegment->start->id, visibleSegment->start->point.x, visibleSegment->start->point.y);
        }
        queueHead++;
        printBST(status, printVal);
    }

    FILE* fileOutput = fopen("output.tex", "w");
    if (!fileOutput) {
        printf("Error opening file\n");
        free(endPoints.endPointsArray);
        return 1;
    }
    
    makeGraph(fileOutput, &endPoints, segmentArray, &point);
    
    return 0;
}
