#pragma once

#include <memory>
#include <string>
#include <map>

namespace RenderEngine
{
	class Sprite;
	struct Animation;

	class Animator
	{
	public:
		Animator(std::string spriteName);
			
		void startAnimation(std::string name);
		void addAnimation(const std::string name, std::shared_ptr<Animation> animation);
		void update(double delta);
	private:
		std::shared_ptr<Animation> getAnimation(std::string name);

		std::string m_spriteName;

		std::string m_currentAnimation;

		double m_currentTime;

		bool m_run;

		typedef std::map<const std::string, std::shared_ptr<Animation>> AnimationMap;
		AnimationMap m_animations;
	};
}