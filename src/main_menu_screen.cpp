#ifndef MAIN_MENU_SCREEN_CPP
#define MAIN_MENU_SCREEN_CPP

#include <raylib/raylib.h>
#include <defines.h>
#include <utils.h>

void renderMainMenuScreen(Texture2D &textureAtlas, Camera2D &staticCamera, int mountainNumber, int highScore) {
    BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(staticCamera);
                    DrawTextureRec(textureAtlas, {0,416,448,176}, {32,32}, WHITE);  // Ice Climber title
                    DrawTextureRec(textureAtlas, {96,264,16,16}, {118,236}, WHITE);  // Hammer
                    Utils::renderNumberAtPosition(textureAtlas, 1, 1, 162, 236, false, WHITE);  // 1 text
                    DrawTextureRec(textureAtlas, {432,352,96,16}, {194,236}, WHITE);  // PLAYER
                    DrawTextureRec(textureAtlas, {432,368,64,16}, {306,236}, WHITE);  // GAME
                    DrawTextureRec(textureAtlas, {304,336,128,16}, {162,268}, { 168, 228, 252, 255 });  // MOUNTAIN (tint #a8e4fc)
                    Utils::renderNumberAtPosition(textureAtlas, mountainNumber, 2, 306, 268, true, { 168, 228, 252, 255 });  // Mountain number
                    DrawTextureRec(textureAtlas, {432,336,48,16}, {162,300}, { 252, 116, 180, 255 });  // TOP (tint #fc74b4)
                    Utils::renderNumberAtPosition(textureAtlas, highScore, 6, 210, 300, true, { 252, 116, 180, 255 });  // High score number
                    DrawTextureRec(textureAtlas, {480,336,48,16}, {312,300}, { 252, 116, 180, 255 });  // PTS (tint #fc74b4)
                    DrawTextureRec(textureAtlas, {0,376,80,16}, {146,430}, WHITE);  // (c)1984
                    DrawTextureRec(textureAtlas, {272,240,128,16}, {242,430}, WHITE);  // NINTENDO
            EndMode2D();

    if (DEBUG) {
        DrawFPS(16, 16);
    }
    EndDrawing();
}

#endif