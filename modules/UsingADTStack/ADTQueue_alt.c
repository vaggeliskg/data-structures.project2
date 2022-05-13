///////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Queue μέσω του ADT Stack (dependent data structure)
//
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "ADTQueue.h"
#include "ADTStack.h"
#include "ADTVector.h"


// Ένα Queue είναι pointer σε αυτό το struct.
struct queue {
	Stack stack;
	Pointer stack_bottom;
	Stack stack_for_rev;
	Pointer stack_bottom_rev;
};


Queue queue_create(DestroyFunc destroy_value) {
	Queue queue = malloc(sizeof(*queue));
	queue->stack = stack_create(NULL);
	queue->stack_for_rev = stack_create(NULL);
	return queue;
}

int queue_size(Queue queue) {
	return stack_size(queue->stack) + stack_size(queue->stack_for_rev);
}

Pointer queue_front(Queue queue) {
	if(stack_size(queue->stack_for_rev) !=0 )
	return stack_top(queue->stack_for_rev);
	else return queue->stack_bottom;
}

Pointer queue_back(Queue queue) {
	if(stack_size(queue->stack_bottom_rev) == 0 && stack_size(queue->stack) != 0 )
	return stack_top(queue->stack);
	else if(stack_size(queue->stack) == 0 && stack_size(queue->stack_bottom_rev) != 0)
	return queue->stack_bottom_rev;
	else //if(stack_size(queue->stack) != 0 && stack_size(queue->stack_bottom_rev) != 0)
	return stack_top(queue->stack);
	
}

void queue_insert_back(Queue queue, Pointer value) {
	if(stack_size(queue->stack) == 0) {
		stack_insert_top(queue->stack,value);
		queue->stack_bottom = value;
	}
	else stack_insert_top(queue->stack,value);
}

void queue_remove_front(Queue queue) {
	int size = stack_size(queue->stack);
	int size_r = stack_size(queue->stack_for_rev);
	if(size != 0 && size_r ==0 ) {
		for(int i = 0 ; i < size ; i++) {
			if(stack_size(queue->stack_for_rev) == 0) {
			stack_insert_top(queue->stack_for_rev, stack_top(queue->stack));
			queue->stack_bottom_rev = stack_top(queue->stack_for_rev);
			stack_remove_top(queue->stack);
			}
			else if(stack_size(queue->stack_for_rev) != 0) {
				stack_insert_top(queue->stack_for_rev, stack_top(queue->stack));
				stack_remove_top(queue->stack);
			}
		}
		queue->stack_bottom = NULL;
		stack_remove_top(queue->stack_for_rev);
		return;
	}
	else if(size == 0 && size_r !=0) {
	stack_remove_top(queue->stack_for_rev);
	return;
	}
}

DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value) {
	return stack_set_destroy_value(queue->stack, destroy_value);
}

void queue_destroy(Queue queue) {
	stack_destroy(queue->stack);
	stack_destroy(queue->stack_for_rev);
	free(queue);
}
