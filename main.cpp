#include <iostream>
#include <vector>
#include <cmath>
#include <mutex>
#include <thread>
#include <bitset>
#include <raylib.h>
#include <defines.h>
#include <entity_data_manager.h>
#include <game_manager.h>
#include <sound_manager.h>
#include <utils.h>
#include <main_menu_screen.cpp>
#include <score_screen.cpp>

constexpr uint32_t SCR_WIDTH = 32 * CELL_HEIGHT * ZOOM;  // REVISIT: The width should be a fixed value and the zoom value should be calculated based on the screen height.
constexpr uint32_t SCR_HEIGHT = 30 * CELL_HEIGHT * ZOOM; // REVISIT: The height should be a fixed value and the zoom value should be calculated based on the screen height.
constexpr uint32_t MAX_OBJECTS = 1000;

std::thread gameLogicThread;
std::chrono::duration<float> cpuTimePerUpdate;
uint8_t pressedKeys = IC_KEY_NONE;
bool exitGame = false;
bool isGameFinished = false;
bool isGameOver = false;
EntityDataManager* entityTextureManager;
GameManager* gameManager;
SoundManager* soundManager;
int gameLogicFrequency = MILLISECONDS_PER_TICK;
bool paused = false;
std::optional<int> lifeCounter;
std::mutex lifeCounterMutex;
std::optional<float> cameraVerticalPosition;
std::mutex cameraVerticalPositionMutex;
GameScoreSummary scoreSummary;
GameScreenType currentGameScreen;
int mountainNumber = 1;
int accumulatedScore = 0;
int highScore = 0;
Music titleScreenMusic;
Music mountainGamePlayMusic;
Music stageClearMusic;

static void gameLogicThreadFunc() {
        UpdateInfo info;
        while (!exitGame && !isGameFinished) {
                if (!paused) {
                        auto t0 = std::chrono::high_resolution_clock::now();
                        info = gameManager->Update(pressedKeys);
                        isGameFinished = info.gameFinished;

                        cameraVerticalPositionMutex.lock();
                        cameraVerticalPosition = info.currentCameraVerticalPosition;
                        cameraVerticalPositionMutex.unlock();

                        lifeCounterMutex.lock();
                        lifeCounter = info.lifeCounter;
                        lifeCounterMutex.unlock();

                        auto t1 = std::chrono::high_resolution_clock::now();
                        cpuTimePerUpdate = t1 - t0;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(gameLogicFrequency) - cpuTimePerUpdate);
        }

        isGameOver = gameManager->IsGameOver();

        // Update accumulated score and update high score if needed
        scoreSummary = gameManager->GetGameScoreSummary();
        accumulatedScore += ((scoreSummary.vegetableCount * scoreSummary.vegetableUnitScore) + (scoreSummary.iceCount * scoreSummary.iceUnitScore) + (scoreSummary.nitpickerCount * scoreSummary.nitpickerUnitScore) + (scoreSummary.brickCount * scoreSummary.brickUnitScore) + (scoreSummary.condorHunted ? scoreSummary.condorUnitScore : 0));
        if (accumulatedScore > highScore) {
                highScore = accumulatedScore;
                Utils::saveHighscoreToFile(highScore, HIGHSCORE_FILENAME);
        }

        PlayMusicStream(stageClearMusic);
        currentGameScreen = GameScreenType::PLAYER_SCORE_SUMMARY;
}

inline void processKeyboardInput() {
        if (IsKeyPressed(KEY_RIGHT) || IsKeyReleased(KEY_RIGHT)) pressedKeys ^= IC_KEY_RIGHT;
        if (IsKeyPressed(KEY_LEFT) || IsKeyReleased(KEY_LEFT)) pressedKeys ^= IC_KEY_LEFT;
        if (IsKeyPressed(KEY_UP) || IsKeyReleased(KEY_UP)) pressedKeys ^= IC_KEY_UP;
        if (IsKeyPressed(KEY_SPACE) || IsKeyReleased(KEY_SPACE)) pressedKeys ^= IC_KEY_SPACE;
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyReleased(KEY_ESCAPE)) pressedKeys ^= IC_KEY_ESCAPE;

        if (DEBUG) {
                if (IsKeyPressed(KEY_P)) gameLogicFrequency += 10;
                if (IsKeyPressed(KEY_O)) gameLogicFrequency -= 10;
                if (IsKeyPressed(KEY_M)) paused = !paused;
        }
}

int main() {
        if (!DEBUG) {
                SetTraceLogLevel(LOG_NONE);
        }

        srand(static_cast<unsigned>(time(0))); // Initialize the random seed to avoid deterministic behaviours. Just for debug purposes.

        InitWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE);
        SetExitKey(0); // Disable close the application when pressing Esc key.
        InitAudioDevice();
        highScore = Utils::loadHighscoreFromFile(HIGHSCORE_FILENAME);
        Utils::loadGameMusic(titleScreenMusic, mountainGamePlayMusic, stageClearMusic);
        PlayMusicStream(titleScreenMusic);
        currentGameScreen = GameScreenType::MAIN_MENU;

        // Camera configuration for simple static screens
        Camera2D staticCamera;
        staticCamera.target = Vector2{ 0, 0 };
        staticCamera.offset = Vector2{ 0, 0 };
        staticCamera.rotation = 0.0f;
        staticCamera.zoom = ZOOM;

        // Camera configuration for mountain game play screen
        Camera2D mountainCamera;
        mountainCamera.target = Vector2{ 0, 0 };
        mountainCamera.offset = Vector2{ 0, -INITIAL_CAMERA_POSITION * ZOOM };
        mountainCamera.rotation = 0.0f;
        mountainCamera.zoom = ZOOM;

        SetTargetFPS(FPS);

        entityTextureManager = new EntityDataManager();
        SpriteRectDoubleBuffer* spriteRectDoubleBuffer = new SpriteRectDoubleBuffer(MAX_OBJECTS);
        soundManager = new SoundManager();
        gameManager = new GameManager(soundManager, entityTextureManager, spriteRectDoubleBuffer, MAX_OBJECTS);
        std::optional<int> lifeCounterCopy;

        // Load texture atlas into GPU memory
        Texture2D textureAtlas = entityTextureManager->LoadTextureAtlas();

        while (!WindowShouldClose() && !exitGame) {
                if (currentGameScreen == GameScreenType::MOUNTAIN_GAME_PLAY) {
                        processKeyboardInput();
                        UpdateMusicStream(mountainGamePlayMusic);

                        BeginDrawing();
                        ClearBackground(BLACK);

                        cameraVerticalPositionMutex.lock();
                        if (cameraVerticalPosition.has_value()) {
                                mountainCamera.offset = Vector2{ 0, -(cameraVerticalPosition.value()) * ZOOM };
                        }
                        cameraVerticalPositionMutex.unlock();

                        BeginMode2D(mountainCamera);
                        spriteRectDoubleBuffer->lock();
                        for (int i = 0; i < spriteRectDoubleBuffer->consumer_buffer_length; i++) {
                                auto position = spriteRectDoubleBuffer->consumer_buffer[i].position;
                                auto source = spriteRectDoubleBuffer->consumer_buffer[i].source;
                                auto tint = spriteRectDoubleBuffer->consumer_buffer[i].tint;
                                DrawTextureRec(textureAtlas, source, position, tint);

                                if (DEBUG) {
                                        // Draw solid bondaries only for debug purposes
                                        auto box = spriteRectDoubleBuffer->consumer_buffer[i].boundaries;
                                        DrawRectangleLinesEx({ static_cast<float>(box.upperBoundX), static_cast<float>(box.upperBoundY), static_cast<float>(box.lowerBoundX - box.upperBoundX), static_cast<float>(box.lowerBoundY - box.upperBoundY) }, 1.0f, PINK);

                                        // Draw attack bondaries only for debug purposes
                                        box = spriteRectDoubleBuffer->consumer_buffer[i].attackBoundaries;
                                        DrawRectangleLinesEx({ static_cast<float>(box.upperBoundX), static_cast<float>(box.upperBoundY), static_cast<float>(box.lowerBoundX - box.upperBoundX), static_cast<float>(box.lowerBoundY - box.upperBoundY) }, 1.0f, YELLOW);
                                }
                        }
                        spriteRectDoubleBuffer->unlock();
                        EndMode2D();

                        lifeCounterMutex.lock();
                        lifeCounterCopy = lifeCounter;
                        lifeCounterMutex.unlock();

                        BeginMode2D(staticCamera);
                        if (lifeCounterCopy.has_value()) {
                                for (float i = 0; i < lifeCounterCopy.value(); i++) {
                                        DrawTextureRec(textureAtlas, LIFE_COUNTER_SPRITE_RECT, { LIFE_COUNTER_X + (i * 18), LIFE_COUNTER_Y }, WHITE);
                                }
                        }
                        EndMode2D();

                        if (DEBUG) {
                                DrawFPS(16, 16);
                        }
                        EndDrawing();
                } else if (currentGameScreen == GameScreenType::PLAYER_SCORE_SUMMARY) {
                        UpdateMusicStream(stageClearMusic);
                        renderScoreScreen(textureAtlas, staticCamera, scoreSummary, mountainNumber, accumulatedScore);

                        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                                StopMusicStream(stageClearMusic);
                                if (isGameOver) {
                                        // Go to main menu
                                        mountainNumber = 1;
                                        accumulatedScore = 0;
                                        currentGameScreen = GameScreenType::MAIN_MENU;
                                        StopMusicStream(titleScreenMusic);
                                        PlayMusicStream(titleScreenMusic);
                                } else {
                                        // Play the next mountain available
                                        isGameFinished = false;
                                        mountainNumber = (mountainNumber % TOTAL_MOUNTAINS) + 1;
                                        cameraVerticalPosition = std::nullopt;
                                        mountainCamera.offset = Vector2{ 0, -INITIAL_CAMERA_POSITION * ZOOM };

                                        pressedKeys = IC_KEY_NONE;
                                        gameManager->SetupMountain(mountainNumber);
                                        currentGameScreen = GameScreenType::MOUNTAIN_GAME_PLAY;
                                        StopMusicStream(mountainGamePlayMusic);
                                        PlayMusicStream(mountainGamePlayMusic);
                                        gameLogicThread = std::thread(gameLogicThreadFunc);
                                }
                        }
                } else if (currentGameScreen == GameScreenType::MAIN_MENU) {
                        PollInputEvents();
                        UpdateMusicStream(titleScreenMusic);
                        renderMainMenuScreen(textureAtlas, staticCamera, mountainNumber, highScore);

                        if (IsKeyPressed(KEY_ESCAPE)) {
                                exitGame = true;
                        } else if (IsKeyPressed(KEY_UP)) {
                                mountainNumber = (mountainNumber % TOTAL_MOUNTAINS) + 1;
                                gameManager->PlaySoundById(SoundIdentificator::SELECT_SOUND);
                        } else if (IsKeyPressed(KEY_DOWN)) {
                                mountainNumber = (mountainNumber - 2 + TOTAL_MOUNTAINS) % TOTAL_MOUNTAINS + 1;
                                gameManager->PlaySoundById(SoundIdentificator::SELECT_SOUND);
                        } else if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                                StopMusicStream(titleScreenMusic);
                                StopMusicStream(mountainGamePlayMusic);
                                PlayMusicStream(mountainGamePlayMusic);
                                accumulatedScore = 0;
                                lifeCounter = std::nullopt;
                                cameraVerticalPosition = std::nullopt;
                                mountainCamera.offset = Vector2{ 0, -INITIAL_CAMERA_POSITION * ZOOM };
                                isGameFinished = false;
                                isGameOver = false;

                                pressedKeys = IC_KEY_NONE;
                                gameManager->SetupMountain(mountainNumber);
                                currentGameScreen = GameScreenType::MOUNTAIN_GAME_PLAY;
                                gameLogicThread = std::thread(gameLogicThreadFunc);
                        }
                }
        }

        exitGame = true;

        // Wait for the gameLogicThread to finish
        if (gameLogicThread.joinable()) {
                gameLogicThread.join();
        }

        delete soundManager; // This also unloads sounds
        delete entityTextureManager;
        delete gameManager;
        delete spriteRectDoubleBuffer;

        UnloadTexture(textureAtlas);
        UnloadMusicStream(titleScreenMusic);
        UnloadMusicStream(mountainGamePlayMusic);
        UnloadMusicStream(stageClearMusic);
        CloseAudioDevice();
        CloseWindow();

        return 0;
}
