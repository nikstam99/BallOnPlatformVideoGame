//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing
#include "ADTVector.h"
#include "state.h"

void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	StateInfo info = state_info(state);
	TEST_ASSERT(info != NULL);

	TEST_ASSERT(info->playing);
	TEST_ASSERT(!info->paused);
	TEST_ASSERT(info->score == 0);
	TEST_ASSERT(info->ball != NULL);
	TEST_ASSERT(info->ball->rect.x >= 0);
	TEST_ASSERT(info->ball->rect.x <= 10000);
	 
	List result1 = state_objects(state, 0, 10000);
	TEST_ASSERT(list_size(result1) != 0);

	List result2 = state_objects(state, 0, 100);
	TEST_ASSERT(list_size(result2) == 0);

	List result3 = state_objects(state, -100000, 100);
	TEST_ASSERT(list_size(result3) == 0);

	

	// Προσθέστε επιπλέον ελέγχους
}

void test_state_update() {
	State state = state_create();
	TEST_ASSERT(state != NULL && state_info(state) != NULL);

	// Πληροφορίες για τα πλήκτρα (αρχικά κανένα δεν είναι πατημένο)
	struct key_state keys = { false, false, false, false, false, false, false };
	
	// Χωρίς κανένα πλήκτρο, η μπάλα μετακινείται 4 pixels δεξιά
	Rectangle old_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	Rectangle new_rect = state_info(state)->ball->rect;

	TEST_ASSERT( new_rect.x == old_rect.x + 4 );

	// Με πατημένο το δεξί βέλος, η μπάλα μετακινείται 6 pixels δεξιά
	keys.right = true;
	old_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;

	TEST_CHECK( new_rect.x == old_rect.x + 6 );

	// Προσθέστε επιπλέον ελέγχους
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};