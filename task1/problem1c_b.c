/*
    This program is used to analyse 
        the Graham's Scan algorithm
        in part B.
*/ 

#include "convexHull.h"
#include "problem.h"
#include "linkedList.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    struct problem *problem = readProblem(stdin, PART_B);

    struct solution *solution = grahamScan(problem);

    printf("Graham's Scan algorithm:\n");
    printf("Input size is: %d\n", problem->numPoints);
    printf("Operation count is: %d\n", solution->operationCount);

    freeSolution(solution);
    freeProblem(problem);

    return EXIT_SUCCESS;
}
