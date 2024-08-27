#ifndef SCORE_SCREEN_CPP
#define SCORE_SCREEN_CPP

#include <vector>
#include <string>
#include <chrono>
#include <raylib/raylib.h>
#include <defines.h>
#include <entity_manager.h>
#include <utils.h>

static inline void renderScoreScreen(Texture2D &textureAtlas, Camera2D &staticCamera, GameScoreSummary &scoreSummary, int mountainNumber, int accumulatedScore) {
    BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(staticCamera);
                    DrawTextureRec(textureAtlas, {304,336,128,16}, {168,32}, WHITE);  // MOUNTAIN
                    DrawTextureRec(textureAtlas, {560,0,220,380}, {140,72}, WHITE);   // Decorative frame

                    // Tricky animation of Popo (crying or jumping) at 1 frame per second.
                    float popoXOffset = 0.0f;
                    auto now = std::chrono::system_clock::now();
                    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
                    std::tm* localTime = std::localtime(&currentTime);    
                    int second = localTime->tm_sec;    
                    if (localTime->tm_sec % 2 == 0) {
                        popoXOffset += 48.0f;
                    }
                    DrawTextureRec(textureAtlas, {(scoreSummary.condorHunted ? 656.0f : 560.0f) + popoXOffset,400,48,64}, {174,120}, WHITE);  // Popo crying or jumping
                    DrawTextureRec(textureAtlas, {272,176+(scoreSummary.condorHunted ? 32.0f : 0.0f),96,32}, {234,138}, WHITE);  // WINNER/NO BONUS

                    if (scoreSummary.condorHunted) {
                        Utils::renderNumberAtPosition(textureAtlas, scoreSummary.condorUnitScore, 4, 250, 170, false, WHITE);  // Condor score
                    }

                    float vegetableXPos = EGGPLANT_VEGETABLE_X + ((scoreSummary.vegetableId - EntityIdentificator::EGGPLANT) * VEGETABLE_SPRITE_WIDTH);
                    DrawTextureRec(textureAtlas, {vegetableXPos,352,VEGETABLE_SPRITE_WIDTH,32}, {178,200}, WHITE);  // Vegetable
                    DrawTextureRec(textureAtlas, {256,112,16,32}, {184,228}, WHITE);  // Ice
                    DrawTextureRec(textureAtlas, {272,144,32,32}, {178,267}, WHITE);  // Nitpicker
                    DrawTextureRec(textureAtlas, {32,232,16,16}, {184,310}, WHITE);  // Blue brick
                    Utils::renderNumberAtPosition(textureAtlas, mountainNumber, 2, 305, 32, true, WHITE);  // Mountain number

                    Utils::renderNumberAtPosition(textureAtlas, scoreSummary.vegetableUnitScore, 4, 218, 214, false, WHITE);  // Vegetable score counter
                    Utils::renderNumberAtPosition(textureAtlas, scoreSummary.vegetableCount, 2, 300, 214, true, WHITE);  // Vegetable counter

                    Utils::renderNumberAtPosition(textureAtlas, scoreSummary.iceUnitScore, 4, 218, 246, false, WHITE);  // Ice score counter
                    Utils::renderNumberAtPosition(textureAtlas, scoreSummary.iceCount, 2, 300, 246, true, WHITE);  // Ice hit counter

                    Utils::renderNumberAtPosition(textureAtlas, scoreSummary.nitpickerUnitScore, 4, 218, 278, false, WHITE);  // Nitpicker score counter
                    Utils::renderNumberAtPosition(textureAtlas, scoreSummary.nitpickerCount, 2, 300, 278, true, WHITE);  // Nitpicker hit counter

                    Utils::renderNumberAtPosition(textureAtlas, scoreSummary.brickUnitScore, 4, 218, 310, false, WHITE);  // Brick score counter
                    Utils::renderNumberAtPosition(textureAtlas, scoreSummary.brickCount, 2, 300, 310, true, WHITE);  // Brick hit counter

                    Utils::renderNumberAtPosition(textureAtlas, accumulatedScore, 6, 202, 390, true, WHITE);  // Total score
            EndMode2D();

    if (DEBUG) {
        DrawFPS(16, 16);
    }
    EndDrawing();
}

#endif