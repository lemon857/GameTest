#include <glm/vec2.hpp>

#include <iostream>
#include <chrono>

#include "Resources/ResourceManager.h"
#include "Renderer/Renderer.h"
#include "Game/Game.h"

#include "Physics/PhysicsEngine.h"
#include "Engine/Engine.h"


glm::ivec2 g_WindowSize(640, 480);

Game g_Game(g_WindowSize);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) 
{
    g_WindowSize.x = width;
    g_WindowSize.y = height;
    RenderEngine::Renderer::setViewport(width, height);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) 
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);

    }
    g_Game.setKey(key, action);
}

int main(int argc, char** argv)
{
    // Инициализация графических библиотек в движке
    if (!Engine::initGraphics())
    {
        std::cout << "Init engine failed\n";
        return -1;
    }
    // Создание окна для отображения в нём всякого прикольного
    GLFWwindow* pWindow = glfwCreateWindow(g_WindowSize.x, g_WindowSize.y, "Test Game", nullptr, nullptr);

    if (!pWindow)
    {
        std::cout << "Generate window failed\n";
        glfwTerminate();
        return -1;
    }
    // Установка калбэков для отслеживания изменения размеров окна и нажатия клавиш
    Engine::setWindowSizeCallBack(pWindow, glfwWindowSizeCallback);
    Engine::setKeyCallback(pWindow, glfwKeyCallback);
    // Выбор текущего окна
    Engine::makeContextCurrent(pWindow);
    // Вывод версии OpenGL и рендерера в консоль
    std::cout << "Renderer: " << RenderEngine::Renderer::getRendererStr() << "\n";
    std::cout << "OpenGL Version: " << RenderEngine::Renderer::getVersionStr() << "\n";
    // Установка пути приложения для подгрузки текстур
    ResourceManager::setExecutablePath(argv[0]);
	// Инициализация класса игры
    if (!g_Game.init())
    {
        std::cout << "Game init failed\n";
        return -1;
    }
    // Отчистка экрана чёрным цветом
    RenderEngine::Renderer::setClearColor(0, 0, 0, 1);
    RenderEngine::Renderer::setDepthTest(true);
    // Инициализация физического движка
    Physics::PhysicsEngine::init();
    // Сохранение времени для вычисления времени кадра
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(pWindow))
    {
        glfwPollEvents();

        auto currentTime = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
        lastTime = currentTime;

        g_Game.update(duration);

        Physics::PhysicsEngine::update(duration);

        RenderEngine::Renderer::clearColor();

        g_Game.render();

        glfwSwapBuffers(pWindow);
    }
    Engine::terminate();
    return 0;
}