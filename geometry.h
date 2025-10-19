#ifndef geometry_H
#define geometry_H

#include <stdbool.h>
#include <math.h>
#include "BST.h"

#define PI 3.14159265358979323846

typedef struct StatusData StatusData;

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point point;
    bool startPoint;
    double angle;
    int id;
    double distanceFromP;
} EndPoint;

typedef struct {
    EndPoint* endPointsArray;
    int count;
    int capacity;
    int id;
} EndPoints;

EndPoints init_endPoints();

EndPoints* addEndPoint(EndPoints* endPoints, EndPoint* endPoint);

void computeLineSegmentDirections(EndPoints* endPoints, Point point);

double lineIntersectionDistance(Point point1, Point point2, Point point, double angle);

int compareEndPointAngles(const void* endPoint1, const void* endPoint2);

void freeEndPoints(EndPoints* endPoints);


#endif