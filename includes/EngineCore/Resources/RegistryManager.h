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
	// for write binary
	static int set_value(char* name_value, char* value, int valueLen);
	// return true if all OK
	// for get binary
	static bool get_value(char* name_value, char* valueBuf, int bufLen);

	// return 0 if all OK
	// for write string
	static int set_value(std::string name_value, std::string value);
	// return true if all OK
	// for get string
	static bool get_value(std::string name_value, std::string& valueBuf);


private:
	static std::string m_path;
};