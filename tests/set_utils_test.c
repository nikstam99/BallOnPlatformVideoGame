#include "acutest.h"
#include "set_utils.h"

// Συνάρτηση που δέχεται ακέραιο και επιστρέφει δείκτη σε αυτόν.
int* create_int(int value) {
	int* pointer = malloc(sizeof(int));	
	*pointer = value;						
	return pointer;
}

// Συνάρτηση που συγκρίνει 2 pointers.
int compare_ints(Pointer a, Pointer b){
    return *(int*)a - *(int*)b;
}

// Έλεγχος για την λειτουργηκότητα των συναρτήσεων του set_utils.c
void void_test_set_func() {
    Pointer val;

	// Δημιουργούμε ένα set και του προσθέτουμε 10 αριθμούς.
    Set set = set_create(compare_ints, free);
    for (int i = 0; i < 10; i++) {
		if (i == 5) continue;
		val = create_int(i);
        set_insert(set, val);
    }
	// Ψάχνουμε το 5 με τις συναρτήσεις στο set.
    Pointer value = create_int(5);
	Pointer SMALLER = set_find_eq_or_smaller(set, value);
	value = create_int(5);
    Pointer GREATER = set_find_eq_or_greater(set, value);

	// Αφού το 5 δεν υπάρχει ελέγχουμε ότι επιστρέφει σωστά το επόμενο/προηγούμενο στοιχείο.
    TEST_ASSERT(*(int*)GREATER == 6);
	TEST_ASSERT(*(int*)SMALLER == 4);

	// Προσθέτουμε το 5 στο set και ελέγχουμε ότι το επιστρέφουν οι συναρτήσεις.
	set_insert(set, create_int(5));
	value = create_int(5);
	SMALLER = set_find_eq_or_smaller(set, value);
	value = create_int(5);
    GREATER = set_find_eq_or_greater(set, value);

	TEST_ASSERT(*(int*)GREATER == *(int*)SMALLER);

	for (int i = 0; i < 10; i++) {
		set_remove(set, set_first(set));
	}
	free(value);
	free(val);
}

TEST_LIST = {
    {"test_set_find_eq_or_greater", void_test_set_func},

    {NULL, NULL}
};