#include "Engine.h"
#include "../Resources/ResourceManager.h"

#include <iostream>
#include <chrono>

std::function<void(int, char**)> Engine::m_start;
std::function<void(double)> Engine::m_update;
GLFWwindow* Engine::m_window;

bool Engine::initGraphics()
{
    if (!glfwInit())
    {
        std::cout << "GLFW init failed\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}
bool Engine::initOpenGL()
{
    if (!gladLoadGL())
    {
        std::cout << "Glad load failed\n";
        return false;
    }

    return true;
}
void Engine::setWindowSizeCallBack(GLFWwindow* window, GLFWwindowsizefun callback)
{
    glfwSetWindowSizeCallback(window, callback);
}
void Engine::setKeyCallback(GLFWwindow* window, GLFWkeyfun callback)
{
    glfwSetKeyCallback(window, callback);
}
void Engine::makeContextCurrent(GLFWwindow* window)
{
    glfwMakeContextCurrent(window);
    m_window = window;
}
void Engine::terminate()
{
    ResourceManager::unloadAllResources();
    glfwTerminate();
}
void Engine::poolEvents()
{
    glfwPollEvents();
}
int Engine::defaultStart(int argc, char** argv)
{
    return 0;
}
void Engine::setStartFunc(std::function<void(int, char**)> start)
{
    m_start = start;
}
void Engine::setUpdateFunc(std::function<void(double)> update)
{
    m_update = update;
}
void Engine::startEngine(int argc, char** argv)
{
    m_start(argc, argv);
    // —охранение времени дл€ вычислени€ времени кадра
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(m_window))
    {
        poolEvents();

        auto currentTime = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
        lastTime = currentTime;

        m_update(duration);

        glfwSwapBuffers(m_window);
    }
    Engine::terminate();
}