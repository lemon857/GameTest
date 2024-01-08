#include "EngineCore/Resources/LanguagePack.h"

#include <fstream>
#include <locale>
#include <codecvt>

LanguagePack::LanguagePack()
{
}

LanguagePack::~LanguagePack()
{
}

bool LanguagePack::load_pack(std::string path)
{
	std::wifstream f;
	f.open(path, std::ios::in);

	if (f.is_open())
	{
		std::wstring line;
		while (std::getline(f, line))
		{
			std::string name = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(line.substr(0, line.find('=')));
			std::wstring value = line.substr(line.find('=') + 1);
			add_field(name, value);
		}
	}
	return true;
}

std::wstring LanguagePack::operator[](std::string name)
{
	std::map<std::string, std::wstring>::const_iterator it = m_data.find(name);
	if (it != m_data.end())
	{
		return it->second;
	}
	return L"";
}

void LanguagePack::add_field(std::string name, std::wstring value)
{
	m_data.emplace(name, value);
}

std::wstring LanguagePack::get_field(std::string name)
{
	std::map<std::string, std::wstring>::const_iterator it = m_data.find(name);
	if (it != m_data.end())
	{
		return it->second;
	}
	return L"";
}
