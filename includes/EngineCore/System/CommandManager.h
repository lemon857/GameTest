#pragma once

#include <string>
#include <map>
#include <vector>
#include <functional>

typedef std::function<void(std::vector<std::string>)> command_callback;

class CommandManager
{
public:
	static void add_command(std::string name, command_callback callback);
	static bool call_command(std::string name, std::vector<std::string> args);

	~CommandManager() = delete;
	CommandManager() = delete;

	CommandManager(const CommandManager&) = delete;
	CommandManager(const CommandManager&&) = delete;
	CommandManager& operator=(const CommandManager&) = delete;
	CommandManager& operator=(const CommandManager&&) = delete;

private:
	typedef std::map<std::string, command_callback> command_map;

	static command_map m_commands;
};