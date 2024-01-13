#include "EngineCore/System/CommandManager.h"

CommandManager::command_map CommandManager::m_commands;

void CommandManager::add_command(std::string name, std::string description, command_callback callback)
{
	CommandHead com;
	com.callback = callback;
	com.description = description;
	m_commands.emplace(name, com);
}

bool CommandManager::call_command(std::string name, std::vector<std::string> args)
{
	command_map::const_iterator it = m_commands.find(name);
	if (it != m_commands.end())
	{
		it->second.callback(args);
		return true;
	}
	return false;
}
std::string CommandManager::get_description_command(std::string name)
{
	command_map::const_iterator it = m_commands.find(name);
	if (it != m_commands.end())
	{
		return it->second.description;
	}
	return "";
}

std::vector<std::string> CommandManager::get_commands()
{
	std::vector<std::string> commands;

	for (const auto cur : m_commands)
	{
		commands.push_back(cur.first);
	}

	return commands;
}
