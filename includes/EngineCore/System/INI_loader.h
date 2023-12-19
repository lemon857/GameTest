#include <map>
#include <vector>
#include <string>

struct BaseINIregion
{
	BaseINIregion()
	{
	}
	virtual void parse(std::string name, std::string value)
	{

	}
	virtual std::string get_str_data()
	{
		return "";
	}
};

struct INIdata
{
	std::map<std::string, BaseINIregion*> regions;
	void add_region(std::string name, BaseINIregion* region);
	BaseINIregion* get_region(std::string name);
};