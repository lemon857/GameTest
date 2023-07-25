#include "Engine.h"
#include "../Resources/ResourceManager.h"

#include <iostream>

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