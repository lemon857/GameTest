#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>


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
	// Установка функции старта
	static void setStartFunc(std::function<void(int, char**)> start);
	// Установка функции апдейта
	static void setUpdateFunc(std::function<void(double)> update);
	// Выбор текущего окна
	static void makeContextCurrent(GLFWwindow* window);
	// Проверка каллбэков
	static void poolEvents();
	// Запуск движка
	static void startEngine(int argc, char** argv);
	// Отчистка всех ресурсов движка 
	static void terminate();
private:
	static std::function<void(int, char**)> m_start;
	static std::function<void(double)> m_update;
	static GLFWwindow* m_window;

	static int defaultStart(int argc, char** argv);

};