#include <stdio.h>
#include "raylib.h"

#include "interface.h"
#include "state.h"

State state;

void update_and_draw() {

	struct key_state keys;
	keys.left = IsKeyDown(KEY_LEFT);
	keys.right = IsKeyDown(KEY_RIGHT);
	keys.up = IsKeyDown(KEY_UP);
	keys.down = IsKeyDown(KEY_DOWN);
	keys.p = IsKeyDown(KEY_P);
	keys.enter = IsKeyDown(KEY_ENTER);
	keys.n = IsKeyDown(KEY_N);
	if (!state_info(state)->playing && keys.enter && !state_info(state)->paused) {
		state = state_create();
	}
	else if (state_info(state)->paused && keys.enter) {
		state_info(state)->playing = false;
		state_info(state)->paused = true;
	}
	else if (state_info(state)->paused && keys.n) {
		state_info(state)->playing = true;
		state_info(state)->paused = false;
		state_update(state, &keys);
		state_info(state)->playing = false;
		state_info(state)->paused = true;
	}
	state_update(state, &keys);
	interface_draw_frame(state);
}

int main() {
	state = state_create();
	interface_init();

	// Η κλήση αυτή καλεί συνεχόμενα την update_and_draw μέχρι ο χρήστης να κλείσει το παράθυρο
	start_main_loop(update_and_draw);

	interface_close();

	return 0;
}



