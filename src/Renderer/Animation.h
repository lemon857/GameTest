#pragma once

#include <memory>
#include <string>
#include <vector>

namespace RenderEngine
{
	class Sprite;

	enum ECallbackAction
	{
		Nothing = 0,
		StartAtFirst = 1
	};

	enum ETypeAnimation
	{
		Single = 0,
		Cyclical = 1
	};

	struct Animation
	{
		Animation(std::vector<std::string> vSubTextureNames, std::vector<double> vDurations,
			ECallbackAction callbackAction, ETypeAnimation typeAnimation);

		std::vector<std::string> subTextureNames;
		std::vector<double> durations;

		ECallbackAction eCallbackAction;
		ETypeAnimation eTypeAnimation;

		size_t currentFrame;
	}; 
}