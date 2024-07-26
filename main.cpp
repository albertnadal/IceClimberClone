#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <thread>
#include <bitset>
#include <raylib/raylib.h>
#include <defines.h>
#include <entity_data_manager.h>
#include <entity_manager.h>

const uint32_t SCR_WIDTH = 1280;
const uint32_t SCR_HEIGHT = 750;
const uint32_t MAX_OBJECTS = 1000;

pthread_t gameLogicThread;
std::chrono::duration<float> cpuTimePerUpdate;
uint8_t pressedKeys = IC_KEY_NONE;
bool running = true;
EntityDataManager *entityTextureManager;
EntityManager *entityManager;
int gameLogicFrequency = 16; // 16 milliseconds ≈ 60 ticks per second
int framesPerSecond = 60;
bool paused = false;

static void* gameLogicThreadFunc(void* v)
{
        while(running) {
                if (!paused) {
                        auto t0 = std::chrono::high_resolution_clock::now();
                        entityManager->Update(pressedKeys);
                        auto t1 = std::chrono::high_resolution_clock::now();
                        cpuTimePerUpdate = t1 - t0;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(gameLogicFrequency) - cpuTimePerUpdate);
        }
        return nullptr;
}

int main()
{
        srand(static_cast<unsigned>(time(0))); // Initialize the random seed to avoid deterministic behaviours. Just for debug purposes.

        InitWindow(SCR_WIDTH, SCR_HEIGHT, "Ice Climber");

        Camera2D camera = { 0 };
        camera.target = (Vector2){ 0, 0 };
        camera.offset = (Vector2){ 0, -5060 };//-450 };
        camera.rotation = 0.0f;
        camera.zoom = 2.0f;  // 2x zoom just for debug purposes

        SetTargetFPS(framesPerSecond);

        entityTextureManager = new EntityDataManager();
        SpriteRectDoubleBuffer *spriteRectDoubleBuffer = new SpriteRectDoubleBuffer(MAX_OBJECTS);
        entityManager = new EntityManager(entityTextureManager, spriteRectDoubleBuffer, MAX_OBJECTS);

        // Load texture atlas into GPU memory
        Texture2D textureAtlas = entityTextureManager->LoadTextureAtlas();

        entityManager->Update(pressedKeys);

        pthread_create(&gameLogicThread, nullptr, gameLogicThreadFunc, nullptr);

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

                                        //auto box = spriteRectDoubleBuffer->consumer_buffer[i].boundaries;
                                        //DrawRectangleLinesEx({static_cast<float>(box.upperBoundX), static_cast<float>(box.upperBoundY), static_cast<float>(box.lowerBoundX-box.upperBoundX), static_cast<float>(box.lowerBoundY-box.upperBoundY)}, 1.0f, PINK);
                                }
                                spriteRectDoubleBuffer->unlock();
                                DrawFPS(535, 110);
                        EndMode2D();
                EndDrawing();
        }

        running = false;

        // Wait for the gameLogicThread to finish
        pthread_join(gameLogicThread, nullptr);

        delete entityTextureManager;
        delete entityManager;
        delete spriteRectDoubleBuffer;
        UnloadTexture(textureAtlas);
        CloseWindow();

        return 0;
}
