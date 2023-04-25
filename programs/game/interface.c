#include "raylib.h"

#include "state.h"
#include "interface.h"

//Assets
Texture ball_img;
Texture platform_img;
Texture star_img;

void interface_init() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
    SetTargetFPS(60);

    //Φόρτωση εικόνων
    ball_img = LoadTextureFromImage(LoadImage("assets/ball-removebg-preview.png"));
    platform_img = LoadTextureFromImage(LoadImage("assets/platform (1).png"));
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

    // Σχεδιάζουμε την μπάλα
    StateInfo info = state_info(state);
    float ball_x = info->ball->rect.x;
    float x_offset = SCREEN_WIDTH - 700 - ball_x;

    DrawTexture(ball_img, ball_x + x_offset, info->ball->rect.y, WHITE);
    

    List objs = state_objects(state, ball_x - SCREEN_WIDTH, ball_x + SCREEN_WIDTH);

    for (ListNode node = list_first(objs);
         node != LIST_EOF;
         node = list_next(objs, node)) {
                Object obj = list_node_value(objs, node);
                 if (obj->type == PLATFORM)
                 DrawTexture(platform_img, obj->rect.x + x_offset, obj->rect.y, WHITE);
                 else if (obj->type == STAR) 
                 DrawTexture(star_img, obj->rect.x + x_offset, obj->rect.y, WHITE);

         }

    // Σχεδιάζουμε το σκορ και το FPS counter
	DrawText(TextFormat("%04i", info->score), 20, 20, 40, RED);
	DrawFPS(SCREEN_WIDTH - 80, 0);

	// Αν το παιχνίδι έχει τελειώσει, σχεδιάζουμε το μήνυμα για να ξαναρχίσει
	if (!info->playing) {
		DrawText(
			" GAME OVER | PRESS [ENTER] TO PLAY AGAIN",
			 GetScreenWidth() / 2 - MeasureText("GAME OVER | PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
			 GetScreenHeight() / 2 - 50, 20, GRAY
		);
	}
    if (info->paused) {
        DrawText(
			" PAUSED ",
			 GetScreenWidth() / 2 - MeasureText("PAUSED", 20) / 2,
			 GetScreenHeight() / 2 - 50, 20, GRAY
		);
    }
    EndDrawing();
}