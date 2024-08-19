#include <vector>
#include <string>
#include <chrono>
#include <raylib/raylib.h>
#include <defines.h>
#include <entity_manager.h>

void extractDigits(int number, std::vector<int>& result, std::size_t minSize, int paddingValue) {
    std::string numStr = std::to_string(number);

    for (char c : numStr) {
        result.push_back(c - '0');
    }

    std::size_t zerosToAdd = minSize > result.size() ? minSize - result.size() : 0;

    result.insert(result.begin(), zerosToAdd, paddingValue);
}

void renderNumberAtPosition(Texture2D &textureAtlas, int number, std::size_t minSize, float x, float y, bool fillWithZeros) {
    std::vector<int> digits;
    extractDigits(number, digits, minSize, fillWithZeros ? 0 : -1);

    for (std::size_t i = 0; i < digits.size(); ++i) {
        float digitXPos = ZERO_NUMBER_X + (digits[i] * NUMBER_SPRITE_WIDTH);
        Rectangle source = (digits[i] != -1) ? (Rectangle){digitXPos,384,NUMBER_SPRITE_WIDTH,16} : (Rectangle){0,0,0,0};
        DrawTextureRec(textureAtlas, source, {x + (i*NUMBER_SPRITE_WIDTH),y}, WHITE);
    }
}

void renderScoreScreen(Texture2D &textureAtlas, Camera2D &staticCamera, GameScoreSummary &scoreSummary, int mountainNumber, int accumulatedScore) {
    // REVISIT: scoreSummary modified here just for debug purposes.
    scoreSummary.condorHunted = true;
    scoreSummary.condorUnitScore = 5000;
    scoreSummary.vegetableUnitScore = 500;
    scoreSummary.vegetableCount = 3;
    scoreSummary.nitpickerUnitScore = 800;
    scoreSummary.nitpickerCount = 2;
    scoreSummary.iceUnitScore = 400;
    scoreSummary.iceCount = 8;
    scoreSummary.brickUnitScore = 40;
    scoreSummary.brickCount = 19;

    BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(staticCamera);
                    DrawTextureRec(textureAtlas, {304,336,128,16}, {168,32}, WHITE);  // MOUNTAIN text
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

                    DrawTextureRec(textureAtlas, {272,176+(scoreSummary.condorHunted ? 32.0f : 0.0f),96,32}, {234,138}, WHITE);  // Winner/No bonus message text

                    if (scoreSummary.condorHunted) {
                        renderNumberAtPosition(textureAtlas, scoreSummary.condorUnitScore, 4, 250, 170, false);  // Condor score
                    }

                    float vegetableXPos = EGGPLANT_VEGETABLE_X + ((scoreSummary.vegetableId - EntityIdentificator::EGGPLANT) * VEGETABLE_SPRITE_WIDTH);
                    DrawTextureRec(textureAtlas, {vegetableXPos,352,VEGETABLE_SPRITE_WIDTH,32}, {178,200}, WHITE);  // Vegetable
                    DrawTextureRec(textureAtlas, {256,112,16,32}, {184,228}, WHITE);  // Ice
                    DrawTextureRec(textureAtlas, {272,144,32,32}, {178,267}, WHITE);  // Nitpicker
                    DrawTextureRec(textureAtlas, {32,232,16,16}, {184,310}, WHITE);  // Blue brick
                    renderNumberAtPosition(textureAtlas, mountainNumber, 2, 305, 32, true);  // Mountain number

                    renderNumberAtPosition(textureAtlas, scoreSummary.vegetableUnitScore, 4, 218, 214, false);  // Vegetable score counter
                    renderNumberAtPosition(textureAtlas, scoreSummary.vegetableCount, 2, 300, 214, true);  // Vegetable counter

                    renderNumberAtPosition(textureAtlas, scoreSummary.iceUnitScore, 4, 218, 246, false);  // Ice score counter
                    renderNumberAtPosition(textureAtlas, scoreSummary.iceCount, 2, 300, 246, true);  // Ice hit counter

                    renderNumberAtPosition(textureAtlas, scoreSummary.nitpickerUnitScore, 4, 218, 278, false);  // Nitpicker score counter
                    renderNumberAtPosition(textureAtlas, scoreSummary.nitpickerCount, 2, 300, 278, true);  // Nitpicker hit counter

                    renderNumberAtPosition(textureAtlas, scoreSummary.brickUnitScore, 4, 218, 310, false);  // Brick score counter
                    renderNumberAtPosition(textureAtlas, scoreSummary.brickCount, 2, 300, 310, true);  // Brick hit counter

                    renderNumberAtPosition(textureAtlas, accumulatedScore, 6, 202, 390, true);  // Total score
            EndMode2D();
            DrawFPS(16, 16);
    EndDrawing();
}