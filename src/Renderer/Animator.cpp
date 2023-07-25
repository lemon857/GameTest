#include "Animator.h"

#include "Animation.h"
#include "../Resources/ResourceManager.h"
#include "Sprite.h"

namespace RenderEngine
{
	Animator::Animator(std::string spriteName)
		: m_spriteName(spriteName)
		, m_currentTime(0)
		, m_run(false)
	{
	}

	void Animator::startAnimation(std::string name)
	{
		/*if (m_currentAnimation == name)
		{
			switch (getAnimation(m_currentAnimation)->eCallbackAction)
			{
			case Nothing:
				return;
			case StartAtFirst:
				break;
			}
		}
		else
		{*/
			m_currentAnimation = name;
			m_run = true;
		//}
	}
	void Animator::addAnimation(const std::string name, std::shared_ptr<Animation> animation)
	{
		m_animations.emplace(name, animation);
	}
	void Animator::update(double delta)
	{
		if (!m_run) return;
		m_currentTime += delta;
		std::shared_ptr<Animation> anim = getAnimation(m_currentAnimation);
		if (m_currentTime >= anim->durations[anim->currentFrame])
		{
			auto pSprite = ResourceManager::getSprite(m_spriteName);
			pSprite->setSubTexture(anim->subTextureNames[anim->currentFrame]);
			m_currentTime = 0;
			if (anim->currentFrame == anim->durations.size() - 1 && anim->eTypeAnimation == Single)
			{
				anim->currentFrame = 0;
				m_run = false;
				return;
			}
			else
			{
				anim->currentFrame++;
			}
		}
	}
	std::shared_ptr<Animation> Animator::getAnimation(std::string name)
	{
		return m_animations.find(name)->second;
	}
} 