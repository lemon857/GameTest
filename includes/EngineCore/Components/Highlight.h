#pragma once

#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include "EngineCore/IComponent.h"

namespace RenderEngine
{
	class Line;
	class Material;
}

class Highlight : public IComponent
{
public:
	Highlight(std::shared_ptr<RenderEngine::Material> material, bool isActive = false, bool mode = false, glm::vec3 color = glm::vec3(1.f));

	void update(const double delta) override;

	void set_active(bool isActive);

	void set_mode(bool mode);

	void set_color(glm::vec3 color);

	bool get_active();

	glm::vec3 get_color();

private:
	bool m_isActive;
	bool m_mode;
	std::unique_ptr<RenderEngine::Line> m_line;
	std::shared_ptr<RenderEngine::Material> m_pMaterial;
	glm::vec3 m_color;
};