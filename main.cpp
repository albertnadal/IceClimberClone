#include <iostream>
#include <vector>
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
void process_input(GLFWwindow *window);
void update_fps(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const uint32 OBJECT_COUNT = 3000;
static uint16 vertices[OBJECT_COUNT * 12];
static float uvs[OBJECT_COUNT * 12];

int nbFrames = 0;
double lastTime = glfwGetTime();

GLFWwindow* window;
unsigned int VBO, VAO, UBO, textureId;
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

int main()
{
        objectTextureManager = new SceneObjectTextureManager();
        sceneObjectManager = new SceneObjectManager(objectTextureManager, vertices, &VBO, uvs, &UBO);

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
        sceneObjectManager->Update();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &UBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 2 * sizeof(uint16), 0);

        glBindBuffer(GL_ARRAY_BUFFER, UBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_DYNAMIC_DRAW/*GL_STATIC_DRAW*/);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 2 * sizeof(GLfloat), 0);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        ourShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindVertexArray(VAO);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        while (!glfwWindowShouldClose(window))
        {
                process_input(window);
                glfwPollEvents();
                sceneObjectManager->Update();
                render();
                update_fps(window);
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &UBO);
        glDeleteTextures(1, &textureId);

        glfwTerminate();
        delete objectTextureManager;

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

void update_fps(GLFWwindow* win)
{
  double currentTime = glfwGetTime();
  nbFrames++;

  if ( currentTime - lastTime >= 1.0 ) { // If last count was more than 1 sec ago
    char title [256];
    title[255] = '\0';

    snprintf(title, 255, "%s - [FPS: %d] [Frame time: %f]", "Rocket", nbFrames, 1000.0f/nbFrames);
    glfwSetWindowTitle(win, title);

    nbFrames = 0;
    lastTime = currentTime;
  }
}
