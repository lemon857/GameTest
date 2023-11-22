#include "EngineCore/System/LogSystem.h"

#include "EngineCore/Resources/ResourceManager.h"

#include <iostream>
#include <Windows.h>
#include <chrono>
#include <ctime>
#include <fstream>

std::ofstream* LogSystem::m_stream;

std::string currentDateTime() {
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);

	auto tim = std::chrono::high_resolution_clock::now();

	auto sec = std::chrono::duration_cast<std::chrono::milliseconds>(tim.time_since_epoch()).count();

	std::string tmp = std::to_string(sec);

	char buffer[128];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now);
	return std::string(buffer) + "." + tmp.substr(tmp.size() - 3);
}

void LogSystem::open_log_file(std::string relPath)
{ 
	m_stream = new std::ofstream();
	m_stream->open(ResourceManager::getExeFilePath() + "/" + relPath);
}

void LogSystem::close_log_file()
{
	m_stream->close();
}

void LogSystem::log_info(std::string msg)
{
#ifdef DEBUG_CONSOLE_LOG
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "[" << currentDateTime() << "] [";
	SetConsoleTextAttribute(h, (((0 << 4) | 2)));
	std::cout << "info";
	SetConsoleTextAttribute(h, (((0 << 4) | 7)));
	std::cout << "] " << msg << std::endl;
#endif
	if (m_stream->is_open()) *m_stream << "[" << currentDateTime() << "] [info] " << msg << std::endl;
}

void LogSystem::log_warn(std::string msg)
{
#ifdef DEBUG_CONSOLE_LOG
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "[" << currentDateTime() << "] [";
	SetConsoleTextAttribute(h, (((0 << 4) | 14)));
	std::cout << "warn";
	SetConsoleTextAttribute(h, (((0 << 4) | 7)));
	std::cout << "] " << msg << std::endl;
#endif
	if (m_stream->is_open()) *m_stream << "[" << currentDateTime() << "] [warn] " << msg << std::endl;
}

void LogSystem::log_error(std::string msg)
{
#ifdef DEBUG_CONSOLE_LOG
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "[" << currentDateTime() << "] [";
	SetConsoleTextAttribute(h, (((0 << 4) | 4)));
	std::cout << "error";
	SetConsoleTextAttribute(h, (((0 << 4) | 7)));
	std::cout << "] " << msg << std::endl;
#endif
	if (m_stream->is_open()) *m_stream << "[" << currentDateTime() << "] [error] " << msg << std::endl;
}

void LogSystem::log_crit(std::string msg)
{
#ifdef DEBUG_CONSOLE_LOG
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "[" << currentDateTime() << "] [";
	SetConsoleTextAttribute(h, (((4 << 4) | 15)));
	std::cout << "critical";
	SetConsoleTextAttribute(h, (((0 << 4) | 7)));
	std::cout << "] " << msg << std::endl;
#endif
	if (m_stream->is_open()) *m_stream << "[" << currentDateTime() << "] [critical] " << msg << std::endl;
}

std::string std::to_string(string str)
{
	return str;
}
