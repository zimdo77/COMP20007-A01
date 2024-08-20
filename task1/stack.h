/*
    Written by William Sutherland for 
    COMP20007 Assignment 1 2023 Semester 1
    Modified by Grady Fitzpatrick
    
    Header for module which contains stack-related
        data structures and functions.

    Slightly modified by Quoc Khang Do (changes
        were made to be used with integer data.
*/
#ifndef STACK_H
#define STACK_H

struct stack;

/* The data structure of the stack */
struct stack {
    int item;
    struct stack *below;
};

/* Creates the stack */
struct stack *createStack(int item);

/* Note that you are passing a pointer to the pointer of the stack
   into the below arguments */

/* Pops an item from the stack and returns the void pointer */
int pop(struct stack **s);

/* Push an item to the top of the stack */
void push(struct stack **s, int item);

/* Frees the memory associated with the stack */
void freeStack(struct stack *s);

#endif
