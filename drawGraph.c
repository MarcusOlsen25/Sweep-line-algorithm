#include <stdio.h>
#include <stdlib.h>
#include "geometry.h"
#include "queue.h"

void writePreamble(FILE* file, double scale) {
    fprintf(file, 
        "\\documentclass{article}\n"
        "\\usepackage[margin=0.5in]{geometry}"
        "\\usepackage{tikz}\n"
        "\\begin{document}\n"
        "\\begin{tikzpicture}[scale=%f] %% <- Scale picture\n", scale
    );
}

void writeEnd(FILE* file) {
    fprintf(file,
        "\\end{tikzpicture}\n"
        "\\end{document}\n"
    );
}

void writeCoordinateSystem(FILE* file, int largestCoord) {

    fprintf(file,
        "   %% Coordinate system grid\n"
        "   \\draw[step=1.0,gray,thin] (-%d,-%d) grid (%d,%d);\n"
        "   %% Coordinate system axis\n"
        "   \\draw[->] (-%d.5,0) -- (%d.5,0) node[right] {x};\n"
        "   \\draw[->] (0,-%d.5) -- (0,%d.5) node[right] {y};\n",
        largestCoord, largestCoord, largestCoord, largestCoord, largestCoord,
        largestCoord, largestCoord, largestCoord
    );
}

void writePoint(FILE* file, Point* point) {
    fprintf(file,
            "   \\draw [fill] (%d,%d) circle [radius=0.13];\n", point->x, point->y
        );
}

void writeLines(FILE* file, EndPoints* endPoints, SegmentInfo* segmentArray) {
    for (int i = 0; i < endPoints->count / 2; i++) {
        int start = segmentArray[i].start;
        int end = segmentArray[i].end;

        EndPoint startP = endPoints->endPointsArray[start];
        EndPoint endP = endPoints->endPointsArray[end];
        writePoint(file, &startP.point);
        writePoint(file, &endP.point);
        if (segmentArray[i].segmentIsVisible) {
            fprintf(file,
                "   \\draw [thick, red] (%d,%d) -- (%d,%d);\n", startP.point.x, startP.point.y, endP.point.x, endP.point.y
            );
        } else {
            fprintf(file,
                "   \\draw [thick] (%d,%d) -- (%d,%d);\n", startP.point.x, startP.point.y, endP.point.x, endP.point.y
            );
        }
    }
}

void makeGraph(FILE* file, EndPoints* endPoints, SegmentInfo* segmentArray, Point* point) {
    int largestCoord;
    if (point->x > point->y) {
        largestCoord = point->x;
    } else {
        largestCoord = point->y;
    }
    
    for (int i = 0; i < endPoints->count; i++) {
        if (abs(endPoints->endPointsArray[i].point.x) > largestCoord) {
            largestCoord = abs(endPoints->endPointsArray[i].point.x);
        }
        if (abs(endPoints->endPointsArray[i].point.y) > largestCoord) {
            largestCoord = abs(endPoints->endPointsArray[i].point.y);
        }
    }

    double scale = 0.8;
    if (largestCoord > 10) {
        scale = 0.8 * (10.0 / largestCoord);
    }

    writePreamble(file, scale);
    writeCoordinateSystem(file, largestCoord);
    writePoint(file, point);
    writeLines(file, endPoints, segmentArray);
    
    writeEnd(file);
}