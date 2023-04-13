
#include <stdlib.h>

#include "ADTVector.h"
#include "ADTList.h"
#include "state.h"


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	Vector objects;			// περιέχει στοιχεία Object (Πλατφόρμες, Αστέρια)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	float speed_factor;		// Πολλαπλασιαστής ταχύτητς (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};


// Δημιουργεί και επιστρέφει ένα αντικείμενο

static Object create_object(ObjectType type, float x, float y, float width, float height, VerticalMovement vert_mov, float speed, bool unstable) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	obj->vert_mov = vert_mov;
	obj->vert_speed = speed;
	obj->unstable = unstable;
	return obj;
}

// Προσθέτει αντικείμενα στην πίστα (η οποία μπορεί να περιέχει ήδη αντικείμενα).
// Τα αντικείμενα ξεκινάνε από x = start_x, και επεκτείνονται προς τα δεξιά.
//
// ΠΡΟΣΟΧΗ: όλα τα αντικείμενα έχουν συντεταγμένες x,y σε ένα
// καρτεσιανό επίπεδο.
// - Στο άξονα x το 0 είναι η αρχή της πίστας και οι συντεταγμένες
//   μεγαλώνουν προς τα δεξιά.
//
// - Στον άξονα y το 0 είναι το πάνω μέρος της πίστας, και οι συντεταγμένες μεγαλώνουν
//   προς τα _κάτω_. Η πίστα έχει σταθερό ύψος, οπότε όλες οι
//   συντεταγμένες y είναι ανάμεσα στο 0 και το SCREEN_HEIGHT.
//
// Πέρα από τις συντεταγμένες, αποθηκεύουμε και τις διαστάσεις width,height
// κάθε αντικειμένου. Τα x,y,width,height ορίζουν ένα παραλληλόγραμμο, οπότε
// μπορούν να αποθηκευτούν όλα μαζί στο obj->rect τύπου Rectangle (ορίζεται
// στο include/raylib.h). Τα x,y αναφέρονται στην πάνω-αριστερά γωνία του Rectangle.

static void add_objects(State state, float start_x) {
	// Προσθέτουμε PLATFORM_NUM πλατφόρμες, με τυχαία χαρακτηριστικά.

	for (int i = 0; i < PLATFORM_NUM; i++) {
		Object platform = create_object(
			PLATFORM,
			start_x + 150 + rand() % 80,						// x με τυχαία απόσταση από το προηγούμενο στο διάστημα [150, 230]
			SCREEN_HEIGHT/4 + rand() % SCREEN_HEIGHT/2,			// y τυχαία στο διάστημα [SCREEN_HEIGHT/4, 3*SCREEN_HEIGHT/4]
			i == 0 ? 250 : 50 + rand()%200,						// πλάτος τυχαία στο διάστημα [50, 250] (η πρώτη πάντα 250)
			20,													// ύψος
			i < 3 || rand() % 2 == 0 ? MOVING_UP : MOVING_DOWN,	// τυχαία αρχική κίνηση (οι πρώτες 3 πάντα πάνω)
			0.6 + 3*(rand()%100)/100,							// ταχύτητα τυχαία στο διάστημα [0.6, 3.6]
			i > 0 && (rand() % 10) == 0							// το 10% (τυχαία) των πλατφορμών είναι ασταθείς (εκτός από την πρώτη)
		);
		vector_insert_last(state->objects, platform);

		// Στο 50% των πλατφορμών (τυχαία), εκτός της πρώτης, προσθέτουμε αστέρι
		if(i != 0 && rand() % 2 == 0) {
			Object star = create_object(
				STAR,
				start_x + 200 + rand() % 60,				// x με τυχαία απόσταση από την προηγούμενη πλατφόρμα στο διάστημα [200,260]
				SCREEN_HEIGHT/8 + rand() % SCREEN_HEIGHT/2,	// y τυχαία στο διάστημα [SCREEN_HEIGHT/8, 5*SCREEN_HEIGHT/8]
				30, 30,										// πλάτος, ύψος
				IDLE,										// χωρίς κίνηση
				0,										 	// ταχύτητα 0
				false										// 'unstable' πάντα false για τα αστέρια
			);
			vector_insert_last(state->objects, star);
		}

		start_x = platform->rect.x + platform->rect.width;	// μετακίνηση των επόμενων αντικειμένων προς τα δεξιά
	}
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));

	// Γενικές πληροφορίες
	state->info.playing = true;				// Το παιχνίδι ξεκινάει αμέσως
	state->info.paused = false;				// Χωρίς να είναι paused.
	state->speed_factor = 1;				// Κανονική ταχύτητα
	state->info.score = 0;				// Αρχικό σκορ 0

	// Δημιουργούμε το vector των αντικειμένων, και προσθέτουμε αντικείμενα
	// ξεκινώντας από start_x = 0.
	state->objects = vector_create(0, NULL);
	add_objects(state, 0);

	// Δημιουργούμε την μπάλα τοποθετώντας τη πάνω στην πρώτη πλατφόρμα
	Object first_platform = vector_get_at(state->objects, 0);
	state->info.ball = create_object(
		BALL,
		first_platform->rect.x,			// x στην αρχή της πλατφόρμας
		first_platform->rect.y - 40,	// y πάνω στην πλατφόρμα
		45, 45,							// πλάτος, ύψος
		IDLE,							// χωρίς αρχική κατακόρυφη κίνηση
		0,								// αρχική ταχύτητα 0
		false							// "unstable" πάντα false για την μπάλα
	);

	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state) {
	return &state->info;
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η συντεταγμένη x είναι ανάμεσα στο x_from και x_to.

List state_objects(State state, float x_from, float x_to) {
	List result = list_create(NULL);
	for (int i = 0; i < vector_size(state->objects); i++) {

		Object obj = vector_get_at(state->objects, i);
		if(obj->rect.x >= x_from && obj->rect.x <= x_to) 
			list_insert_next(result, LIST_BOF, obj);

	}

	return result;
}

// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys) {
	float SPEED = 1;
	if (state->info.playing) {
		if (keys->right)						// Αν είναι πατημένο το δεξί βελάκι,
			state->info.ball->rect.x += SPEED * 6;		// η μπάλα μετακινείται 6 pixels δεξιά.

		else if (keys->left)					// Αν είναι πατημένο το αριστερό βελάκι,
			state->info.ball->rect.x += SPEED * 1;		// η μπάλα μετακινείται 1 pixel δεξιά.

		else 									// Αν δεν είναι πατημένο κανένα βελάκι,
			state->info.ball->rect.x += SPEED * 4;		// η μπάλα μετακινείται 4 pixel δεξιά.

		if (state->info.ball->vert_mov == IDLE && keys->up) {
			state->info.ball->vert_mov = JUMPING;
			state->info.ball->vert_speed = SPEED * 17;
		}
		else if (state->info.ball->vert_mov == JUMPING) {
			state->info.ball->rect.y += SPEED * state->info.ball->vert_speed;
			state->info.ball->vert_speed -= 85/100 * state->info.ball->vert_speed;
			if (state->info.ball->vert_speed <= 0.5) 
				state->info.ball->vert_mov = FALLING;
		}
		else if (state->info.ball->vert_mov == FALLING) {
			state->info.ball->rect.y -= SPEED * state->info.ball->vert_speed;
			state->info.ball->vert_speed += 10/100 * state->info.ball->vert_speed;
			if (state->info.ball->vert_speed > 7) 
				state->info.ball->vert_speed = 7;
		}

		for (int i = 0; i < vector_size(state->objects); i++) {
			Object obj = vector_get_at(state->objects, i);
			if (obj->type == PLATFORM) {
				if (obj->vert_mov == MOVING_UP) {
					obj->rect.y += SPEED * obj->vert_speed;
					if (obj->rect.y > SCREEN_HEIGHT/4)
						obj->vert_mov = MOVING_DOWN;
				}
				else if (obj->vert_mov == MOVING_DOWN) {
					obj->rect.y -= SPEED * obj->vert_speed;
					if (obj->rect.y < 3*SCREEN_HEIGHT/4)
						obj->vert_mov = MOVING_UP;
				}
				else if (obj->vert_mov == FALLING) {
					obj->rect.y -= SPEED * 4;
				}
				if (state->info.ball->vert_mov == IDLE) {
					if (state->info.ball->rect.x >= obj->rect.x 
					    && state->info.ball->rect.x <= obj->rect.width + obj->rect.x 
					    && state->info.ball->rect.y == obj->rect.y) 
						state->info.ball->rect.y = obj->rect.y;

					else {
						state->info.ball->vert_mov = FALLING;
						state->info.ball->vert_speed = SPEED * 1.5;
					}
				}
			}
		}
		int i = 0;
		while (i < vector_size(state->objects)) {
			Object obj = vector_get_at(state->objects, i);
			if (CheckCollisionRecs(state->info.ball->rect, obj->rect)
				&& obj->type == STAR) {
					Object swap = vector_get_at(state->objects, vector_size(state->objects) - 1);
					vector_set_at(state->objects, i, swap);
					vector_remove_last(state->objects);
					state->info.score += 10;
				}
			if (obj->type == PLATFORM && obj->rect.y == -SCREEN_HEIGHT) {
					Object swap = vector_get_at(state->objects, vector_size(state->objects) - 1);
					vector_set_at(state->objects, i, swap);
					vector_remove_last(state->objects);
			}
			if (CheckCollisionRecs(state->info.ball->rect, obj->rect)
				&& state->info.ball->vert_mov == FALLING
				&& obj->type == PLATFORM) {
					state->info.ball->vert_mov = IDLE;
					state->info.ball->rect.y = obj->rect.y;
					}
			i++;
		}
		Object last_platform = vector_get_at(state->objects, 0);
		for (i = 1; i < vector_size(state->objects); i++) {
			Object plat = vector_get_at(state->objects, i);
			if (last_platform->rect.x < plat->rect.x) 
				last_platform = plat;
		}
		if (last_platform->rect.x - state->info.ball->rect.x <= SCREEN_WIDTH) {
			add_objects(state, last_platform->rect.x);
			state->speed_factor += 10/100 * state->speed_factor;
		}
		if (state->info.ball->rect.y == -SCREEN_HEIGHT) 
			state->info.playing = false;
		
	} 
	else if (!state->info.playing && keys->enter) 
		state_create();
	else if (state->info.playing && keys->p) {
		state->info.playing = false;
		state->info.paused = true;
	}
	else if (state->info.paused && keys->n)
		state_update(state, keys);
}

	


// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state) {
	// Προς υλοποίηση
}