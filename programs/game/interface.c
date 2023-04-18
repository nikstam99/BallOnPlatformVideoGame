#include "state.h"
#include "interface.h"
#include "raylib.h"
#include <stdlib.h>

void interface_init() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game.c");
    SetTargetFPS(60);
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

}