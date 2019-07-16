#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <thread>
#include <bitset>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_s.h>
#include <filesystem.h>
#include <vec2.h>
#include <types.h>
#include <scene_object_texture_manager.h>
#include <scene_object_manager.h>
#include <scene_object.h>
#include <main_character.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void process_input(GLFWwindow *window);
void update_fps(GLFWwindow* window);

const uint32 SCR_WIDTH = 1280;
const uint32 SCR_HEIGHT = 750;

pthread_t gameLogicMainThreadId;

const uint32 OBJECT_COUNT = 1000;
//static uint16 vertices[OBJECT_COUNT * 12];
//static float uvs[OBJECT_COUNT * 12];

int nbFrames = 0;
int previousFPS = 0;
double lastTime = glfwGetTime();
uchar pressedKeys = KEY_NONE;
bool running = true;

GLFWwindow* window;
uint32 VBO, VAO, UBO, textureId;
Shader* ourShader;
SceneObjectTextureManager *objectTextureManager;
SceneObjectManager *sceneObjectManager;

void render()
{
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glFrontFace(GL_CCW);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_SCISSOR_TEST);

/*
        ourShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
 */
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, OBJECT_COUNT * 6);

        glfwSwapBuffers(window);
        //glfwPollEvents();
}

static void* gameLogicMainThreadFunc(void* v)
{
  while(running) {
    sceneObjectManager->Update(pressedKeys);
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
  return 0;
}

int main()
{
        objectTextureManager = new SceneObjectTextureManager();
        UInt16DoubleBuffer *verticesDoubleBuffer = new UInt16DoubleBuffer(OBJECT_COUNT * 12);
        FloatDoubleBuffer *uvsDoubleBuffer = new FloatDoubleBuffer(OBJECT_COUNT * 12);
        sceneObjectManager = new SceneObjectManager(objectTextureManager, verticesDoubleBuffer, uvsDoubleBuffer, OBJECT_COUNT);

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rocket", NULL, NULL);
        if (window == NULL)
        {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, keyboard_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
                std::cout << "Failed to initialize GLAD" << std::endl;
                return -1;
        }

        ourShader = new Shader("shader.vs", "shader.fs");

        // Load texture atlas into GPU memory
        textureId = objectTextureManager->LoadObjectsTextures();

        // Initial scene update
        sceneObjectManager->Update(pressedKeys);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &UBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, verticesDoubleBuffer->size(), verticesDoubleBuffer->consumer_buffer, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_UNSIGNED_SHORT, GL_FALSE, 2 * sizeof(uint16), 0);

        glBindBuffer(GL_ARRAY_BUFFER, UBO);
        glBufferData(GL_ARRAY_BUFFER, uvsDoubleBuffer->size(), uvsDoubleBuffer->consumer_buffer, GL_DYNAMIC_DRAW /*GL_STATIC_DRAW*/);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 2 * sizeof(float), 0);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        ourShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindVertexArray(VAO);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        pthread_create(&gameLogicMainThreadId, NULL, gameLogicMainThreadFunc, 0);

        while (!glfwWindowShouldClose(window))
        {
                process_input(window);
                glfwPollEvents();

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                verticesDoubleBuffer->lock();
                glBufferSubData(GL_ARRAY_BUFFER, 0, verticesDoubleBuffer->size(), verticesDoubleBuffer->consumer_buffer);
                verticesDoubleBuffer->unlock();

                glBindBuffer(GL_ARRAY_BUFFER, UBO);
                uvsDoubleBuffer->lock();
                glBufferSubData(GL_ARRAY_BUFFER, 0, uvsDoubleBuffer->size(), uvsDoubleBuffer->consumer_buffer);
                uvsDoubleBuffer->unlock();

                render();
                update_fps(window);
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &UBO);
        glDeleteTextures(1, &textureId);

        glfwTerminate();

        running = false;
        delete objectTextureManager;
        delete sceneObjectManager;
        delete verticesDoubleBuffer;
        delete uvsDoubleBuffer;

        return 0;
}

void process_input(GLFWwindow *window)
{
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
        glViewport(0, 0, width, height);
}

void keyboard_callback(GLFWwindow* window, int key, int32 scancode, int32 action, int32 mode)
{
        KeyboardKeyCode keyCode = KEY_NONE;
        switch(key) {
          case GLFW_KEY_LEFT:   keyCode = KEY_LEFT; break;
          case GLFW_KEY_RIGHT:  keyCode = KEY_RIGHT; break;
          case GLFW_KEY_UP:     keyCode = KEY_UP; break;
          case GLFW_KEY_DOWN:   keyCode = KEY_DOWN; break;
          case GLFW_KEY_Q:      keyCode = KEY_Q; break;
          case GLFW_KEY_W:      keyCode = KEY_W; break;
          case GLFW_KEY_A:      keyCode = KEY_A; break;
          case GLFW_KEY_SPACE:  keyCode = KEY_SPACE; break;
          case GLFW_KEY_ESCAPE: if(action == GLFW_PRESS) { glfwSetWindowShouldClose(window, GL_TRUE); } keyCode = KEY_NONE; break;
        }

        if(keyCode != KEY_NONE) {
                if((action == GLFW_PRESS) || (action == GLFW_RELEASE)) {
                        pressedKeys = pressedKeys ^ keyCode;
                }

                std::bitset<8> x(pressedKeys);
                std::cout << "KEYS: " << x << endl;
        }
}

void update_fps(GLFWwindow* win)
{
        double currentTime = glfwGetTime();
        nbFrames++;

        if(previousFPS >= 60) {
          std::this_thread::sleep_for(std::chrono::milliseconds((int)(10)));
        }

        if ( currentTime - lastTime >= 1.0 ) { // If last count was more than 1 sec ago
                char title [256];
                title[255] = '\0';

                snprintf(title, 255, "%s - [FPS: %d] [Frame time: %f]", "Rocket", nbFrames, 1000.0f/nbFrames);
                glfwSetWindowTitle(win, title);

                previousFPS = nbFrames;
                nbFrames = 0;
                lastTime = currentTime;
        }
}
