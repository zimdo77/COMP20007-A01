/*
    Skeleton written by Grady Fitzpatrick for 
    COMP20007 Assignment 1 2024 Semester 1
    
    Header for module which contains convex hull 
        specification data structures and functions.

    Implemented by Quoc Khang Do
*/
#include "linkedList.h"
#include "problem.h"
#include "convexHull.h"
#include "stack.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Minimum number of points required in a set for a convex hull to be defined */
#define MIN_POINTS 3 

enum orientationResult {
    COLLINEAR = 0,
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = 2
};

/*****************************************************************************/

/********************************/
/* HELPER FUNCTION DECLARATIONS */
/********************************/

enum orientationResult orientation(struct problem *p, int idxFirst, int idxMiddle, int idxFinal);
int findLeftmostIdx(struct problem *p);
int findLowestIdx(struct problem *p);
long double findPolarAngle(struct problem *p, int pointIdx);
void sortPolarAngle(struct problem *p, struct solution *s, int pointsIdx[], int left, int right);
void merge(struct problem *p, struct solution *s, int pointsIdx[], int left, int middle, int right);

/*****************************************************************************/

/*******************************/
/* HELPER FUNCTION DEFINITIONS */
/*******************************/

/* Finds the orientation between the three given points - calculates the angle
    between the Middle-First vector and Middle-Final vector - if the 
    Middle-First vector is zero-length, the gradient of the Middle-Final 
    vector is instead used. */
enum orientationResult orientation(struct problem *p, int idxFirst, int idxMiddle, int idxFinal) {
    assert(idxFirst >= 0 && idxFirst < p->numPoints);
    assert(idxMiddle >= 0 && idxMiddle < p->numPoints);
    assert(idxFinal >= 0 && idxFinal < p->numPoints);

    /* Use cross-product to calculate turn direction. */
    long double p0x = p->pointsX[idxFirst];
    long double p0y = p->pointsY[idxFirst];

    long double p1x = p->pointsX[idxMiddle];
    long double p1y = p->pointsY[idxMiddle];

    long double p2x = p->pointsX[idxFinal];
    long double p2y = p->pointsY[idxFinal];

    /* Cross product of vectors P1P0 & P1P2 */
    long double crossProduct = (p0x - p1x) * (p2y - p1y) - (p0y - p1y) * (p2x - p1x);

    if (crossProduct == 0){
        if (idxFirst == idxMiddle) {
            /* Special case where we are only looking for positive slope of 
                P1P2. */
            if (p2x == p1x) {
                /* Special case: dx = 0, vertical */
                if (p2y < p1y) {
                    /* Directly upwards */
                    return COUNTERCLOCKWISE;
                } else if (p2y == p1y) {
                    /* Same point. */
                    return COLLINEAR;
                } else {
                    return CLOCKWISE;
                }
            }
            long double m = (p2y - p1y) / (p2x - p1x);
            if (m >= 0) {
                return COUNTERCLOCKWISE;
            } else {
                return CLOCKWISE;
            }
            
        }
        return COLLINEAR;
    } else if (crossProduct > 0) {
        return CLOCKWISE;
    } else {
        return COUNTERCLOCKWISE;
    }
}

/*****************************************************************************/

/* Find and return the index of the leftmost point. */
int findLeftmostIdx(struct problem *p) {
    int i, minIdx = 0;

    for (i = 1; i < p->numPoints; i++) {
        if (p->pointsX[i] < p->pointsX[minIdx]) {
            minIdx = i;
        } else if (p->pointsX[i] == p->pointsX[minIdx]) {
        /* break tie with the lower point being chosen */
            if (p->pointsY[i] < p->pointsY[minIdx]) {
                minIdx = i;
            }
        }
    }
    return minIdx;
}

/*****************************************************************************/

/* Find and return the index of the lowest point. */
int findLowestIdx(struct problem *p) {
    int i, minIdx = 0;

    for (i = 1; i < p->numPoints; i++) {
        if (p->pointsY[i] < p->pointsY[minIdx]) {
            minIdx = i;
        } else if (p->pointsY[i] == p->pointsY[minIdx]) {
        /* break tie with the leftmost point being chosen */
            if (p->pointsX[i] < p->pointsX[minIdx]) {
                minIdx = i;
            }
        }
    }
    return minIdx;
}

/*****************************************************************************/

/* Calculate and return the polar angle of a selected point with respect to
    the lowest point. The formula uses basic trigonometry to find the polar
    angle: take the inverse tangent of the quotient of deltaY and deltaX, 
    where deltaY and deltaX are the changes in the Y and X direction
    respectively. */
long double findPolarAngle(struct problem *p, int pointIdx) {
    long double polarAngle, deltaX, deltaY;
    int lowestIdx = findLowestIdx(p);        /* index of the lowest point */

    deltaX = p->pointsX[pointIdx] - p->pointsX[lowestIdx];
    deltaY = p->pointsY[pointIdx] - p->pointsY[lowestIdx];

    polarAngle = atan2(deltaY, deltaX);
    
    return polarAngle;
}

/*****************************************************************************/

/* Sort the points based on their polar angles with respect to the lowest
    point. The sorting is done on an array of integers <pointsIdx> where
    the integers correspond to the indexes of the points. <left> and <right>
    are the first and last indexes of the array <pointsIdx> respectively */
/* Adapted from Mergesort: 
    https://www.geeksforgeeks.org/c-program-for-merge-sort/ */
void sortPolarAngle(struct problem *p, struct solution *s, int pointsIdx[], int left, int right) {
    if (left < right) {
        /* Find the middle index */
        int middle = left + ((right - left) / 2);

        /* Recursively sort the left half of the array */
        sortPolarAngle(p, s, pointsIdx, left, middle);
        /* Recursively sort the right half of the array */
        sortPolarAngle(p, s, pointsIdx, middle + 1, right);

        /* Merge the left half of the array with the right half */
        merge(p, s, pointsIdx, left, middle, right);
    }
}

/*****************************************************************************/

/* Merge the sorted subarrays of <pointsIdx> */
/* Adapted from Mergesort: 
    https://www.geeksforgeeks.org/c-program-for-merge-sort/ */
void merge(struct problem *p, struct solution *s, int pointsIdx[], int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;     /* size of tempLeft subarray */
    int n2 = right - middle;        /* size of tempRight subarray */

    /* Make temporary arrays and insert in data */
    int tempLeft[n1], tempRight[n2];
    for (i = 0; i < n1; i++) {
        tempLeft[i] = pointsIdx[left + i];
    }
    for (j = 0; j < n2; j++) {
        tempRight[j] = pointsIdx[middle + 1 + j];
    }

    /* Merge the temporary arrays back together */
    i = 0;                          /* starting index of tempLeft */
    j = 0;                          /* starting index of tempRight */
    k = left;                       /* starting index of the merged array */
    
    while (i < n1 && j < n2) {
        if (findPolarAngle(p, tempLeft[i]) <= findPolarAngle(p, tempRight[j])) {
            pointsIdx[k] = tempLeft[i];
            i++;
        } else {
            pointsIdx[k] = tempRight[j];
            j++;
        }
        k++;
        /* A basic operation for Graham's Scan is the comparison between
            the angles of points during the sort */
        (s->operationCount)++;
    }
    
    /* Merge the remaining elements of tempLeft (or tempRight) */
    while (i < n1) {
        pointsIdx[k] = tempLeft[i];
        i++;
        k++;
    }
    while (j < n2) {
        pointsIdx[k] = tempRight[j];
        j++;
        k++;
    }
}

/*****************************************************************************/

/************************/
/* FUNCTION DEFINITIONS */
/************************/

/* Part A - perform Jarvis' March to construct a convexhull for the given
    problem. */
struct solution *jarvisMarch(struct problem *p) {
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);

    /* Initialise the operation count */
    s->operationCount = 0;

    /* Record the number of points */
    int numPoints = p->numPoints;

    /* Initialise an empty list to store convex hull points */
    struct linkedList *hull = newList();

    /* Check that there are at least the minimum number of points */
    if (numPoints < MIN_POINTS) {
        s->convexHull = hull;
        return s;
    }

    /* Keep track of the points' indexes */
    int leftmostIdx, currentIdx, nextIdx, pointIdx;

    /* Find the left most point and store its index */
    leftmostIdx = findLeftmostIdx(p);

    /* Start from the leftmost point */
    currentIdx = leftmostIdx;

    do {
        /* Add current point to the convex hull */
        insertTail(hull, p->pointsX[currentIdx], p->pointsY[currentIdx]);

        /* Find the next point such that it forms a counterclockwise turn with
            the current point and all the other points in the set */
        nextIdx = 0;
        for (pointIdx = 0; pointIdx < numPoints; pointIdx++) {
            if (nextIdx == currentIdx || orientation(p, currentIdx, nextIdx,
                pointIdx) == COUNTERCLOCKWISE) {
                nextIdx = pointIdx;
            }
            /* A basic operation for Jarvis' March is the comparison between
                the angles of points */
            (s->operationCount)++;
        }
        /* Change the current point to be the next point for the next 
            iteration */
        currentIdx = nextIdx;
    }
    /* Repeat this until we return to the starting (leftmost) point */
    while (currentIdx != leftmostIdx);

    /* Done! Return the convex hull */
    s->convexHull = hull;
    return s;
}

/*****************************************************************************/

/* Part B - perform Graham's Scan to construct a convex hull for the given
    problem. */
struct solution *grahamScan(struct problem *p) {
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);

    /* Initialise the operation count */
    s->operationCount = 0;

    /* Record the number of points */
    int numPoints = p->numPoints;

    /* Initialise an empty list to store convex hull points */
    struct linkedList *hull = newList();

    /* Check that there are at least the minimum number of points */
    if (numPoints < MIN_POINTS) {
        s->convexHull = hull;
        return s;
    }

    /* Create an array of the points' indexes to be sorted */
    int *pointsIdx = (int *) malloc(numPoints * sizeof(int));
    assert(pointsIdx);
    for (int i = 0; i < numPoints; i++) {
        pointsIdx[i] = i;
    }

    /* Sort the array above by their points' polar angles with respect to the
        lowest point */
    sortPolarAngle(p, s, pointsIdx, 0, numPoints - 1);

    /* Initialise an empty stack to store indexes of convex hull points */
    struct stack *hullIdx = NULL;

    /* Push the first 3 points in the sorted array to the stack */
    push(&hullIdx, pointsIdx[0]);
    push(&hullIdx, pointsIdx[1]);
    push(&hullIdx, pointsIdx[2]);

    /* Record the stack's elements */
    int poppedItem;

    for (int j = 3; j < numPoints; j++) {
        /* While the current point and the top two points of the stack make
            a non-left turn, pop the top of the stack */
        while (orientation(p, hullIdx->below->item, hullIdx->item,
            pointsIdx[j]) != COUNTERCLOCKWISE) {
            poppedItem = pop(&hullIdx);
        }
        /* Push the current point onto the stack */
        push(&hullIdx, pointsIdx[j]);
    }

    /* Turn the stack of indexes of convex hull points into a list consisting
        of the points themselves */
    while ((poppedItem = pop(&hullIdx)) != -1) {
        insertHead(hull, p->pointsX[poppedItem], p->pointsY[poppedItem]);
    }

    /* Free the pointer to the sorted array of indexes */
    free(pointsIdx);
    pointsIdx = NULL;

    /* Free the stack that stored the indexes convex hull points */
    freeStack(hullIdx);

    /* Done! Return the convex hull */
    s->convexHull = hull;
    return s;
}

/*****************************************************************************/

/* Frees solution structure and all contained information. */
void freeSolution(struct solution *s) {
    if (!s) {
        return;
    }
    if (s->convexHull) {
        freeList(s->convexHull);
    }
    free(s);
    s = NULL;
}

/*****************************************************************************/


