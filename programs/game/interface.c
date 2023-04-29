#include "raylib.h"

#include "state.h"
#include "interface.h"

//Assets
Texture ball_img;
Texture star_img;


void interface_init() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
    SetTargetFPS(60);

    //Φόρτωση εικόνων
    ball_img = LoadTextureFromImage(LoadImage("assets/ball-removebg-preview.png"));
    star_img = LoadTextureFromImage(LoadImage("assets/star-removebg-preview.png"));
    
}

void interface_close() {
    CloseWindow();
}

// Draw game (one frame)
void interface_draw_frame(State state){
    BeginDrawing();

    // Καθαρισμός, θα τα σχεδιάσουμε όλα από την αρχή
    ClearBackground(RAYWHITE);
    
    StateInfo info = state_info(state);

    // κρατάμε σε μια μεταβλητή το offset για να ακολουθεί την μπάλα η οθόνη.
    float ball_x = info->ball->rect.x;
    float x_offset = SCREEN_WIDTH - 700 - ball_x;

    // Σχεδιάζουμε την μπάλα
    DrawTexture(ball_img, ball_x + x_offset, info->ball->rect.y, WHITE);
    
    // Καλούμε την συνάρτηση state_objects για να φτιάξουμε την λίστα με τα αντικείμενα.
    List objs = state_objects(state, ball_x - SCREEN_WIDTH, ball_x + SCREEN_WIDTH);

    // Διατρέχουμε την λίστα για να σχεδιάσουμε τα αντικείμενα.
    for (ListNode node = list_first(objs);
         node != LIST_EOF;
         node = list_next(objs, node)) {
                Object obj = list_node_value(objs, node);

                // Σχεδιάζουμε τις stable πλατφόρμες.
                 if (obj->type == PLATFORM && !obj->unstable)
                 DrawRectangle(obj->rect.x + x_offset, obj->rect.y, obj->rect.width, obj->rect.height, BLUE);

                 // Σχεδιάζουμε τα αστέρια.
                 else if (obj->type == STAR) 
                 DrawTexture(star_img, obj->rect.x + x_offset, obj->rect.y, WHITE);

                 // Σχεδιάζουμε τις unstable πλατφόρμες.
                 else if (obj->unstable) 
                 DrawRectangle(obj->rect.x + x_offset, obj->rect.y, obj->rect.width, obj->rect.height,  RED);

         }
    // Σχεδιάζουμε το σκορ και το FPS counter
	DrawText(TextFormat("%04i", info->score), 20, 20, 40, RED);
	DrawFPS(SCREEN_WIDTH - 80, 0);

	// Αν το παιχνίδι έχει τελειώσει, σχεδιάζουμε το μήνυμα για να ξαναρχίσει
	if (!info->playing && !info->paused) {
		DrawText(
			" GAME OVER | PRESS [ENTER] TO PLAY AGAIN",
			 GetScreenWidth() / 2 - MeasureText("GAME OVER | PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
			 GetScreenHeight() / 2 - 50, 20, GRAY
		);
	}

    // Αν το παιχνίδι είναι σε pause mode, σχεδιάζουμε μήνυμα για να συνεχίσει.
    if (info->paused) {
        DrawText(
			" PAUSED | PRESS [ENTER] TO CONTINUE",
			 GetScreenWidth() / 2 - MeasureText(" PAUSED | PRESS [ENTER] TO CONTINUE", 20) / 2,
			 GetScreenHeight() / 2 - 50, 20, GRAY
		);
    }
    EndDrawing();
}