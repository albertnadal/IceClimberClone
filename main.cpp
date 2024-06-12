#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <thread>
#include <bitset>
#include <raylib/raylib.h>
#include <defines.h>
#include <scene_object_data_manager.h>
#include <scene_object_manager.h>

//void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const uint32_t SCR_WIDTH = 1280;
const uint32_t SCR_HEIGHT = 750;
const uint32_t MAX_OBJECTS = 1000;

pthread_t gameLogicMainThreadId;
std::chrono::duration<float> cpuTimePerUpdate;
uint8_t pressedKeys = HC_KEY_NONE;
bool running = true;
SceneObjectDataManager *objectTextureManager;
SceneObjectManager *sceneObjectManager;

static void* gameLogicMainThreadFunc(void* v)
{
        while(running) {
                auto t0 = std::chrono::high_resolution_clock::now();
                sceneObjectManager->Update(pressedKeys);
                auto t1 = std::chrono::high_resolution_clock::now();
                cpuTimePerUpdate = t1 - t0;
                std::this_thread::sleep_for(std::chrono::milliseconds(16) - cpuTimePerUpdate);
        }
        return 0;
}

int main()
{
        InitWindow(SCR_WIDTH, SCR_HEIGHT, "Ice Climber");
        SetTargetFPS(30);

        objectTextureManager = new SceneObjectDataManager();
        SpriteRectDoubleBuffer *spriteRectDoubleBuffer = new SpriteRectDoubleBuffer(MAX_OBJECTS);
        sceneObjectManager = new SceneObjectManager(objectTextureManager, spriteRectDoubleBuffer, MAX_OBJECTS);

        // Load texture atlas into GPU memory
        Texture2D textureAtlas = objectTextureManager->LoadTextureAtlas();

        sceneObjectManager->Update(pressedKeys);

        pthread_create(&gameLogicMainThreadId, NULL, gameLogicMainThreadFunc, 0);

        while (!WindowShouldClose())
        {
                BeginDrawing();
                        ClearBackground(RAYWHITE);
                        spriteRectDoubleBuffer->lock();
                        for(int i=0; i<spriteRectDoubleBuffer->consumer_buffer_length; i++) {
                                auto position = spriteRectDoubleBuffer->consumer_buffer[i].position;
                                auto source = spriteRectDoubleBuffer->consumer_buffer[i].source;
                                DrawTextureRec(textureAtlas, spriteRectDoubleBuffer->consumer_buffer[i].source, spriteRectDoubleBuffer->consumer_buffer[i].position, WHITE);
                        }
                        spriteRectDoubleBuffer->unlock();
                        DrawFPS(550, 10);
                EndDrawing();
        }

        running = false;
        delete objectTextureManager;
        delete sceneObjectManager;
        delete spriteRectDoubleBuffer;
        UnloadTexture(textureAtlas);
        CloseWindow();

        return 0;
}

/*
void keyboard_callback(GLFWwindow* window, int key, int32_t scancode, int32_t action, int32_t mode)
{
        KeyboardKeyCode keyCode = HC_KEY_NONE;
        switch(key) {
        case GLFW_KEY_LEFT:   keyCode = HC_KEY_LEFT; break;
        case GLFW_KEY_RIGHT:  keyCode = HC_KEY_RIGHT; break;
        case GLFW_KEY_UP:     keyCode = HC_KEY_UP; break;
        case GLFW_KEY_DOWN:   keyCode = HC_KEY_DOWN; break;
        case GLFW_KEY_Q:      keyCode = HC_KEY_Q; break;
        case GLFW_KEY_W:      keyCode = HC_KEY_W; break;
        case GLFW_KEY_A:      keyCode = HC_KEY_A; break;
        case GLFW_KEY_SPACE:  keyCode = HC_KEY_SPACE; break;
        case GLFW_KEY_ESCAPE: if(action == GLFW_PRESS) { glfwSetWindowShouldClose(window, GL_TRUE); } keyCode = HC_KEY_NONE; break;
        }

        if(keyCode != HC_KEY_NONE) {
                if((action == GLFW_PRESS) || (action == GLFW_RELEASE)) {
                        pressedKeys = pressedKeys ^ keyCode;
                }

                std::bitset<8> x(pressedKeys);
        }
}
*/
