///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Set μέσω Binary Search Tree (BST)
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>
#include "ADTVector.h"
#include "ADTSet.h"
#include <stdio.h>


// Υλοποιούμε τον ADT Set μέσω BST, οπότε το struct set είναι ένα Δυαδικό Δέντρο Αναζήτησης.
struct set {
	SetNode root;				// η ρίζα, NULL αν είναι κενό δέντρο
	int size;					// μέγεθος, ώστε η set_size να είναι Ο(1)
	CompareFunc compare;		// η διάταξη
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο του set
};

// Ενώ το struct set_node είναι κόμβος ενός Δυαδικού Δέντρου Αναζήτησης
struct set_node {
	SetNode left, right;		// Παιδιά
	Pointer value;
	int height;
	int size;
};


// Παρατηρήσεις για τις node_* συναρτήσεις
// - είναι βοηθητικές (κρυφές από το χρήστη) και υλοποιούν διάφορες λειτουργίες πάνω σε κόμβους του BST.
// - είναι αναδρομικές, η αναδρομή είναι γενικά πολύ βοηθητική στα δέντρα.
// - όσες συναρτήσεις _τροποποιούν_ το δέντρο, ουσιαστικά ενεργούν στο _υποδέντρο_ με ρίζα τον κόμβο node, και επιστρέφουν τη νέα
//   ρίζα του υποδέντρου μετά την τροποποίηση. Η νέα ρίζα χρησιμοποιείται από την προηγούμενη αναδρομική κλήση.
//
// Οι set_* συναρτήσεις (πιο μετά στο αρχείο), υλοποιούν τις συναρτήσεις του ADT Set, και είναι απλές, καλώντας τις αντίστοιχες node_*.


// Δημιουργεί και επιστρέφει έναν κόμβο με τιμή value (χωρίς παιδιά)

static SetNode node_create(Pointer value) {
	SetNode node = malloc(sizeof(*node));
	node->left = NULL;
	node->right = NULL;
	node->value = value;
	node->height = 0;
	node->size = 1;
	return node;
}

// Επιστρέφει τον μικρότερο κόμβο του υποδέντρου με ρίζα node

static SetNode node_find_min(SetNode node) {
	return node != NULL && node->left != NULL
		? node_find_min(node->left)				// Υπάρχει αριστερό υποδέντρο, η μικρότερη τιμή βρίσκεται εκεί
		: node;									// Αλλιώς η μικρότερη τιμή είναι στο ίδιο το node
}
//Επιστρέφει ένα ισοζυγισμένο δέντρο με τις τιμές του ταξινομημένου vector
Set set_create_from_sorted_values(CompareFunc compare, DestroyFunc destroy_value, Vector values){
	assert(compare !=NULL);

	Set set = malloc(sizeof(*set));
	set->root = NULL;
	set->compare = compare;
	set->destroy_value = destroy_value;
	int start = 0;
	int end = vector_size(values) - 1;
	set->root = set_rec(values, set, start, end);
	set->size = vector_size(values);	 
	return set;

}
//καλείται μόνο απο την set_create_from_sorted_values, λειτουργεί αναδρομικά και κατασκευάζει
//το ισοζυγισμένο δέντρο
SetNode set_rec(Vector values, Set set, int start, int end) {
	if(start > end)
		return NULL;
	else {
		int mid_pos = (start + end) / 2;
		Pointer mid = vector_get_at(values, mid_pos);
		SetNode node = node_create(mid);
		node->value = mid;
		//printf("~ node = %d\n", *(int*)mid);
		node->left = set_rec(values,set,start,mid_pos - 1);
		node->right = set_rec(values,set,mid_pos+1,end);
		return node;
	}
}
//Επιστρέφει τον πατέρα του κόμβου με τιμή value
static SetNode find_parent(Set set, SetNode node, Pointer key) {
	if(key == set->root->value) {
		return set->root;
	}
	if(node == NULL) {
		return NULL;
	}
	SetNode parent = NULL;
	while(node != NULL) {
		if(key < node->value) {
			parent = node;
			node = node->left;
		}
		else if(key > node->value) {
			parent = node;
			node = node->right;
		}
		else if (key == node->value)
		return parent;
	}
	return NULL;
}


//Επιστρέφει τη μέγιστη τιμή μεταξύ 2 ακεραίων
static int int_max(int a, int b) {
 	return (a > b) ? a : b ;
}
//Επιστρέφει το ύψος που βρίσκεται ο κόμβος στο δέντρο
static int node_height(SetNode node) {
	if (!node) return 0;
	return node->height;
}
//Ενημερώνει το ύψος ενός κόμβου
static void node_update_height(SetNode node) {
	node->height = 1 + int_max(node_height(node->left), node_height(node->right));
}
//Επιστρέφει το μέγεθος ενός κόμβου
static int node_size(SetNode node) {
	if(!node) return 0;
	return node->size;
}
//Ενημερώνει το μέγεθος ενός κόμβου
static void node_update_size(SetNode node) {
	node->size = 1 + node_size(node->left) + node_size(node->right);
}

//Ελέγχει(αν το ύψος του node ειναι >= 4) αν το υποδέντρο του node με ρίζα τον ίδιο χρειάζεται balance
//Αν χρειάζεται επιστρέφει 1, αν δε χρειάζεται επιστρέφει 0 
static int check_balance(SetNode node) {
	if(node->height >= 4) {
		if(node_size(node->left) > (2 / 3) * node_size(node) ||  node_size(node->right) > (2 / 3) * node_size(node) ) {
			return 1;
		}
		else return 0;
	}
	else return 0;
}
//Η check_route ανιχνεύει ΜΟΝΟ το path του δέντρου στο οποίο γίνεται insert ή remove του value
//ελέγχοντας σε κάθε κόμβο του path αν χρείαζεται balance από εκεί και κάτω(μέσω της check_balance)
//Αν χρειάζεται επιστρέφει τη ρίζα του υποδέντρου που θέλει balance αλλιώς NULL 
static SetNode check_route(SetNode node, Pointer value, Set set) {
	CompareFunc compare = set->compare;
	int compare_res = compare(value, node->value);
	int check = 0;
	check = check_balance(node);
	if(check) {
		return node;
	}
	if(compare_res == 0) {
		return NULL;
	}
	else if(compare_res < 0) {
		node = check_route(node->left,value,set);
	}
	else {
		node = check_route(node->right,value,set);
	}	
	return NULL;
}


// Επιστρέφει τον κόμβο με τιμή ίση με value στο υποδέντρο με ρίζα node, διαφορετικά NULL

static SetNode node_find_equal(SetNode node, CompareFunc compare, Pointer value) {
	// κενό υποδέντρο, δεν υπάρχει η τιμή
	if (node == NULL)
		return NULL;
	
	// Το πού βρίσκεται ο κόμβος που ψάχνουμε εξαρτάται από τη διάταξη της τιμής
	// value σε σχέση με την τιμή του τρέχοντος κόμβο (node->value)
	//
	int compare_res = compare(value, node->value);			// αποθήκευση για να μην καλέσουμε την compare 2 φορές
	if (compare_res == 0)									// value ισοδύναμη της node->value, βρήκαμε τον κόμβο
		return node;
	else if (compare_res < 0)								// value < node->value, ο κόμβος που ψάχνουμε είναι στο αριστερό υποδέντρο
		return node_find_equal(node->left, compare, value);
	else													// value > node->value, ο κόμβος που ψάχνουμε είνια στο δεξιό υποδέντρο
		return node_find_equal(node->right, compare, value);
}

// Επιστρέφει τον μεγαλύτερο κόμβο του υποδέντρου με ρίζα node

static SetNode node_find_max(SetNode node) {
	return node != NULL && node->right != NULL
		? node_find_max(node->right)			// Υπάρχει δεξί υποδέντρο, η μεγαλύτερη τιμή βρίσκεται εκεί
		: node;									// Αλλιώς η μεγαλύτερη τιμή είναι στο ίδιο το node
}

// Επιστρέφει τον προηγούμενο (στη σειρά διάταξης) του κόμβου target στο υποδέντρο με ρίζα node,
// ή NULL αν ο target είναι ο μικρότερος του υποδέντρου. Το υποδέντρο πρέπει να περιέχει τον κόμβο
// target, οπότε δεν μπορεί να είναι κενό.

static SetNode node_find_previous(SetNode node, CompareFunc compare, SetNode target) {
	if (node == target) {
		// Ο target είναι η ρίζα του υποδέντρου, o προηγούμενός του είναι ο μεγαλύτερος του αριστερού υποδέντρου.
		// (Aν δεν υπάρχει αριστερό παιδί, τότε ο κόμβος με τιμή value είναι ο μικρότερος του υποδέντρου, οπότε
		// η node_find_max θα επιστρέψει NULL όπως θέλουμε.)
		return node_find_max(node->left);

	} else if (compare(target->value, node->value) < 0) {
		// Ο target είναι στο αριστερό υποδέντρο, οπότε και ο προηγούμενός του είναι εκεί.
		return node_find_previous(node->left, compare, target);

	} else {
		// Ο target είναι στο δεξί υποδέντρο, ο προηγούμενός του μπορεί να είναι επίσης εκεί,
		// αν όχι ο προηγούμενός του είναι ο ίδιος ο node.
		SetNode res = node_find_previous(node->right, compare, target);
		return res != NULL ? res : node;
	}
}

// Επιστρέφει τον επόμενο (στη σειρά διάταξης) του κόμβου target στο υποδέντρο με ρίζα node,
// ή NULL αν ο target είναι ο μεγαλύτερος του υποδέντρου. Το υποδέντρο πρέπει να περιέχει τον κόμβο
// target, οπότε δεν μπορεί να είναι κενό.

static SetNode node_find_next(SetNode node, CompareFunc compare, SetNode target) {
	if (node == target) {
		// Ο target είναι η ρίζα του υποδέντρου, o επόμενός του είναι ο μικρότερος του δεξιού υποδέντρου.
		// (Aν δεν υπάρχει δεξί παιδί, τότε ο κόμβος με τιμή value είναι ο μεγαλύτερος του υποδέντρου, οπότε
		// η node_find_min θα επιστρέψει NULL όπως θέλουμε.)
		return node_find_min(node->right);

	} else if (compare(target->value, node->value) > 0) {
		// Ο target είναι στο δεξί υποδέντρο, οπότε και ο επόμενός του είναι εκεί.
		return node_find_next(node->right, compare, target);

	} else {
		// Ο target είναι στο αριστερό υποδέντρο, ο επόμενός του μπορεί να είναι επίσης εκεί,
		// αν όχι ο επόμενός του είναι ο ίδιος ο node.
		SetNode res = node_find_next(node->left, compare, target);
		return res != NULL ? res : node;
	}
}

// Αν υπάρχει κόμβος με τιμή ισοδύναμη της value, αλλάζει την τιμή του σε value, διαφορετικά προσθέτει
// νέο κόμβο με τιμή value. Επιστρέφει τη νέα ρίζα του υποδέντρου, και θέτει το *inserted σε true
// αν έγινε προσθήκη, ή false αν έγινε ενημέρωση.

static SetNode node_insert(Set set, SetNode node, CompareFunc compare, Pointer value, bool* inserted, Pointer* old_value) {
	// Αν το υποδέντρο είναι κενό, δημιουργούμε νέο κόμβο ο οποίος γίνεται ρίζα του υποδέντρου
	if (node == NULL) {
		*inserted = true;			// κάναμε προσθήκη
		return node_create(value);
	}

	// Το που θα γίνει η προσθήκη εξαρτάται από τη διάταξη της τιμής
	// value σε σχέση με την τιμή του τρέχοντος κόμβου (node->value)
	//
	int compare_res = compare(value, node->value);
	if (compare_res == 0) {
		// βρήκαμε ισοδύναμη τιμή, κάνουμε update
		*inserted = false;
		*old_value = node->value;
		node->value = value;

	} else if (compare_res < 0) {
		// value < node->value, συνεχίζουμε αριστερά.
		node->left = node_insert(set,node->left, compare, value, inserted, old_value);

	} else {
		// value > node->value, συνεχίζουμε δεξιά
		node->right = node_insert(set,node->right, compare, value, inserted, old_value);
	}
	//Ενημερώνω το ύψος και το μέγεθος κάθε φορά που προσθέτω έναν κόμβο
	node_update_height(node);
	node_update_size(node);
	return node;
}
// Αφαιρεί και αποθηκεύει στο min_node τον μικρότερο κόμβο του υποδέντρου με ρίζα node.
// Επιστρέφει τη νέα ρίζα του υποδέντρου.

static SetNode node_remove_min(SetNode node, SetNode* min_node) {
	if (node->left == NULL) {
		// Δεν έχουμε αριστερό υποδέντρο, οπότε ο μικρότερος είναι ο ίδιος ο node
		*min_node = node;
		return node->right;		// νέα ρίζα είναι το δεξιό παιδί

	} else {
		// Εχουμε αριστερό υποδέντρο, οπότε η μικρότερη τιμή είναι εκεί. Συνεχίζουμε αναδρομικά
		// και ενημερώνουμε το node->left με τη νέα ρίζα του υποδέντρου.
		node->left = node_remove_min(node->left, min_node);
		return node;			// η ρίζα δεν μεταβάλλεται
	}
}

// Διαγράφει το κόμβο με τιμή ισοδύναμη της value, αν υπάρχει. Επιστρέφει τη νέα ρίζα του
// υποδέντρου, και θέτει το *removed σε true αν έγινε πραγματικά διαγραφή.

static SetNode node_remove(SetNode node, CompareFunc compare, Pointer value, bool* removed, Pointer* old_value) {
	if (node == NULL) {
		*removed = false;		// κενό υποδέντρο, δεν υπάρχει η τιμή
		return NULL;
	}

	int compare_res = compare(value, node->value);
	if (compare_res == 0) {
		// Βρέθηκε ισοδύναμη τιμή στον node, οπότε τον διαγράφουμε. Το πώς θα γίνει αυτό εξαρτάται από το αν έχει παιδιά.
		*removed = true;
		*old_value = node->value;

		if (node->left == NULL) {
			// Δεν υπάρχει αριστερό υποδέντρο, οπότε διαγράφεται απλά ο κόμβος και νέα ρίζα μπαίνει το δεξί παιδί
			SetNode right = node->right;	// αποθήκευση πριν το free!
			free(node);
			if(right != NULL) {
			node_update_height(right);
			node_update_size(right);
			}
			return right;

		} else if (node->right == NULL) {
			// Δεν υπάρχει δεξί υποδέντρο, οπότε διαγράφεται απλά ο κόμβος και νέα ρίζα μπαίνει το αριστερό παιδί
			SetNode left = node->left;		// αποθήκευση πριν το free!
			free(node);
			if(left != NULL) {
				node_update_height(left);
				node_update_size(left);
			}
			return left;

		} else {
			// Υπάρχουν και τα δύο παιδιά. Αντικαθιστούμε την τιμή του node με την μικρότερη του δεξιού υποδέντρου, η οποία
			// αφαιρείται. Η συνάρτηση node_remove_min κάνει ακριβώς αυτή τη δουλειά.

			SetNode min_right;
			node->right = node_remove_min(node->right, &min_right);

			// Σύνδεση του min_right στη θέση του node
			min_right->left = node->left;
			min_right->right = node->right;

			free(node);
			node_update_height(min_right);
			node_update_size(min_right);
			return min_right;
		}
	}

	// compare_res != 0, συνεχίζουμε στο αριστερό ή δεξί υποδέντρο, η ρίζα δεν αλλάζει.
	if (compare_res < 0)
		node->left  = node_remove(node->left,  compare, value, removed, old_value);
	else
		node->right = node_remove(node->right, compare, value, removed, old_value);

	return node;
}

// Καταστρέφει όλο το υποδέντρο με ρίζα node

static void node_destroy(SetNode node, DestroyFunc destroy_value) {
	if (node == NULL)
		return;
	
	// πρώτα destroy τα παιδιά, μετά free το node
	node_destroy(node->left, destroy_value);
	node_destroy(node->right, destroy_value);

	if (destroy_value != NULL)
		destroy_value(node->value);

	free(node);
}


//// Συναρτήσεις του ADT Set. Γενικά πολύ απλές, αφού καλούν τις αντίστοιχες node_*

Set set_create(CompareFunc compare, DestroyFunc destroy_value) {
	assert(compare != NULL);	// LCOV_EXCL_LINE

	// δημιουργούμε το stuct
	Set set = malloc(sizeof(*set));
	set->root = NULL;			// κενό δέντρο
	set->size = 0;
	set->compare = compare;
	set->destroy_value = destroy_value;

	return set;
}

int set_size(Set set) {
	return set->size;
}


bool set_remove(Set set, Pointer value) {
	bool removed;
	Pointer old_value = NULL;
	// SetNode node;
	set->root = node_remove(set->root, set->compare, value, &removed, &old_value);
	// node = check_route(set->root,value,set);
	// if(node != NULL) {
	// set->root = node_repair_balance(set,node);
	// }
	// Το size αλλάζει μόνο αν πραγματικά αφαιρεθεί ένας κόμβος
	if (removed) {
		set->size--;

		if (set->destroy_value != NULL)
			set->destroy_value(old_value);
	}

	return removed;
}

Pointer set_find(Set set, Pointer value) {
	SetNode node = node_find_equal(set->root, set->compare, value);
	return node == NULL ? NULL : node->value;
}

DestroyFunc set_set_destroy_value(Set vec, DestroyFunc destroy_value) {
	DestroyFunc old = vec->destroy_value;
	vec->destroy_value = destroy_value;
	return old;
}

void set_destroy(Set set) {
	node_destroy(set->root, set->destroy_value);
	free(set);
}

SetNode set_first(Set set) {
	return node_find_min(set->root);
}

SetNode set_last(Set set) {
	return node_find_max(set->root);
}

SetNode set_previous(Set set, SetNode node) {
	return node_find_previous(set->root, set->compare, node);
}

SetNode set_next(Set set, SetNode node) {
	return node_find_next(set->root, set->compare, node);
}

Pointer set_node_value(Set set, SetNode node) {
	return node->value;
}

SetNode set_find_node(Set set, Pointer value) {
	return node_find_equal(set->root, set->compare, value);
}


static void node_repair_balance(Set set, SetNode node) {
	Vector values = vector_create(0,NULL);
	Pointer key = set_node_value(set,node);
	SetNode parent = find_parent(set,set->root,key);
	int work = 1;
	while(work) {
		if(node->size == 1) {
			work = 0;
			break;
		}
		SetNode min;
		if(node->left == NULL) {
			node->right = node_remove_min(node,&min);
			Pointer node_vec = min;
			vector_insert_last(values,node_vec);
			node->size--;
			node->height--;
			set->size--;
		}
		else if(node->right == NULL) {
			node->left = node_remove_min(node,&min);
			Pointer node_vec = min;
			vector_insert_last(values,node_vec);
			node->size--;
			node->height--;
			set->size--;
		}
	}
	Set balanced_set = set_create_from_sorted_values(set->compare, NULL, values);
	if(parent == NULL || parent == set->root) {
		set->root = balanced_set->root;
		}
		else if(balanced_set->root->value >= parent->value) {
			parent->right = balanced_set->root;
		}
		else {
			parent->left = balanced_set->root;
		}
}
// SetNode min;
// int work = 1;
// while(work) {
// 	if(node->left != NULL) {
// 		node = node_remove_min(node,&min);
// 		Pointer node_vec = min;
// 		vector_insert_last(values,node_vec);
// 	}
// 	if(node->right != NULL && node = set->root) {
// 		node = node_remove_min(node->right,&min);
// 		Pointer node_vec = min;
// 		vector_insert_last(values,node_vec);
// 	}
// }







// 	Vector values = vector_create(0,NULL);
// 	SetNode min;
// 	SetNode parent;
// 	Pointer value;
// 	int work = 1;
// 	int first = 1;
// 	while(work) {
// 		if(node->size == 1) {
// 			work = 0;
// 			value = set_node_value(set,node);
// 			parent = find_parent(set->root,value);
// 			Pointer node_for_vec = node_remove_min(node,&min);
// 			vector_insert_last(values,node_for_vec);
// 			break;
// 		}
// 		if(node->left == NULL && first == 1 && node == set->root) {
// 			min = node_find_min(node->right);
// 			Pointer node_for_vec = node;
// 			vector_insert_last(values,node_for_vec);
// 			printf("value : %d\n", *(int*)min->value);
// 			first = 0;
// 		}
// 		else if(node->left == NULL && node == set->root) {
// 			node->right = node_remove_min(node->right, &min);
// 			node->height--;
// 			node->size--;
// 			Pointer node_for_vec = min;
// 			vector_insert_last(values,node_for_vec);
// 			printf("value : %d\n", *(int*)min->value);
// 		}
// 		else{
// 			node = node_remove_min(node,&min);
// 			Pointer node_for_vec = min;
// 			vector_insert_last(values, node_for_vec);
// 		}
// 	}
// 	Set balanced_set = set_create_from_sorted_values(set->compare,NULL,values);
	
// 	if(parent == NULL) {
// 		set->root = balanced_set->root;
// 	}
// 	else if(balanced_set->root->value >= parent->value) {
// 		parent->right = balanced_set->root;
// 	}
// 	else {
// 		parent->left = balanced_set->root;
// 	}
// 	return set->root;
// }

void set_insert(Set set, Pointer value) {
	bool inserted;
	Pointer old_value;
	SetNode node;
	set->root = node_insert(set,set->root, set->compare, value, &inserted, &old_value);
	// Το size αλλάζει μόνο αν μπει νέος κόμβος. Στα updates κάνουμε destroy την παλιά τιμή
	if (inserted) {
		set->size++;
	}
	else if (set->destroy_value != NULL) {
		set->destroy_value(old_value);
	}
	node = check_route(set->root,value,set);
	if(node != NULL) {
	node_repair_balance(set,node);
	}
}





// Συναρτήσεις που δεν υπάρχουν στο public interface αλλά χρησιμοποιούνται στα tests.
// Ελέγχουν ότι το δέντρο είναι ένα σωστό BST.

// LCOV_EXCL_START (δε μας ενδιαφέρει το coverage των test εντολών, και επιπλέον μόνο τα true branches εκτελούνται σε ένα επιτυχημένο test)

static bool node_is_bst(SetNode node, CompareFunc compare) {
	if (node == NULL)
		return true;

	// Ελέγχουμε την ιδιότητα:
	// κάθε κόμβος είναι > αριστερό παιδί, > δεξιότερο κόμβο του αριστερού υποδέντρου, < δεξί παιδί, < αριστερότερο κόμβο του δεξιού υποδέντρου.
	// Είναι ισοδύναμη με την BST ιδιότητα (κάθε κόμβος είναι > αριστερό υποδέντρο και < δεξί υποδέντρο) αλλά ευκολότερο να ελεγθεί.
	bool res = true;
	if(node->left != NULL)
		res = res && compare(node->left->value, node->value) < 0 && compare(node_find_max(node->left)->value, node->value) < 0;
	if(node->right != NULL)
		res = res && compare(node->right->value, node->value) > 0 && compare(node_find_min(node->right)->value, node->value) > 0;

	return res &&
		node_is_bst(node->left, compare) &&
		node_is_bst(node->right, compare);
}

bool set_is_proper(Set node) {
	return node_is_bst(node->root, node->compare);
}

// LCOV_EXCL_STOP