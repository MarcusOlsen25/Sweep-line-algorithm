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

int main(int argc, char *argv[]) {

    if (!argc == 2) {
        printf("Wrong number of arguments\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
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

    computeLineSegmentDirections(&endPoints, point);

    //Sort array of end points based on angle
    qsort(endPoints.endPointsArray, endPoints.count, sizeof(EndPoint), compareEndPointAngles);

    //Array storing the index of endpoints in the original array endPoints
    SegmentInfo* segmentArray = malloc(endPoints.count / 2 * sizeof(SegmentInfo));

    //Set indices of segmentArray
    computeEndPointIndices(&endPoints, segmentArray);

    //Insert initial line segments that crosses the starting horizontal ray from the point
    insertSegmentsCrossingRay(&endPoints, segmentArray, point, status);

    //Line segment closest to point after initial insertions
    StatusData* visibleSegment = (StatusData*) getSmallestNodeData(status);
        if (visibleSegment) {
            segmentArray[visibleSegment->start->id].segmentIsVisible = true;
        }
    

    EndPoint* queueTail = endPoints.endPointsArray + endPoints.count;
    EndPoint* queueHead = endPoints.endPointsArray;
    while (queueHead != queueTail) {
        EndPoint* pop = queueHead;
        if (pop->startPoint) {
            handleNewSegment(status, &endPoints, segmentArray, pop, point);
        } else {
            handleEndOfSegment(status, segmentArray, pop);
        }
        visibleSegment = (StatusData*) getSmallestNodeData(status);
        if (visibleSegment) {
            segmentArray[visibleSegment->start->id].segmentIsVisible = true;
        }
        queueHead++;
    }

    FILE* fileOutput = fopen("output.tex", "w");
    if (!fileOutput) {
        printf("Error opening file\n");
        free(endPoints.endPointsArray);
        return 1;
    }
    
    makeGraph(fileOutput, &endPoints, segmentArray, &point);

    printSegments(segmentArray, &endPoints);

    freeTree(status);

    freeEndPoints(&endPoints);

    free(segmentArray);
    
    return 0;
}
