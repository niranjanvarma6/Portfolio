/* In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Niranjan Varma
 * Email: varman@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */

struct pq
{
    struct dynarray * dy;
};

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */

struct pq* pq_create()
{
    struct pq* pri = malloc(sizeof(struct pq));
    pri->dy = dynarray_create();
    return pri;
}

/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */

void pq_free(struct pq* pq)
{
    dynarray_free(pq->dy);
    free(pq);
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */

int pq_isempty(struct pq* pq)
{
    if(dynarray_size(pq->dy) == 0){
        return 1;
    }
    return 0;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */

//Necessary to access the priority and value pointers of the dynamic array
struct pq_node{
    int p;
    void * v;

};

void pq_insert(struct pq* pq, void* value, int priority)
{
    struct pq_node* pq_v = malloc(sizeof(struct pq_node));
    pq_v->v = value;
    pq_v->p = priority;
    dynarray_insert(pq->dy, pq_v);
    int next = dynarray_size(pq->dy)-1;
    int parent = 0;
    struct pq_node *temp;
    while(parent>=0){
        parent=(next-1)/2; 
		temp=dynarray_get(pq->dy, parent); 
		if(temp->p > pq_v->p){ 
			dynarray_set(pq->dy, parent, pq_v);
			dynarray_set(pq->dy, next, temp);
			next=parent; 
		}
		else{
			break;
		}
    }
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */

void* pq_first(struct pq* pq)
{
    struct pq_node* pop;
    pop=dynarray_get(pq->dy, 0);
    return pop->v;
}

/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */

int pq_first_priority(struct pq* pq)
{
    struct pq_node* pop;
    pop=dynarray_get(pq->dy, 0);
    return pop->p;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */

void* pq_remove_first(struct pq* pq)
{
    struct pq_node* first = dynarray_get(pq->dy, 0);
    int last_val = dynarray_size(pq->dy)-1;
    struct pq_node* last = dynarray_get(pq->dy, last_val);
    dynarray_set(pq->dy, 0, last);
    dynarray_remove(pq->dy, last_val);
    int current=0;
    int lowest; 
	int left;
	int right;
	struct pq_node* temp1; 
	struct pq_node* temp2; 
    int n = 0;
    //reordering of p(priority)
	while(n == 0){ 
		left=(2*current)+1; 
		right=(2*current)+2;  

        //If statements used to check the left value of the current value to be compared with the last value. If less than, the left checks with right for priority to see which is the value with the lowest priority  
		if(left>last_val){
			n=1;
            break;
        }
		else if(left==last_val){
			lowest=left;
        }
		else{
			if(((struct pq_node*)dynarray_get(pq->dy, left))->p >((struct pq_node*)dynarray_get(pq->dy, right))->p){
				lowest=right; 
            }
			else{
				lowest=left;
            } 
		}

        //Assigns the temp values into the dynamic array if the current value is greater than the lowest priority value
		if((((struct pq_node*)dynarray_get(pq->dy, current))->p)>(((struct pq_node*)dynarray_get(pq->dy, lowest))->p)){ 
			temp1=dynarray_get(pq->dy,lowest); 
            temp2=dynarray_get(pq->dy, current);
            dynarray_set(pq->dy, current, temp1);
			dynarray_set(pq->dy, lowest, temp2);
			current=lowest;
		}
		else{
			n=1;
            break;
        }
	}
    void*pq_node=first->v;
    free(first); 
    return pq_node;
}