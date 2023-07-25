#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Engine
{
public:
	// Инициализация графики
	// Вернёт: 
	// false при неудачной инициалиации
	// true при удачной инициализации
	static bool initGraphics();
	// Инициализация OpenGL
	// Вернёт: 
	// false при неудачной инициалиации
	// true при удачной инициализации
	static bool initOpenGL();
	// Установка калбэков для отслеживания изменения размеров окна
	static void setWindowSizeCallBack(GLFWwindow* window, GLFWwindowsizefun callback);
	// Установка каллбэков для отслеживания нажатия клавиш
	static void setKeyCallback(GLFWwindow* window, GLFWkeyfun callback);
	// Выбор текущего окна
	static void makeContextCurrent(GLFWwindow* window);
	// Проверка каллбэков
	static void poolEvents();
	// Отчистка всех ресурсов движка 
	static void terminate();
private:

};