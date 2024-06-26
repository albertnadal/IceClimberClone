#include "scene_object_data_manager.h"
#include <fstream>
#include <sstream>
#include <collision/collision.h>

using namespace collision;

SceneObjectDataManager::SceneObjectDataManager()
{
        cout << "SceneObjectDataManager created!" << endl;
        LoadObjectsDataFromFile(OBJECT_TYPES_FILENAME);
        Print();
}

SceneObjectDataManager::~SceneObjectDataManager() {
        for (auto& kv : objectSpriteSheetsMap) {
                ObjectSpriteSheet* objectSpriteSheet = kv.second;
                delete objectSpriteSheet;
        }

        objectSpriteSheetsMap.clear();
}

void SceneObjectDataManager::Print()
{
        printf("Texture filename: %s\n", textureFilename.c_str());
        printf("Total object sprite sheets: %lu\n", objectSpriteSheetsMap.size());
        for (auto& kv : objectSpriteSheetsMap) {
                ObjectSpriteSheet* objectSpriteSheet = kv.second;
                objectSpriteSheet->Print();
        }
}

void SceneObjectDataManager::LoadObjectsDataFromFile(std::string filename)
{
        enum LineType { OBJ_TEX_FILENAME, OBJ_ID, OBJ_ANIMATION_ID, OBJ_SPRITE, OBJ_SPRITE_COLLISION_AREA };

        std::ifstream infile(filename);
        std::string line;
        ObjectSpriteSheet *currentObjectSpriteSheet;
        ObjectSpriteSheetAnimation *currentObjectSpriteSheetAnimation;
        uint16_t currentObjectSpriteSheetAnimationId;
        SpriteAreas *currentAreas;

        while (std::getline(infile, line))
        {
                std::istringstream iss(line);
                string token;
                bool commentFound = false;
                std::vector<string> *currentFrameValues = new std::vector<string>;
                LineType currentLineType;

                while((iss >> token) && (!commentFound)) {
                        if(startsWith(token, "//")) {
                                commentFound = true;
                        } else if(startsWith(token, "###")) {
                                currentLineType = OBJ_TEX_FILENAME;
                                textureFilename = token.substr(3);
                        } else if(startsWith(token, "##")) {
                                currentLineType = OBJ_ID;
                                SceneObjectIdentificator objectId = (SceneObjectIdentificator)std::stoi(token.substr(2));
                                currentObjectSpriteSheet = new ObjectSpriteSheet(objectId);
                                objectSpriteSheetsMap[objectId] = currentObjectSpriteSheet;
                        } else if(startsWith(token, "#")) {
                                currentLineType = OBJ_ANIMATION_ID;
                                uint16_t objectSpriteSheetAnimationId = std::stoi(token.substr(1));
                                currentObjectSpriteSheetAnimation = new ObjectSpriteSheetAnimation(objectSpriteSheetAnimationId);
                                currentObjectSpriteSheet->AddAnimation(currentObjectSpriteSheetAnimation);
                        } else if(startsWith(token, "_")) {
                                currentLineType = OBJ_SPRITE_COLLISION_AREA;
                                uint16_t collisionAreaId = std::stoi(token.substr(1));
                                iss >> token;
                                string collisionAreaType = token; // Type is a string value

                                // Creates a temporal vector of tokens of the current line being processed
                                std::vector<float> *currentCollisionAreaValues = new std::vector<float>;

                                while(iss >> token) {
                                  currentCollisionAreaValues->push_back(stof(token));
                                }

                                // Load polygon points to a vector of points
                                std::vector<vec2<float>> points;
                                for(uint16_t i=0; i<currentCollisionAreaValues->size(); i+=2) {
                                  points.push_back(vec2<float>(currentCollisionAreaValues->at(i), currentCollisionAreaValues->at(i+1)));
                                }

                                delete currentCollisionAreaValues;
                                // Initializes a polygon from the array of points
                                collision::Rectangle rectangle(points);

                                // If the polygon corresponds to a solid area then add the polygon to the solidArea vector, otherwise add the polygon to simpleAreas
                                if(collisionAreaType=="solid") {
                                  currentAreas->solidAreas.push_back({ collisionAreaId, rectangle });
                                } else if(collisionAreaType=="simple") {
                                  currentAreas->simpleAreas.push_back({ collisionAreaId, rectangle });
                                }

                        } else {
                                currentLineType = OBJ_SPRITE;
                                currentFrameValues->push_back(token);
                        }
                }

                if(currentLineType == OBJ_SPRITE) {
                        if(currentFrameValues->size() == 13) {
                                uint16_t width = stoi(currentFrameValues->at(0));
                                uint16_t height = stoi(currentFrameValues->at(1));
                                int16_t xOffset = stoi(currentFrameValues->at(2));
                                int16_t yOffset = stoi(currentFrameValues->at(3));
                                float u1 = stof(currentFrameValues->at(4));
                                float v1 = stof(currentFrameValues->at(5));
                                float u2 = stof(currentFrameValues->at(6));
                                float v2 = stof(currentFrameValues->at(7));
                                uint16_t duration = stoi(currentFrameValues->at(8));
                                uint16_t lowerBoundX = stoi(currentFrameValues->at(9));
                                uint16_t lowerBoundY = stoi(currentFrameValues->at(10));
                                uint16_t upperBoundX = stoi(currentFrameValues->at(11));
                                uint16_t upperBoundY = stoi(currentFrameValues->at(12));

                                // An sprite may contain some areas defined by polygons in order to check possible collisions with other objects during the gameplay
                                currentAreas = new SpriteAreas();
                                currentObjectSpriteSheetAnimation->AddSprite({ width, height, xOffset, yOffset, u1, v1, u2, v2, duration, false, lowerBoundX, lowerBoundY, upperBoundX, upperBoundY, currentAreas });
                        }
                }

                delete currentFrameValues;
        }
}

Texture2D SceneObjectDataManager::LoadTextureAtlas() {
        return LoadTexture(FileSystem::getPath(textureFilename).c_str());
}

ObjectSpriteSheet* SceneObjectDataManager::GetSpriteSheetBySceneObjectIdentificator(SceneObjectIdentificator sceneObjectIdentificator) {
        auto searchIterator = objectSpriteSheetsMap.find(sceneObjectIdentificator);
        if (searchIterator != objectSpriteSheetsMap.end()) {
                return searchIterator->second;
        } else {
                return nullptr;
        }
}

bool startsWith(std::string mainStr, std::string toMatch)
{
        // Convert mainStr to lower case
        std::transform(mainStr.begin(), mainStr.end(), mainStr.begin(), ::tolower);
        // Convert toMatch to lower case
        std::transform(toMatch.begin(), toMatch.end(), toMatch.begin(), ::tolower);

        return (mainStr.find(toMatch) == 0);
}
