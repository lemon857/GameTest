#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <chrono>

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Renderer/AnimatedSprite.h"
#include "Resources/ResourceManager.h"

GLfloat points[] = {
    -50.0f, 50.0f, 0.0f,
    50.0f, -50.0f, 0.0f,
    -50.0f, -50.0f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat texCoord[] = {
    0.0f, 0.5f,
    0.5f, 0.0f,
    0.0f, 0.0f
};

glm::vec2 g_WindowSize(640, 480);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) 
{
    g_WindowSize.x = width;
    g_WindowSize.y = height;
    glViewport(0, 0, width, height);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) 
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(pWindow, GL_TRUE);
        }
    }
}

int main(int argc, char** argv)
{
    if (!glfwInit())
    {
        std::cout << "glfwInit failed\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pWindow = glfwCreateWindow(g_WindowSize.x, g_WindowSize.y, "Test Game", nullptr, nullptr);

    if (!pWindow)
    {

        std::cout << "glfwGenerateWindow failed\n";
        glfwTerminate();
        return -1;
    }
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    glfwMakeContextCurrent(pWindow);

	if (!gladLoadGL())
	{
		std::cout << "gladLoadGL failed\n";
		return -1;
	}
	
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
	
	glClearColor(0, 0, 0, 1);   

    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "Can't create shader program!\n";
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram)
        {
            std::cerr << "Can't create shader program!\n";
            return -1;
        }

        auto pTex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        std::vector<std::string> subStringNames{ "eagle", "deadEagle", "respawn0", "respawn1", "respawn2", "respawn3", "respawn4", "respawn5" };
        std::vector<std::string> subStringNamesTanks{ 
            "YellowUp11", 
            "YellowUp12", 
            "YellowLeft11",
            "YellowLeft12", 
            "YellowDown11", 
            "YellowDown12", 
            "YellowRight11", 
            "YellowRight12" 
        };
        std::vector <std::pair<std::string, uint64_t>> spawnState;

        std::vector <std::pair<std::string, uint64_t>> tankState;


        tankState.emplace_back(std::make_pair<std::string, uint64_t>("YellowUp11", 70000000));
        tankState.emplace_back(std::make_pair<std::string, uint64_t>("YellowUp12", 70000000));

        spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn0", 50000000));
        spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn1", 50000000));
        spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn2", 50000000));
        spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn3", 50000000));
        spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn4", 80000000));
        spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn5", 100000000));
        spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn4", 100000000));
        spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn5", 100000000));
        spawnState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 300000000));

        auto pTexAtlas = resourceManager.loadTextureAtlas("DefaultTextureAtlas", subStringNames,  "res/textures/map_16x16.png", 16, 16);
        auto pTexAtlasTank = resourceManager.loadTextureAtlas("TanksTextureAtlas", subStringNamesTanks, "res/textures/tanks.png", 16, 16);

        auto pAnimatedSprite = resourceManager.loadAnimatedSprite("AnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, 0, "deadEagle");
        pAnimatedSprite->setPosition(glm::vec2(240, 350));

        auto pTankSprite = resourceManager.loadAnimatedSprite("TankSprite", "TanksTextureAtlas", "SpriteShader", 100, 100, 0, "YellowUp11");
        pTankSprite->setPosition(glm::vec2(240, 120));

        pAnimatedSprite->insertState("waterState", std::move(spawnState));
        pAnimatedSprite->setState("waterState");

        pTankSprite->insertState("up1", std::move(tankState));
        pTankSprite->setState("up1");

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0); 

        glm::mat4 modelMatrix_2 = glm::mat4(1.0f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(450.0f, 50.0f, 0.0f));

        glm::mat4 modelMatrix_1 = glm::mat4(1.0f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.0f, 50.0f, 0.0f));

        glm::mat4 projectionMatrix = glm::ortho(0.0f, g_WindowSize.x, 0.0f, g_WindowSize.y, -100.0f, 100.0f);

        pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        auto lastTime = std::chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(pWindow))
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;
            pAnimatedSprite->update(duration);
            pTankSprite->update(duration);

            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            pTex->bind();

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pAnimatedSprite->render();
            pTankSprite->render();

            glfwSwapBuffers(pWindow);
            
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}