#include "state.h"
#include "raylib.h"
#include "interface.h"

//Assets
Texture ball_img;

void interface_init() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
    SetTargetFPS(60);

    //Φόρτωση εικόνων
    ball_img = LoadTextureFromImage(LoadImage("assets/ball.png"));
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

    DrawTexture(ball_img, SCREEN_WIDTH - 700, info->ball->rect.y, WHITE);
    EndDrawing();
}