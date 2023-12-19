#include "EngineCore/System/INI_loader.h"

void INIdata::add_region(std::string name, BaseINIregion* region)
{
	regions.emplace(name, std::move(region));
}

BaseINIregion* INIdata::get_region(std::string name)
{
	std::map<std::string, BaseINIregion*>::const_iterator it = regions.find(name);
	if (it != regions.end())
	{
		return it->second;
	}
	return nullptr;
}