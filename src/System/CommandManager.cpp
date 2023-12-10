#include "EngineCore/System/CommandManager.h"

CommandManager::command_map CommandManager::m_commands;

void CommandManager::add_command(std::string name, command_callback callback)
{
	m_commands.emplace(name, callback);
}

bool CommandManager::call_command(std::string name, std::vector<std::string> args)
{
	command_map::const_iterator it = m_commands.find(name);
	if (it != m_commands.end())
	{
		it->second(args);
		return true;
	}
	return false;
}
