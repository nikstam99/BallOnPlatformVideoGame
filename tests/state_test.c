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
	 
	List result1 = state_objects(state, 0, SCREEN_WIDTH);
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

	// Με πατημένο το αριστερό βέλος, η μπάλα μετακινείται 1 pixel δεξιά
	keys.left = true;
	keys.right = false;
	old_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;
	
	TEST_ASSERT( new_rect.x == old_rect.x + 1);

	// Με πατημένο το πάνω βέλος σε κατάσταση IDLE, η μπάλα μπαίνει σε κατάσταση JUMPING
	keys.up = true;
	keys.left = false;
	state_update(state, &keys);
	VerticalMovement new_mov = state_info(state)->ball->vert_mov;

	TEST_ASSERT( new_mov == JUMPING );

	// Αν η μπάλα βρίσκεται σε κατάσταση JUMPING, μετακινείται προς τα πάνω όσο η κατακόρυφη ταχύτητητα της
	old_rect = state_info(state)->ball->rect;
	state_info(state)->ball->vert_mov = JUMPING;
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;

	TEST_ASSERT( new_rect.y == old_rect.y + state_info(state)->ball->vert_speed);

	// Αν η μπάλα βρίσκεται σε κατάσταση FALLING, μετακινείται προς τα κάτω όσο η κατακόρυφη ταχύτητητα της,
	// εκτός αν συγκρουστεί με πλατφόρμα
	keys.up = false;
	state_info(state)->ball->vert_mov = FALLING;
	float speed = state_info(state)->ball->vert_speed;
	old_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;

	Rectangle obj_rect;
	Object obj;
	List list = state_objects(state, 0, SCREEN_WIDTH);
	bool FLAG = false;
	for(ListNode node = list_first(list);         
    node != LIST_EOF;                          
    node = list_next(list, node)) {
		obj = list_node_value(list, node);
		obj_rect = obj->rect;
		if (CheckCollisionRecs(state_info(state)->ball->rect, obj_rect)) 
			FLAG = true;
	}
	
	if (FLAG)
		TEST_ASSERT(new_rect.y == obj_rect.y);
	else 
		TEST_ASSERT(new_rect.y == old_rect.y - speed);

	// Αν η μπάλα συγκρουστεί με αστέρι, τότε το score αυξάνεται κατά 10
	int old_score = state_info(state)->score;
	state_update(state, &keys);
	int new_score = state_info(state)->score;

    list = state_objects(state, 0, SCREEN_WIDTH);
	bool FLAG2 = false;
	for(ListNode node = list_first(list);         
    node != LIST_EOF;                          
    node = list_next(list, node)) {
		obj = list_node_value(list, node);
		obj_rect = obj->rect;
		if (CheckCollisionRecs(state_info(state)->ball->rect, obj_rect)) 
			FLAG2 = true;
	}

	if (obj->type == STAR && FLAG2) {
		TEST_ASSERT(new_score == old_score + 10);
	}

	// Αν η μπάλα βρίσκεται πάνω σε πλατφόρμα, τότε ακολουθεί το ύψος της πλατφόρμας
	StateInfo info = state_info(state);
	state_update(state, &keys);
	list = state_objects(state, 0, SCREEN_WIDTH);
	bool FLAG3 = false;
	for(ListNode node = list_first(list);         
    node != LIST_EOF;                          
    node = list_next(list, node)) {
		obj = list_node_value(list, node);
		obj_rect = obj->rect;
		if (info->ball->rect.x >= obj_rect.x 
			&& info->ball->rect.x <= obj->rect.width + obj->rect.x
			&& info->ball->rect.y == obj_rect.y) 
			FLAG3 = true;
	}
	if (obj->type == PLATFORM && FLAG3) {
		TEST_ASSERT(state_info(state)->ball->vert_mov == IDLE);
	}
		TEST_ASSERT(state_info(state)->ball->rect.y == obj_rect.y);




	// Προσθέστε επιπλέον ελέγχους
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};