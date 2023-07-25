#pragma once

#include <memory>

namespace RenderEngine
{
	class Sprite;

	class Anomator
	{
	public:
		Anomator(std::shared_ptr<Sprite> sprite)
			: m_sprite(sprite) {};

	private:
		std::shared_ptr<Sprite> m_sprite;
	};
}