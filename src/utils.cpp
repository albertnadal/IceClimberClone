#ifndef UTILS_CPP
#define UTILS_CPP

#include <vector>
#include <string>
#include <raylib/raylib.h>
#include <defines.h>

void extractDigits(int number, std::vector<int>& result, std::size_t minSize, int paddingValue) {
    std::string numStr = std::to_string(number);

    for (char c : numStr) {
        result.push_back(c - '0');
    }

    std::size_t zerosToAdd = minSize > result.size() ? minSize - result.size() : 0;

    result.insert(result.begin(), zerosToAdd, paddingValue);
}

void renderNumberAtPosition(Texture2D &textureAtlas, int number, std::size_t minSize, float x, float y, bool fillWithZeros, Color tint) {
    std::vector<int> digits;
    extractDigits(number, digits, minSize, fillWithZeros ? 0 : -1);

    for (std::size_t i = 0; i < digits.size(); ++i) {
        float digitXPos = ZERO_NUMBER_X + (digits[i] * NUMBER_SPRITE_WIDTH);
        Rectangle source = (digits[i] != -1) ? (Rectangle){digitXPos,384,NUMBER_SPRITE_WIDTH,16} : (Rectangle){0,0,0,0};
        DrawTextureRec(textureAtlas, source, {x + (i*NUMBER_SPRITE_WIDTH),y}, tint);
    }
}

#endif