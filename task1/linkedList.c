/*
    Skeleton written by Grady Fitzpatrick for 
    COMP20007 Assignment 1 2024 Semester 1
    
    Implementation details for module which contains doubly-linked list 
        specification data structures and functions.
    
    Implemented by Quoc Khang Do
*/

#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct linkedListNode;

/*****************************************************************************/

/*************************/
/* STRUCTURE DEFENITIONS */
/*************************/

struct linkedListNode {
    struct linkedListNode *next;
    struct linkedListNode *prev;
    long double x;
    long double y;
};

/*****************************************************************************/

/********************************/
/* HELPER FUNCTION DECLARATIONS */
/********************************/

struct linkedListNode *newNode();

/*****************************************************************************/

/*******************************/
/* HELPER FUNCTION DEFINITIONS */
/*******************************/

/* Create a new node and return its address. */
struct linkedListNode *newNode() {
    /* Allocate memory for the node */
    struct linkedListNode *node = (struct linkedListNode *) malloc
        (sizeof(struct linkedListNode));
    assert(node);

    /* Initialise the node */
    node->next = NULL;
    node->prev = NULL;

    return node;
}

/*****************************************************************************/

/************************/
/* FUNCTION DEFINITIONS */
/************************/

/* Print out each value in the list from the first value to the final value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting 
    the item. */
void traverseRingForward(struct linkedList *list) {
    assert(list);

    /* Check if list is empty */
    if (list->size == 0) {
        printf("Cannot traverse ring. Convex hull is empty.");
        return;
    }
    
    struct linkedListNode *node = list->head;

    while (node != NULL) {
        printf("(%Lf, %Lf) ", node->x, node->y);
        node = node->next;
    }
}

/*****************************************************************************/

/* Print out first the first item of the list, then print out each value in 
    the list from the final value to the second value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting 
    the item. */
void traverseRingBackwards(struct linkedList *list) {
    assert(list);

    /* Check if list is empty */
    if (list->size == 0) {
        printf("Cannot traverse ring backwards. Convex hull is empty.");
        return;
    }

    struct linkedListNode *node = list->tail;

    printf("(%Lf, %Lf) ", list->head->x, list->head->y);
    while (node != list->head) {
        printf("(%Lf, %Lf) ", node->x, node->y);
        node = node->prev;
    }
}

/*****************************************************************************/

/* Return a new empty list. */
struct linkedList *newList() {

    /* Allocate memory for the linked list */
    struct linkedList *list = (struct linkedList *) malloc
        (sizeof(struct linkedList));
    assert(list);

    /* Initialise the linked list */
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

/*****************************************************************************/

/* Insert the given x, y pair at the head of the list */
void insertHead(struct linkedList *list, long double x, long double y) {
    assert(list);

    /* Create a new node and insert its coordinate values */
    struct linkedListNode *node = newNode();
    node->x = x;
    node->y = y;

    if (list->size == 0) {
    /* If the list was previously empty, the new node also becomes the tail */
        list->tail = node;
    } else {
    /* Otherwise (if the list has one or more elements already in it), the
        element NEXT of the new node is the list's head, and the element
        PREVIOUS to the list's head is the new node */
        node->next = list->head;
        list->head->prev = node;
    }

    /* The new node becomes the new head */
    list->head = node;

    /* Update list size */
    (list->size)++;
}

/*****************************************************************************/

/* Insert the given x, y pair at the tail of the list */
void insertTail(struct linkedList *list, long double x, long double y) {
    assert(list);

    /* Create a new node and insert its coordinate values */
    struct linkedListNode *node = newNode();
    node->x = x;
    node->y = y;

    if (list->size == 0) {
    /* If the list was previously empty, the new node also becomes the head */
        list->head = node;
    } else {
    /* Otherwise (if the list has one or more elements already in it), the
        element PREVIOUS to the new node is the list's tail, and the
        element NEXT of the list's tail is the new node */
        node->prev = list->tail;
        list->tail->next = node;
    }

    /* The new node becomes the new tail */
    list->tail = node;

    /* Update list size */
    (list->size)++;
}

/*****************************************************************************/

/* Free all items in the given list. */
void freeList(struct linkedList *list) {
    /* IMPLEMENT HERE */ ////
    assert(list);

    /* Free each node within the list */
    struct linkedListNode *node, *next;
    node = list->head;
    while (node) {
        next = node->next;
        free(node);
        node = next;
    }

    /* Free the linked list itself */
    free(list);
    list = NULL;
}

/*****************************************************************************/



