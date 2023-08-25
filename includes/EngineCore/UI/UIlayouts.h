#pragma once

#include <imgui/imgui.h>

#include <functional>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class IUIlayout
{
public:
	IUIlayout()
	{
	};


private:

};

class UIlayoutTransform : IUIlayout
{
public:
	enum ETypeChanegedProp
	{
		position,
		scale,
		rotation
	};

	UIlayoutTransform() {};

	void on_draw_ui();

	void set_on_chanege_callback(std::function<void(float*, ETypeChanegedProp)> on_chanege);

	void set_props(glm::vec3 pos, glm::vec3 scale, glm::vec3 rot);
private:
	std::function<void(float*, ETypeChanegedProp)> m_on_chanege;

	float m_prop_pos[3] = { 0.f, 0.f, 0.f };
	float m_prop_scale[3] = { 1.f, 1.f, 1.f };
	float m_prop_rot[3] = { 0.f, 0.f, 0.f };
};

class UIlayoutHighlight : IUIlayout
{
public:

	UIlayoutHighlight() {};

	void on_draw_ui();

	void set_on_chanege_callback(std::function<void(float*, bool)> on_chanege);

	void set_color(glm::vec3 color);

	void activate() { m_is_active = true; };
private:
	std::function<void(float*, bool)> m_on_chanege;

	float m_color[3] = { 0.f, 0.f, 0.f };
	bool m_is_active = false;
};