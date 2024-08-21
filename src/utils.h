#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include <raylib/raylib.h>
#include <defines.h>

class Utils {
public:

    static void extractDigits(int number, std::vector<int>& result, std::size_t minSize, int paddingValue) {
        std::string numStr = std::to_string(number);

        for (char c : numStr) {
            result.push_back(c - '0');
        }

        std::size_t zerosToAdd = minSize > result.size() ? minSize - result.size() : 0;

        result.insert(result.begin(), zerosToAdd, paddingValue);
    }

    static void renderNumberAtPosition(Texture2D &textureAtlas, int number, std::size_t minSize, float x, float y, bool fillWithZeros, Color tint) {
        std::vector<int> digits;
        extractDigits(number, digits, minSize, fillWithZeros ? 0 : -1);

        for (std::size_t i = 0; i < digits.size(); ++i) {
            float digitXPos = ZERO_NUMBER_X + (digits[i] * NUMBER_SPRITE_WIDTH);
            Rectangle source = (digits[i] != -1) ? (Rectangle){digitXPos,384,NUMBER_SPRITE_WIDTH,16} : (Rectangle){0,0,0,0};
            DrawTextureRec(textureAtlas, source, {x + (i * NUMBER_SPRITE_WIDTH), y}, tint);
        }
    }

    static void getMountainFilename(int mountainNumber, std::string& filename) {
        std::vector<int> mountainNumberDigits;
        extractDigits(mountainNumber, mountainNumberDigits, 2, 0);

        filename.clear();
        filename += MOUNTAIN_FILENAME;

        std::string numberStr;
        for (int number : mountainNumberDigits) {
            numberStr += std::to_string(number);
        }

        std::size_t pos = filename.find('?');
        if (pos == std::string::npos) {
            assert((pos == std::string::npos) && "Error: Character '?' not found in mountain filename template.");
        }

        filename.replace(pos, 1, numberStr);
    }

    static int loadHighscoreFromFile(const std::string& filename) {
        std::ifstream file(filename);

        if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof()) {
            return 0;
        }

        std::string content;
        std::getline(file, content);
        file.close();

        int highscore = 0;
        std::stringstream ss(content);
        ss >> highscore;

        return highscore;
    }

    static void saveHighscoreToFile(int score, const std::string& filename) {
        std::ofstream outFile(filename);

        if (outFile.is_open()) {
            outFile << score;
            outFile.close();
        } else {
            std::cerr << "Error: Cannot open the file " << filename << std::endl;
        }
    }
};

#endif // UTILS_H
