/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2024
*/
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <limits.h>
#include "graph.h"
#include "utils.h"
#include "pq.h"

#define INITIALEDGES 32
#define MARKED_VISITED 1
#define DAMAGE_TAKEN_PER_STEP 1
#define UNDEF_STEPS INT_MAX
#define UNDEF_COST INT_MAX
#define UNDEF_PC_INCREASE INT_MAX

struct edge;

/* Definition of a graph. */
struct graph {
    int numVertices;
    int numEdges;
    int allocedEdges;
    struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
    int start;
    int end;
    int cost;
};

struct graph *newGraph(int numVertices) {
    struct graph *g = (struct graph *) malloc(sizeof(struct graph));
    assert(g);
    /* Initialise edges. */
    g->numVertices = numVertices;
    g->numEdges = 0;
    g->allocedEdges = 0;
    g->edgeList = NULL;
    return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost) {
    assert(g);
    struct edge *newEdge = NULL;
    /* Check we have enough space for the new edge. */
    if ((g->numEdges + 1) > g->allocedEdges) {
        if (g->allocedEdges == 0) {
            g->allocedEdges = INITIALEDGES;
        } else {
            (g->allocedEdges) *= 2;
        }
        g->edgeList = (struct edge **) realloc(g->edgeList,
            sizeof(struct edge *) * g->allocedEdges);
        assert(g->edgeList);
    }

    /* Create the edge */
    newEdge = (struct edge *) malloc(sizeof(struct edge));
    assert(newEdge);
    newEdge->start = start;
    newEdge->end = end;
    newEdge->cost = cost;

    /* Add the edge to the list of edges. */
    g->edgeList[g->numEdges] = newEdge;
    (g->numEdges)++;
}

/* Returns a new graph which is a deep copy of the given graph (which must be 
    freed with freeGraph when no longer used). */
struct graph *duplicateGraph(struct graph *g) {
    struct graph *copyGraph = (struct graph *) malloc(sizeof(struct graph));
    assert(copyGraph);
    copyGraph->numVertices = g->numVertices;
    copyGraph->numEdges = g->numEdges;
    copyGraph->allocedEdges = g->allocedEdges;
    copyGraph->edgeList = (struct edge **) malloc(sizeof(struct edge *) * g->allocedEdges);
    assert(copyGraph->edgeList || copyGraph->numEdges == 0);
    int i;
    /* Copy edge list. */
    for (i = 0; i < g->numEdges; i++) {
        struct edge *newEdge = (struct edge *) malloc(sizeof(struct edge));
        assert(newEdge);
        newEdge->start = (g->edgeList)[i]->start;
        newEdge->end = (g->edgeList)[i]->end;
        newEdge->cost = (g->edgeList)[i]->cost;
        (copyGraph->edgeList)[i] = newEdge;
    }
    return copyGraph;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph *g) {
    int i;
    for (i = 0; i < g->numEdges; i++) {
        free((g->edgeList)[i]);
    }
    if (g->edgeList) {
        free(g->edgeList);
    }
    free(g);
}

struct solution *graphSolve(struct graph *g, enum problemPart part,
    int numLocations, int startingLocation, int finalLocation) {
    struct solution *solution = (struct solution *)
        malloc(sizeof(struct solution));
    assert(solution);
    int i;
    if (part == PART_A) {
        /* 2A SOLUTION: use breadth-first-search (BFS) to find the shortest path
            between a starting and ending location. */
        solution->damageTaken = -1;

        /* Initialise an empty queue */
        /* The queue is implemented using a minimum priority queue. So use a key
            to incrementally increase the priority value as elements are being
            added (to maintain the FIFO property of normal queues). */
        struct pq *queue = newPQ();
        int key = 0;
        /* Initialise an array to mark visited nodes. Mark 0 for unvisited, and
            1 for visited. No nodes are visited yet, so initialize all nodes to 0
            using calloc. */
        int *marked = (int *) calloc(numLocations, sizeof(int)); 
        /* Initialise an array to count number of steps from the starting node
            to each node */
        int *steps = (int *) malloc(numLocations * sizeof(int));
        assert(steps);

        /* The number of steps from the first node to any unreachable or
            unvisited node is undefined */
        for (i = 0; i < numLocations; i++) {
            steps[i] = UNDEF_STEPS;
        }

        /* Steps at the starting node is 0 */
        steps[startingLocation] = 0;

        /* Enqueue the starting node and mark it as visited */
        enqueue(queue, startingLocation, key);
        key++;
        marked[startingLocation] = MARKED_VISITED;

        /* Do BFS until the ending node is found */
        int location, neighbour;
        while (!emptyPQ(queue)) {
            location = deleteMin(queue);
            if (location == finalLocation) {
            /* Ending node is found. Update solution. */
                solution->damageTaken = DAMAGE_TAKEN_PER_STEP * steps[location];
                break;
            }
            for (i = 0; i < g->numEdges; i++) {
            /* Find every unvisited neighbouring node to the node that was dequeued */
                if (((g->edgeList)[i]->start == location && !marked[(g->edgeList)[i]->end]) ||
                    ((g->edgeList)[i]->end == location && !marked[(g->edgeList)[i]->start])) {
                    if ((g->edgeList)[i]->start == location) {
                        neighbour = (g->edgeList)[i]->end;
                    } else {
                        neighbour = (g->edgeList)[i]->start;
                    }
                    /* Update the node's step count, mark it as visited, and
                        add it to the queue */
                    steps[neighbour] = steps[location] + 1;
                    marked[neighbour] = MARKED_VISITED;
                    enqueue(queue, neighbour, key);
                    key++;
                }    
            }
        }

        /* Free allocated memory */
        freePQ(queue);

        free(marked);
        marked = NULL;

        free(steps);
        steps = NULL;

        /*************************************************************************/

    } else if (part == PART_B) {
        /* 2B SOLUTION: implement Dijkstra's algorithm to find the cheapest path
            between a starting and ending location. */
        solution->totalCost = -1;

        /* Initialise an array to count the total cost from the starting node to
            each node. */
        int *cost = (int *) malloc(numLocations * sizeof(int));
        assert(cost);

        /* The cost to go from the first node to any unreachable or unvisited node
            is undefined */
        for (i = 0; i < numLocations; i++) {
            cost[i] = UNDEF_COST;
        }

        /* Cost at the starting node is 0 */
        cost[startingLocation] = 0;

        /* Initialise a priority queue that prioritises nodes with lower costs */
        struct pq *pq = newPQ();
        for (i = 0; i < numLocations; i++) {
            enqueue(pq, i, cost[i]);
        }

        int location, neighbour;
        while (!emptyPQ(pq)) {
            /* Eject the node with the lowest cost */
            location = deleteMin(pq);
            for (i = 0; i < g->numEdges; i++) {
            /* Find every neighbouring node to the node that was ejected */
                if (((g->edgeList)[i]->start == location && inPQ(pq, (g->edgeList)[i]->end)) ||
                    ((g->edgeList)[i]->end == location && inPQ(pq, (g->edgeList)[i]->start))) {
                    if ((g->edgeList)[i]->start == location) {
                        neighbour = (g->edgeList)[i]->end;
                    } else {
                        neighbour = (g->edgeList)[i]->start;
                    }
                    /* For each neighbouring node, update the current cost with
                        the new cost if its new cost is cheaper */
                    int newCost = cost[location] + (g->edgeList)[i]->cost;
                    if (newCost < cost[neighbour]) {
                        cost[neighbour] = newCost;
                        updatePQ(pq, neighbour, cost[neighbour]);
                    }
                }
            }    
        }

        /* Update solution */
        solution->totalCost = cost[finalLocation];

        /* Free allocated memory */
        freePQ(pq);

        free(cost);
        cost = NULL;

        /*************************************************************************/
    
    } else if(part == PART_C) {
        /* 2C SOLUTION: implement Prim's algorithm to find the minimum spanning
            tree of the graph */
        solution->artisanCost = -1;

        /* Initialise an array to store, for each node, the cost 
            of adding it to the spanning tree */
        int *cost = (int *) malloc(numLocations * sizeof(int));
        assert(cost);

        /* The cost to add to an unreachable or unvisited node is undefined */
        for (i = 0; i < numLocations; i++) {
            cost[i] = UNDEF_COST;
        }  

        /* Cost at the starting node is 0 */
        cost[startingLocation] = 0;
    
        /* Initialise a priority queue that prioritises nodes with lower costs */
        struct pq *pq = newPQ();
        for (i = 0; i < numLocations; i++) {
            enqueue(pq, i, cost[i]);
        }

        int location, neighbour;
        while (!emptyPQ(pq)) {
            /* Eject the node with the lowest cost */
            location = deleteMin(pq);
            /* Find every neighbouring node to the node that was ejected */
            for (i = 0; i < g->numEdges; i++) {
                if (((g->edgeList)[i]->start == location && inPQ(pq, (g->edgeList)[i]->end)) ||
                    ((g->edgeList)[i]->end == location && inPQ(pq, (g->edgeList)[i]->start))) {
                    if ((g->edgeList)[i]->start == location) {
                        neighbour = (g->edgeList)[i]->end;
                    } else {
                        neighbour = (g->edgeList)[i]->start;
                    }
                    /* For each neighbouring node, update the current cost with
                        the new cost if its new cost is cheaper */
                    int newCost = (g->edgeList)[i]->cost;
                    if (newCost < cost[neighbour]) {
                        cost[neighbour] = newCost;
                        updatePQ(pq, neighbour, cost[neighbour]);
                    }
                }
            }
        }

        /* Sum up all the costs of the tree edges */
        int totalCost = 0;
        for (i = 0; i < numLocations; i++) {
            totalCost += cost[i];
        }

        /* Update solution */
        solution->artisanCost = totalCost;

        /* Free allocated memory */
        freePQ(pq);

        free(cost);
        cost = NULL;

        /*************************************************************************/
        
    } else {
        /* 2D SOLUTION: implement an adaptation of Dijkstra's algorithm to find 
            the cheapest path between a starting and ending location */
        solution->totalPercentage = -1;

        /* Initialise an array to store the "accumulated percentage increase" from
            the starting node to each node. */
        double *percentIncrease = (double *) malloc(numLocations * sizeof(double));
        assert(percentIncrease);

        /* The "accumulated percentage increase" to an unreachable or unvisited 
            node is undefined */
        for (i = 0; i < numLocations; i++) {
            percentIncrease[i] = UNDEF_PC_INCREASE;
        }

        /* Percentage increase at the starting node is 0 */
        percentIncrease[startingLocation] = 0;

        /* Initialise a priority queue that prioritises nodes with lower
            "accumulated percentage increase" values */
        struct pq *pq = newPQ();
        for (i = 0; i < numLocations; i++) {
            enqueue(pq, i, percentIncrease[i]);
        }

        int location, neighbour;
        while (!emptyPQ(pq)) {
            /* Eject the node with the lowest "accumulated percentage increase" 
                value */
            location = deleteMin(pq);
            /* Find every neighbouring node to the node that was ejected */
            for (i = 0; i < g->numEdges; i++) {
                if (((g->edgeList)[i]->start == location && inPQ(pq, (g->edgeList)[i]->end)) ||
                    ((g->edgeList)[i]->end == location && inPQ(pq, (g->edgeList)[i]->start))) {
                    if ((g->edgeList)[i]->start == location) {
                        neighbour = (g->edgeList)[i]->end;
                    } else {
                        neighbour = (g->edgeList)[i]->start;
                    } 
                    /* For each neighbouring node, update its "accumulated
                        percentage increase" if its new multiplier value is less 
                        than its current multiplier value */
                    double locationMultiplier = 1 + percentIncrease[location] / 100;
                    double neighbourMultiplier = 1 + percentIncrease[neighbour] / 100;
                    double newMultiplier = locationMultiplier * (1 + ((double)((g->edgeList)[i]->cost) / 100));
                    if (newMultiplier < neighbourMultiplier) {
                        percentIncrease[neighbour] = (newMultiplier - 1) * 100;
                        updatePQ(pq, neighbour, percentIncrease[neighbour]);
                    }
                }
            }    
        }

        /* Update solution */
        solution->totalPercentage = (int)(percentIncrease[finalLocation]);

        /* Free allocated memory */
        freePQ(pq);

        free(percentIncrease);
        percentIncrease = NULL;

        /*************************************************************************/

    }
    return solution;
}

