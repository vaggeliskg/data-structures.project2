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
	Pointer stack_bottom; // pointer στο τελευταίο στοιχείο της στοίβας
	int steps;		// steps για να αποθηκεύονται τα βήματα για την άσκηση 3
};


Queue queue_create(DestroyFunc destroy_value) {
	Queue queue = malloc(sizeof(*queue));
	queue->stack = stack_create(NULL);
	queue->steps = 1;
	return queue;
}

int queue_size(Queue queue) {
	queue->steps = 1;
	return stack_size(queue->stack);
	
}

Pointer queue_front(Queue queue) {
	queue->steps = 1;
	return queue->stack_bottom; // το μπροστινό μέρος της ουράς είναι το τελευταίο της στοίβας
}

Pointer queue_back(Queue queue) {
	queue->steps = 1;
	return stack_top(queue->stack);
}

void queue_insert_back(Queue queue, Pointer value) {
	if(stack_size(queue->stack) == 0) {
	stack_insert_top(queue->stack,value);
	queue->stack_bottom = value;		// το πρώτο στοιχείο που μπαίνει στη στοίβα είναι το κάτω-κάτω
	}
	else stack_insert_top(queue->stack,value);
	queue->steps = 1;
}
//κάθε φορά που αφαιρείται ένα στοιχείο μπαίνουν στο vector , βγαίνει το στοιχείο και ξαναμπαίνουν στο stack
void queue_remove_front(Queue queue) {
	int size = queue_size(queue);
	Vector vec = vector_create(0, NULL);
	for(int i = 0 ; i <size ; i++) {
		vector_insert_last(vec, stack_top(queue->stack));
		stack_remove_top(queue->stack);
	}
	vector_remove_last(vec);
	for(int i = vector_size(vec) ; i > 0 ; i--) {
		stack_insert_top(queue->stack,vector_get_at(vec,i- 1));
		if(stack_size(queue->stack) == 1) {
			queue->stack_bottom = stack_top(queue->stack); // ανανέωση του τελευταίου στοιχείου της στοίβας
		}
	}
	queue->steps = size;
}

DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value) {
	queue->steps = 1;
	return stack_set_destroy_value(queue->stack, destroy_value);
}

void queue_destroy(Queue queue) {
	queue->steps = 1;
	stack_destroy(queue->stack);
	free(queue);
}

int queue_steps(Queue queue) {
	return queue->steps;
}
