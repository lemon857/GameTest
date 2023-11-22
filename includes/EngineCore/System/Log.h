#pragma once

//#define DEBUG_CONSOLE_OBJ_LOAD

#include "EngineCore/System/LogSystem.h"

#define LOG_INFO(...)	LogSystem::info(__VA_ARGS__)
#define LOG_WARN(...)	LogSystem::warn(__VA_ARGS__)
#define LOG_ERROR(...)	LogSystem::error(__VA_ARGS__)
#define LOG_CRIT(...)	LogSystem::critical(__VA_ARGS__)

#ifndef DEBUG_CONSOLE 
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // DEBUG_CONSOLE