#include "scene_object_data_manager.h"
#include <fstream>
#include <sstream>
#include <utils.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
        int currentObjectSpriteSheetAnimationId;

        while (std::getline(infile, line))
        {
                std::istringstream iss(line);
                string token;
                bool commentFound = false;
                std::vector<string> *currentframeValues = new std::vector<string>;
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
                                int objectSpriteSheetAnimationId = std::stoi(token.substr(1));
                                currentObjectSpriteSheetAnimation = new ObjectSpriteSheetAnimation(objectSpriteSheetAnimationId);
                                currentObjectSpriteSheet->AddAnimation(currentObjectSpriteSheetAnimation);
                        } else if(startsWith(token, "_")) {
                                currentLineType = OBJ_SPRITE_COLLISION_AREA;
                                int objectSpriteSheetCollisionAreaId = std::stoi(token.substr(1));
                                std::cout << "COLLISION AREA ID: " << objectSpriteSheetCollisionAreaId << std::endl;
                                //currentObjectSpriteSheetAnimation = new ObjectSpriteSheetAnimation(objectSpriteSheetAnimationId);
                                //currentObjectSpriteSheet->AddAnimation(currentObjectSpriteSheetAnimation);
                        } else {
                                currentLineType = OBJ_SPRITE;
                                currentframeValues->push_back(token);
                        }
                }

                if(currentLineType == OBJ_SPRITE) {
                        if(currentframeValues->size() == 11) {
                                uint16 width = stoi(currentframeValues->at(0));
                                uint16 height = stoi(currentframeValues->at(1));
                                float u1 = stof(currentframeValues->at(2));
                                float v1 = stof(currentframeValues->at(3));
                                float u2 = stof(currentframeValues->at(4));
                                float v2 = stof(currentframeValues->at(5));
                                uint16 duration = stoi(currentframeValues->at(6));
                                uint16 lowerBoundX = stoi(currentframeValues->at(7));
                                uint16 lowerBoundY = stoi(currentframeValues->at(8));
                                uint16 upperBoundX = stoi(currentframeValues->at(9));
                                uint16 upperBoundY = stoi(currentframeValues->at(10));
                                currentObjectSpriteSheetAnimation->AddSprite({ width, height, u1, v1, u2, v2, duration, false, lowerBoundX, lowerBoundY, upperBoundX, upperBoundY });
                        }
                }
        }
}

uint32 SceneObjectDataManager::LoadObjectsTextures() {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(FileSystem::getPath(textureFilename).c_str(), &width, &height, &nrChannels, 0);

        if(data)
        {
                // Remove the chroma key color (##ff00ffff) of the texture atlas
                for(int i=0; i < width*height*sizeof(GL_RGBA); i+=sizeof(GL_RGBA)) {
                        if((data[i] == 255) && (data[i+1] == 0) && (data[i+2] == 255) && (data[i+3] == 255)) {
                                data[i] = data[i+1] = data[i+2] = data[i+3] = 0;
                        }
                }

                // Save the texture atlas in the vram
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
                std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);
        return textureId;
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
