#pragma once

#include <string>

// using HKEY_CURRENT_USER
class RegistryManager
{
public:
	~RegistryManager() = delete;
	RegistryManager() = delete;

	RegistryManager(const RegistryManager&) = delete;
	RegistryManager(const RegistryManager&&) = delete;
	RegistryManager& operator=(const RegistryManager&) = delete;
	RegistryManager& operator=(const RegistryManager&&) = delete;

	static void set_reg_path(std::string path);
	// return 0 if all OK
	static int set_value(std::string name_value, std::string value);

	static std::string get_value(std::string name_value);

private:
	static std::string m_path;
};