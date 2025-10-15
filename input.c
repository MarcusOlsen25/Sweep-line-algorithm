#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include "geometry.h"

//Check line for white space
static int checkLine(char* line, int position) {
    while (isspace(line[position])) position++;
        if (line[position] == '\0') {
            return 1;
        }
    return 0;
}

bool scanLineForSegment(char* line, EndPoints* endPoints) {
    int position = 0;

    Point point1;
    Point point2;

    int coordinates = sscanf(line, "%d %d %d %d %n", &point1.x, &point1.y, &point2.x, &point2.y, &position);
    if (coordinates == 4) {
            int lineParsed = checkLine(line, position);
            if (lineParsed) {
                EndPoint endPoint1 = { .point = point1, .id = endPoints->id };
                EndPoint endPoint2 = { .point = point2, .id = endPoints->id++ };
                
                addEndPoint(endPoints, &endPoint1);
                addEndPoint(endPoints, &endPoint2);
                return true;
            }
    }
    return false;
}

bool scanLineForPoint(char* line, Point* point) {
    int position = 0;

    Point newPoint;

    int coordinates = sscanf(line, "%d %d %n", &newPoint.x, &newPoint.y, &position);
    if (coordinates == 2) {
            int lineParsed = checkLine(line, position);
            if (lineParsed) {
                *point = newPoint;
                return true;
            }
    }
    return false;
}