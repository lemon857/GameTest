//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <glm/vec2.hpp>

#include <iostream>
#include <chrono>

#include "EngineCore/Window.h"

#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "Examples/TanksDemo/TanksDemoGame.h"
#include "Examples/SnakeDemo/SnakeDemoGame.h"
#include "Examples/PongDemo/PongDemoGame.h"

#include "EngineCore/Physics/PhysicsEngine.h"
#include "EngineCore/Application.h"
#include "EngineCore/System/Log.h"

class MyApp : public Application
{
public:
    MyApp()
    {

    }
    ~MyApp()
    {

    }
private:
};

void keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    //g_Game->setKey(key, action);
}

int main(int argc, char** argv)
{
    glm::ivec2 g_WindowSize(800, 600);

    //IGame* g_Game;
    ResourceManager::setExecutablePath(argv[0]);
    //g_Game = new TanksDemoGame(g_WindowSize);
    //g_Game = new SnakeDemoGame(g_WindowSize, glm::vec2(100));
    //g_Game = new PongDemoGame(g_WindowSize);

    Application* myApp = new MyApp();

    myApp->start(g_WindowSize, "Test game");

    delete myApp;

    return 0;
}