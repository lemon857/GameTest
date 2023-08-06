#include <glm/vec2.hpp>

#include <iostream>
#include <chrono>

#include "Resources/ResourceManager.h"
#include "Renderer/Renderer.h"
#include "Examples/ColliderDemoGame.h"

#include "Physics/PhysicsEngine.h"
#include "Engine/Engine.h"


glm::ivec2 g_WindowSize(640, 480);

IGame* g_Game;

void windowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_Game->setWindowSize(glm::ivec2(width, height));
    RenderEngine::Renderer::setViewport(width, height);
}

void keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    g_Game->setKey(key, action);
}

void Start(int argc, char** argv)
{
    // Инициализация графических библиотек в движке
    if (!Engine::initGraphics())
    {
        std::cout << "Init graphics failed\n";
        return;
    }

    // Создание окна для отображения в нём всякого прикольного
    GLFWwindow* pWindow = glfwCreateWindow(g_WindowSize.x, g_WindowSize.y,
        "Test Game", nullptr, nullptr);

    if (!pWindow)
    {
        std::cout << "Generate window failed\n";
        glfwTerminate();
        return;
    }

    // Установка калбэков для отслеживания изменения размеров окна и нажатия клавиш
    Engine::setWindowSizeCallBack(pWindow, windowSizeCallback);
    Engine::setKeyCallback(pWindow, keyCallback);

    // Выбор текущего окна
    Engine::makeContextCurrent(pWindow);
    // Иницициализация OprnGL
    if (!Engine::initOpenGL())
    {
        std::cout << "Init OpenGL failed\n";
        return;
    }

    // Вывод версии OpenGL и рендерера в консоль
    std::cout << "Renderer: " << RenderEngine::Renderer::getRendererStr() << "\n";
    std::cout << "OpenGL Version: " << RenderEngine::Renderer::getVersionStr() << "\n";
    // Установка пути приложения для подгрузки текстур
    ResourceManager::setExecutablePath(argv[0]);
    // Инициализация класса игры
    if (!g_Game->init())
    {
        std::cout << "Game init failed\n";
        return;
    }
    // Отчистка экрана чёрным цветом
    RenderEngine::Renderer::setClearColor(0, 0, 0, 1);
    RenderEngine::Renderer::setDepthTest(true);
    // Инициализация физического движка
    Physics::PhysicsEngine::init();
}

void Update(double delta)
{
    g_Game->update(delta);

    Physics::PhysicsEngine::update(delta);

    RenderEngine::Renderer::clearColor();

    g_Game->render();
}

int main(int argc, char** argv)
{
    g_Game = new ColliderDemoGame(g_WindowSize);

    Engine::setStartFunc(Start);
    Engine::setUpdateFunc(Update);
    Engine::startEngine(argc, argv);

    delete g_Game;
    return 0;
}