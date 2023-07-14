#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include "IGameObjec.h"

namespace RenderEngine
{
	class Sprite;
}

class Tank : public IGameObject
{
public:
	enum class EOrentation
	{
		Top,
		Bottom,
		Left,
		Right
	};

	Tank(std::shared_ptr<RenderEngine::Sprite> pSprite, double velocity, double weight, glm::vec2& position, glm::vec2& size);

	void render() const;
	void setOrentation(const EOrentation orentation);
	void move(const bool move);
	void update(const double delta) override;

private:
	EOrentation m_eOrentation;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite;
};