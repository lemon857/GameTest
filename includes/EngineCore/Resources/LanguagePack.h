#pragma once

#include <string>
#include <map>
#include <memory>

class LanguagePack
{
public:
	LanguagePack();
	~LanguagePack();

	bool load_pack(std::string path);

	std::wstring operator[](std::string name);

	std::wstring get(std::string name);
private:
	void add_field(std::string name, std::wstring value);

	std::map<std::string, std::wstring> m_data;
};