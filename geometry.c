#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "geometry.h"
#include "queue.h"

//Initialize dynamic array of EndPoint. Each new EndPoint gets a unique id.
EndPoints init_endPoints() {
    EndPoints endPoints;
    endPoints.endPointsArray = malloc(sizeof(EndPoint) * 8);
    endPoints.capacity = 8;
    endPoints.count = 0;
    endPoints.id = 0;
    return endPoints;
}

void freeEndPoints(EndPoints* endPoints) {
    if (endPoints->endPointsArray) {
        free(endPoints->endPointsArray);
    }
}

//Append new EndPoint to the array. If count == capacity, the size doubles.
EndPoints* addEndPoint(EndPoints* endPoints, EndPoint* endPoint) {
    if (endPoints->count >= endPoints->capacity) {
        endPoints->capacity *= 2;
        EndPoint* tmp = realloc(endPoints->endPointsArray, endPoints->capacity * sizeof(EndPoint));
        if (!tmp) {
            printf("realloc failed");
            return NULL;
        }
        endPoints->endPointsArray = tmp;
    }
    endPoints->endPointsArray[endPoints->count++] = *endPoint;

    return endPoints;
}

//Compute angle between a horizontal line to the right of Point and a line between point and endPoint
static double computeAngle(Point point, EndPoint* endPoint) {
    double dx = endPoint->point.x - point.x;
    double dy = endPoint->point.y - point.y;
    double angle = atan2(dy, dx);

    if (angle < 0) {
        angle += 2 * PI;
    }

    return angle;
}

static double euclideanDistance(Point a, Point b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

//Given a line segment point1, point2 and a point and an angle, compute the length of a line from the point and
//the intersection with the input line segment
double lineIntersectionDistance(Point point1, Point point2, Point point, double angle) {
    double dx = cos(angle);
    double dy = sin(angle);

    double x1 = point1.x;
    double y1 = point1.y;
    double x2 = point2.x;
    double y2 = point2.y;

    double lengthx = x2 - x1;
    double lengthy = y2 - y1;

    double det = dx * (-lengthy) - dy * (-lengthx);

    if (fabs(det) < 1e-10) {
        return -1.0;
    }

    double pointx = point.x;
    double pointy = point.y;

    double t = ((x1 - pointx) * (-lengthy) - (y1 - pointy) * (-lengthx)) / det;

    double intersectionx = pointx + t * dx;
    double intersectiony = pointy + t * dy;

    double distance = sqrt((intersectionx - pointx)*(intersectionx - pointx) + (intersectiony - pointy)*(intersectiony - pointy));
    return distance;
}

//Determine the starting point and ending point of a line segment depending on the angle for each endpoint.
//If the line is horizontal with the sweep line, the closest point is the start point.
static void determineStartAndEndEndPoint(EndPoint* endPoint1, EndPoint* endPoint2) {
    double angle1 = endPoint1->angle;
    double angle2 = endPoint2->angle;

    if (angle1 == angle2) {
        if (endPoint1->distanceFromP < endPoint2->distanceFromP) {
            endPoint1->startPoint = true;
            endPoint2->startPoint = false;
        } else {
            endPoint1->startPoint = false;
            endPoint2->startPoint = true;
        }
        return;
    }

    double diff = fabs(angle1 - angle2);

    bool sameOrder = (diff < PI);
    bool endPoint1ComesFirst = (angle1 < angle2);

    endPoint1->startPoint =  (sameOrder == endPoint1ComesFirst);
    endPoint2->startPoint = !(sameOrder == endPoint1ComesFirst);

}
//Initializes each endpoint with its angle, whether it's astarting point or ending point and its distance from Point
void computeLineSegmentDirections(EndPoints* endPoints, Point point) {
    for (int i = 0; i < endPoints->count; i += 2) {
        EndPoint* endPoint1 = &endPoints->endPointsArray[i];
        EndPoint* endPoint2 = &endPoints->endPointsArray[i + 1];
        endPoint1->angle = computeAngle(point, endPoint1);
        endPoint1->distanceFromP = euclideanDistance(point, endPoint1->point);
        endPoint2->angle = computeAngle(point, endPoint2);
        endPoint2->distanceFromP = euclideanDistance(point, endPoint2->point);
        determineStartAndEndEndPoint(endPoint1, endPoint2);
    }
}
//Compare function passed to the binary search tree.
int compareEndPointAngles(const void* endPoint1, const void* endPoint2) {
    const EndPoint* ep1 = (const EndPoint*)endPoint1;
    const EndPoint* ep2 = (const EndPoint*)endPoint2;
    if (ep1->angle < ep2->angle) return -1;
    if (ep1->angle > ep2->angle) return 1;
    if (ep1->startPoint > ep2->startPoint) return -1;
    if (ep1->startPoint < ep2->startPoint) return 1;
    if (ep1->startPoint && ep2->startPoint && ep1->distanceFromP < ep2->distanceFromP) return -1;
    if (ep1->startPoint && ep2->startPoint && ep1->distanceFromP > ep2->distanceFromP) return 1;
    if (!ep1->startPoint && !ep2->startPoint && ep1->distanceFromP > ep2->distanceFromP) return -1;
    if (!ep1->startPoint && !ep2->startPoint && ep1->distanceFromP < ep2->distanceFromP) return 1;
    return 0;
}
