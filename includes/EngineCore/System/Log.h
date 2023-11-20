#pragma once

#ifdef DEBUG
#define DEBUG_CONSOLE
#endif // DEBUG

//#define DEBUG_CONSOLE_OBJ_LOAD

#ifdef DEBUG_CONSOLE
#include <spdlog/spdlog.h>

#define LOG_INFO(...)	spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)	spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...)	spdlog::error(__VA_ARGS__)
#define LOG_CRIT(...)	spdlog::critical(__VA_ARGS__)
#endif // DEBUG_CONSOLE

#ifndef DEBUG_CONSOLE
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_CRIT(...)
#endif // DEBUG_CONSOLE