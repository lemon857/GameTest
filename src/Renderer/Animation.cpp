#include "Animation.h"

namespace RenderEngine
{
	Animation::Animation(std::vector<std::string> vSubTextureNames, std::vector<double> vDurations, ECallbackAction callbackAction, ETypeAnimation typeAnimation)
	{
		currentFrame = 0;
		eCallbackAction = callbackAction;
		eTypeAnimation = typeAnimation;
		subTextureNames = vSubTextureNames;
		durations = vDurations;
	}
}