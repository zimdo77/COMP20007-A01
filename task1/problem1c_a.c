/*
    This program is used to analyse 
        the Jarvis' March algorithm
        in part A.
*/ 

#include "convexHull.h"
#include "problem.h"
#include "linkedList.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    struct problem *problem = readProblem(stdin, PART_A);

    struct solution *solution = jarvisMarch(problem);

    printf("Jarvis' March algorithm:\n");
    printf("Input size is: %d\n", problem->numPoints);
    printf("Number of points on the convex hull is: %d\n", solution->convexHull->size);
    printf("Operation count is: %d\n", solution->operationCount);

    freeSolution(solution);
    freeProblem(problem);

    return EXIT_SUCCESS;
}
