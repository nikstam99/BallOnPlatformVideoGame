#include "acutest.h"
#include "set_utils.h"

int* create_int(int value) {
	int* pointer = malloc(sizeof(int));	
	*pointer = value;						
	return pointer;
}

int compare_ints(Pointer a, Pointer b){
    return *(int*)a - *(int*)b;
}

void void_test_set_func() {
    
    Set set = set_create(compare_ints, free);
    for (int i = 0; i < 10; i++) {
        set_insert(set, create_int(i));
    }
    Pointer value = create_int(5);
    Pointer GREATER = set_find_eq_or_greater(set, value);
    Pointer SMALLER = set_find_eq_or_smaller(set, value);

    TEST_ASSERT(compare_ints(GREATER, value) && compare_ints(value, SMALLER));
}


TEST_LIST = {
    {"test_set_find_eq_or_greater", set_find_eq_or_greater},
    {"test_set_find_eq_or_smaller", set_find_eq_or_smaller},

    {NULL, NULL}
};