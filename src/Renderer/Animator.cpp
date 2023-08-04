#include "Animator.h"

#include "Animation.h"
#include "../Resources/ResourceManager.h"
#include "../Engine/IGameObjec.h"
#include "Sprite.h"

namespace RenderEngine
{
	Animator::Animator(IGameObject& targetObj)
		: m_currentTime(0)
		, m_run(false)
		, IComponent(targetObj)
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
		std::shared_ptr<Animation> anim = getAnimation(m_currentAnimation);
		auto pSprite = m_targetObj.getpSprite();
		pSprite->setSubTexture(anim->subTextureNames[anim->currentFrame]);
	}
	void Animator::stopAnimations()
	{
		m_run = false;
	}
	void Animator::addAnimation(const std::string name, std::shared_ptr<Animation>& animation)
	{
		m_animations.emplace(name, animation);
	}
	void Animator::update(const double delta)
	{
		if (!m_run) return;
		std::shared_ptr<Animation> anim = getAnimation(m_currentAnimation);
		if (m_currentTime >= anim->durations[anim->currentFrame])
		{
			auto pSprite = m_targetObj.getpSprite();
			pSprite->setSubTexture(anim->subTextureNames[anim->currentFrame]);
			m_currentTime = 0;
			if (anim->currentFrame == anim->durations.size() - 1 && anim->eTypeAnimation == Single)
			{
				anim->currentFrame = 0;
				m_run = false;
				return;
			} 
			else if (anim->currentFrame == anim->durations.size() - 1 && anim->eTypeAnimation == Cyclical)
			{
				anim->currentFrame = 0;
				return;
			}
			else
			{
				anim->currentFrame++;
			}
		}
		m_currentTime += delta;
	}
	std::shared_ptr<Animation> Animator::getAnimation(std::string name)
	{
		return m_animations.find(name)->second;
	}
}