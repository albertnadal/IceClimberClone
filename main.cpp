#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include "filesystem.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

typedef short int16;
typedef int int32;
typedef unsigned short uint16;
typedef unsigned int uint32;

struct Texture { uint16 width, height; float u1, v1, u2, v2; };
struct Object { int16 x, y; Texture texture; };

Texture
        watermelon = { 64, 64, 0.0f,   0.0f,   0.5f,   0.5f   },
        pineapple  = { 64, 64, 0.5f,   0.0f,   1.0f,   0.5f   },
        orange     = { 32, 32, 0.0f,   0.5f,   0.25f,  0.75f  },
        grapes     = { 32, 32, 0.25f,  0.5f,   0.5f,   0.75f  },
        pear       = { 32, 32, 0.0f,   0.75f,  0.25f,  1.0f   },
        banana     = { 32, 32, 0.25f,  0.75f,  0.5f,   1.0f   },
        strawberry = { 16, 16, 0.5f,   0.5f,   0.625f, 0.625f },
        raspberry  = { 16, 16, 0.625f, 0.5f,   0.75f,  0.625f },
        cherries   = { 16, 16, 0.5f,   0.625f, 0.625f, 0.75f  };

Texture textures[9] =
{
        watermelon,
        pineapple,
        orange,
        grapes,
        pear,
        banana,
        strawberry,
        cherries,
        raspberry
};

const uint32 OBJECT_COUNT = 100;
static Object objects[OBJECT_COUNT];
static int16 vertices[OBJECT_COUNT * 12];
static float uvs[OBJECT_COUNT * 12];
GLFWwindow* window;
unsigned int VBO, VAO, UBO, textureId;
Shader* ourShader;

void updateObject(int i)
{
        // top right
        vertices[i * 12] = objects[i].x + objects[i].texture.width;
        vertices[i * 12 + 1] = objects[i].y;

        // bottom right
        vertices[i * 12 + 2] = objects[i].x + objects[i].texture.width;
        vertices[i * 12 + 3] = objects[i].y + objects[i].texture.height;

        // top left
        vertices[i * 12 + 4] = objects[i].x;
        vertices[i * 12 + 5] = objects[i].y;

        // bottom right
        vertices[i * 12 + 6] = objects[i].x + objects[i].texture.width;
        vertices[i * 12 + 7] = objects[i].y + objects[i].texture.height;

        // bottom left
        vertices[i * 12 + 8] = objects[i].x;
        vertices[i * 12 + 9] = objects[i].y + objects[i].texture.height;

        // top left
        vertices[i * 12 + 10] = objects[i].x;
        vertices[i * 12 + 11] = objects[i].y;
}

void update()
{
        for (uint32 i = 0; i < OBJECT_COUNT; i++)
        {
                objects[i].x += rand() % 5 - 2;
                objects[i].y += rand() % 5 - 2;

                updateObject(i);
        }

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}

void render()
{
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_SCISSOR_TEST);

        ourShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, OBJECT_COUNT * 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
}

int main()
{
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

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(FileSystem::getPath("texture.png").c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
                std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);


        for (uint32 i = 0; i < OBJECT_COUNT; i++)
        {
                Texture t = textures[rand() % 9];
                objects[i] =
                {
                        (int16)(rand() % (SCR_WIDTH - t.width)),
                        (int16)(rand() % (SCR_HEIGHT - t.height)),
                        t
                };

                // top right
                vertices[i * 12] = objects[i].x + objects[i].texture.width;
                vertices[i * 12 + 1] = objects[i].y;

                // bottom right
                vertices[i * 12 + 2] = objects[i].x + objects[i].texture.width;
                vertices[i * 12 + 3] = objects[i].y + objects[i].texture.height;

                // top left
                vertices[i * 12 + 4] = objects[i].x;
                vertices[i * 12 + 5] = objects[i].y;

                // bottom right
                vertices[i * 12 + 6] = objects[i].x + objects[i].texture.width;
                vertices[i * 12 + 7] = objects[i].y + objects[i].texture.height;

                // bottom left
                vertices[i * 12 + 8] = objects[i].x;
                vertices[i * 12 + 9] = objects[i].y + objects[i].texture.height;

                // top left
                vertices[i * 12 + 10] = objects[i].x;
                vertices[i * 12 + 11] = objects[i].y;



                // top right
                uvs[i * 12] = objects[i].texture.u2;
                uvs[i * 12 + 1] = objects[i].texture.v2;

                // bottom right
                uvs[i * 12 + 2] = objects[i].texture.u2;
                uvs[i * 12 + 3] = objects[i].texture.v1;

                // top left
                uvs[i * 12 + 4] = objects[i].texture.u1;
                uvs[i * 12 + 5] = objects[i].texture.v2;

                // bottom right
                uvs[i * 12 + 6] = objects[i].texture.u2;
                uvs[i * 12 + 7] = objects[i].texture.v1;

                // bottom left
                uvs[i * 12 + 8] = objects[i].texture.u1;
                uvs[i * 12 + 9] = objects[i].texture.v1;

                // top left
                uvs[i * 12 + 10] = objects[i].texture.u1;
                uvs[i * 12 + 11] = objects[i].texture.v2;
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &UBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_SHORT, GL_FALSE, 2 * sizeof(int16), 0);

        glBindBuffer(GL_ARRAY_BUFFER, UBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 2 * sizeof(GLfloat), 0);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        ourShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindVertexArray(VAO);


        double e;

        while (!glfwWindowShouldClose(window))
        {
                processInput(window);

                e = 50.0;
                while (e>0.0)
                {
                        glfwPollEvents();
                        update();
                        e--;
                }

                render();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &UBO);
        glDeleteTextures(1, &textureId);

        glfwTerminate();
        return 0;
}

void processInput(GLFWwindow *window)
{
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
        glViewport(0, 0, width, height);
}
