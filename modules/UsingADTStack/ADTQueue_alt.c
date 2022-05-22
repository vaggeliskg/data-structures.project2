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
	Pointer stack_bottom;	// pointer στο κάτω-κάτω στοιχείο της στοίβας 
	Stack stack_for_rev;	//δεύτερη στοίβα για τις διαγραφές 
	Pointer stack_bottom_rev; //pointer στο κάτω-κάτω στοιχείο της 2ης στοίβας
	int steps;	//αποθήκευση βημάτων κάθε συνάρτησης
};


Queue queue_create(DestroyFunc destroy_value) {
	Queue queue = malloc(sizeof(*queue));
	queue->stack = stack_create(NULL);
	queue->stack_for_rev = stack_create(NULL);
	queue->stack_bottom_rev = NULL;
	queue->stack_bottom = NULL;
	queue->steps = 1;
	return queue;
}
//το μέγεθος της ουράς τώρα εξαρτάται και από τις 2 στοίβες
int queue_size(Queue queue) {
	queue->steps = 1;
	return stack_size(queue->stack) + stack_size(queue->stack_for_rev);
}
//περιπτώσεις του μπροστινού στοιχείου της ουράς ανάλογα με την κατανομή των στοιχείων στις στοίβες
Pointer queue_front(Queue queue) {
	queue->steps = 1;
	if(stack_size(queue->stack_for_rev) !=0 )
	return stack_top(queue->stack_for_rev);
	else return queue->stack_bottom;
}
//περιπτώσεις και για το πίσω στοιχείο για τον ίδιο λόγο
Pointer queue_back(Queue queue) {
	queue->steps = 1;
	if(stack_size(queue->stack) == 0 && stack_size(queue->stack_bottom_rev) != 0)
	return queue->stack_bottom_rev;
	else 
	return stack_top(queue->stack);
	
}
//λογική ίδια με αυτή της άσκησης 1 για την insert
void queue_insert_back(Queue queue, Pointer value) {
	queue->steps = 1;
	if(stack_size(queue->stack) == 0) {
		stack_insert_top(queue->stack,value);
		queue->stack_bottom = value;
	}
	else stack_insert_top(queue->stack,value);
}
//εδώ πλέον έχουμε 2 στοίβες και όποτε απαιτείται διαγραφή τα στοιχεία μεταφέρονται στη 2η στοίβα
//όπου και παραμένουν μέχρι να αδειάσει, τώρα δηλαδή τα στοιχεία μου μπορεί να βρίσκονται σε 2 στοίβες
//στην 1η γίνονται οι εισαγωγές και στη 2η γίνονται οι διαγραφές όσο έχει στοιχεία.Επειδή λοιπόν έχω
//2 στοίβες χρειάζομαι και περιπτώσεις ανάλογα με το πως ειναι κατανεμημένα τα στοιχεία 
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
		queue->steps = size;
		return;
	}
	else if(size == 0 && size_r !=0) {
	queue->steps = 1;	
	stack_remove_top(queue->stack_for_rev);
	return;
	}
	else if(size != 0 && size_r !=0) {
	queue->steps = 1;	
	stack_remove_top(queue->stack_for_rev);
	return;
	}
}

DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value) {
	queue->steps = 1;
	return stack_set_destroy_value(queue->stack, destroy_value);
}

void queue_destroy(Queue queue) {
	queue->steps = 1;	
	stack_destroy(queue->stack);
	stack_destroy(queue->stack_for_rev);
	free(queue);
}
int queue_steps(Queue queue) {
	return queue->steps;
}