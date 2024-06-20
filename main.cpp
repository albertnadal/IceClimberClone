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

const uint32_t SCR_WIDTH = 1280;
const uint32_t SCR_HEIGHT = 750;
const uint32_t MAX_OBJECTS = 1000;

pthread_t gameLogicMainThreadId;
std::chrono::duration<float> cpuTimePerUpdate;
uint8_t pressedKeys = IC_KEY_NONE;
bool running = true;
SceneObjectDataManager *objectTextureManager;
SceneObjectManager *sceneObjectManager;
int gameLogicFrequency = 16;
bool paused = false;

static void* gameLogicMainThreadFunc(void* v)
{
        while(running) {
                if (!paused) {
                        auto t0 = std::chrono::high_resolution_clock::now();
                        sceneObjectManager->Update(pressedKeys);
                        auto t1 = std::chrono::high_resolution_clock::now();
                        cpuTimePerUpdate = t1 - t0;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(gameLogicFrequency) - cpuTimePerUpdate);
        }
        return 0;
}

int main()
{
        InitWindow(SCR_WIDTH, SCR_HEIGHT, "Ice Climber");

        Camera2D camera = { 0 };
        camera.target = (Vector2){ 0, 0 };
        camera.offset = (Vector2){ 0, -150 };//-450 };
        camera.rotation = 0.0f;
        camera.zoom = 2.0f;  // 2x zoom just for debug purposes

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
                if (IsKeyPressed(KEY_RIGHT) || IsKeyReleased(KEY_RIGHT)) pressedKeys ^= IC_KEY_RIGHT;
                if (IsKeyPressed(KEY_LEFT) || IsKeyReleased(KEY_LEFT)) pressedKeys ^= IC_KEY_LEFT;
                if (IsKeyPressed(KEY_UP) || IsKeyReleased(KEY_UP)) pressedKeys ^= IC_KEY_UP;
                if (IsKeyPressed(KEY_DOWN) || IsKeyReleased(KEY_DOWN)) pressedKeys ^= IC_KEY_DOWN;
                if (IsKeyPressed(KEY_Q) || IsKeyReleased(KEY_Q)) pressedKeys ^= IC_KEY_Q;
                if (IsKeyPressed(KEY_W) || IsKeyReleased(KEY_W)) pressedKeys ^= IC_KEY_W;
                if (IsKeyPressed(KEY_A) || IsKeyReleased(KEY_A)) pressedKeys ^= IC_KEY_A;
                if (IsKeyPressed(KEY_SPACE) || IsKeyReleased(KEY_SPACE)) pressedKeys ^= IC_KEY_SPACE;
                if (IsKeyPressed(KEY_ESCAPE) || IsKeyReleased(KEY_ESCAPE)) pressedKeys ^= IC_KEY_DOWN;

                if (IsKeyPressed(KEY_P)) gameLogicFrequency += 10;
                if (IsKeyPressed(KEY_O)) gameLogicFrequency -= 10;
                if (IsKeyPressed(KEY_M)) paused = !paused;

                BeginDrawing();
                        ClearBackground(BLACK);
                        BeginMode2D(camera);
                                spriteRectDoubleBuffer->lock();
                                for(int i=0; i<spriteRectDoubleBuffer->consumer_buffer_length; i++) {
                                        auto position = spriteRectDoubleBuffer->consumer_buffer[i].position;
                                        auto source = spriteRectDoubleBuffer->consumer_buffer[i].source;
                                        auto tint = spriteRectDoubleBuffer->consumer_buffer[i].tint;
                                        DrawTextureRec(textureAtlas, source, position, tint);

                                        auto box = spriteRectDoubleBuffer->consumer_buffer[i].boundaries;
                                        DrawRectangleLinesEx({static_cast<float>(box.upperBoundX), static_cast<float>(box.upperBoundY), static_cast<float>(box.lowerBoundX-box.upperBoundX), static_cast<float>(box.lowerBoundY-box.upperBoundY)}, 1.0f, PINK);
                                }
                                spriteRectDoubleBuffer->unlock();
                                DrawFPS(550, 10);
                        EndMode2D();
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
