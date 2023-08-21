#pragma once

#include <memory>
#include <string>
#include <map>

#include "EngineCore/IComponent.h"

namespace RenderEngine
{
	class SpriteRenderer;
	struct Animation;

	class Animator : public IComponent
	{
	public:
		Animator(IGameObject& targetObj);

		void startAnimation(std::string name);
		void stopAnimations();
		void addAnimation(const std::string name, std::shared_ptr<Animation>& animation);
		void update(const double delta) override;
	private:
		std::shared_ptr<Animation> getAnimation(std::string name);

		std::string m_currentAnimation;

		double m_currentTime;

		bool m_run;

		typedef std::map<const std::string, std::shared_ptr<Animation>> AnimationMap;
		AnimationMap m_animations;
	};
}