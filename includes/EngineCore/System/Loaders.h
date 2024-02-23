#pragma once

#include <string>
#include <map>
#include <vector>

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

struct GraphicsObject;

namespace loaders
{
	enum class ErrorCode
	{
		null = 0,
		errorFileOpen,
		errorFileRead,
		errorInitGraphObj
	};

	GraphicsObject* load_obj_file(const std::string& OBJPath);

	ErrorCode load_ini(const std::string& INIpath, INIdata& data, const bool isWrite);

	// support .jpg .png
	unsigned char* load_image(const char* fullpath, int* width, int* height, int* channels, bool flip = true);

	int save_image_png(unsigned char* pixels, const char* fullpath, int width, int height, int channels);

	void clear_image(unsigned char* image_data);
}