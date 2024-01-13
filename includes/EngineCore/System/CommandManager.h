#pragma once

#include <string>
#include <map>
#include <vector>
#include <functional>

typedef std::function<void(std::vector<std::string>)> command_callback;

class CommandManager
{
public:
	static void add_command(std::string name, std::string description, command_callback callback);
	static bool call_command(std::string name, std::vector<std::string> args);
	static std::string get_description_command(std::string name);

	static std::vector<std::string> get_commands();

	~CommandManager() = delete;
	CommandManager() = delete;

	CommandManager(const CommandManager&) = delete;
	CommandManager(const CommandManager&&) = delete;
	CommandManager& operator=(const CommandManager&) = delete;
	CommandManager& operator=(const CommandManager&&) = delete;

private:
	struct CommandHead
	{
		command_callback callback;
		std::string description;
	};

	typedef std::map<std::string, CommandHead> command_map;

	static command_map m_commands;
};