#pragma once

#include <imgui/imgui.h>

#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <string>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace RenderEngine
{
	class ShaderProgramLayout;
	class Material;
}

class IUIlayout
{
public:
	IUIlayout(){};
	virtual void on_draw_ui(){};

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

	void on_draw_ui() override;

	void set_callback(std::function<void(const float*, ETypeChanegedProp)> on_chanege);

	void set_props(glm::vec3 pos, glm::vec3 scale, glm::vec3 rot);
private:
	std::function<void(const float*, ETypeChanegedProp)> m_on_chanege;

	float m_prop_pos[3] = { 0.f, 0.f, 0.f };
	float m_prop_scale[3] = { 1.f, 1.f, 1.f };
	float m_prop_rot[3] = { 0.f, 0.f, 0.f };
};

class UIlayoutHighlight : IUIlayout
{
public:

	UIlayoutHighlight() {};

	void on_draw_ui() override;

	void set_callback(std::function<void(const float*, bool)> on_chanege);

	void set_color(glm::vec3 color);

	void activate() { m_is_active = true; };
private:
	std::function<void(const float*, bool)> m_on_chanege;

	float m_color[3] = { 0.f, 0.f, 0.f };
	bool m_is_active = false;
};

class UIlayoutShaderProgram : IUIlayout
{
public:
	enum EUITypeData
	{
		Int = 0,
		Float,
		Vec3,
		Vec4,
		Col3,
		Col4,
	};
	UIlayoutShaderProgram();

	void on_draw_ui() override;

	void set_shader_layout(std::shared_ptr<RenderEngine::ShaderProgramLayout> shaderLayout);

	void set_callback(std::function<void(std::string nameProp, const void* data, EUITypeData type)> on_chanege);
private:
	float* get_data(std::string name);

	std::shared_ptr<RenderEngine::ShaderProgramLayout> m_shaderLayout;
	std::function<void(std::string nameProp, const void* data, EUITypeData type)> m_on_chanege;
	
	std::map<std::string, float*> m_data_map;
};

class UIlayoutMaterial : IUIlayout
{
public:
	UIlayoutMaterial();

	void on_draw_ui() override;

	void set_callback(std::function<void(const std::string nameTexture, const std::string nameShaderProgram)> on_chanege);

	void set_material(std::shared_ptr<RenderEngine::Material> pMaterial);
private:
	std::function<void(const std::string nameTexture, const std::string nameShaderProgram)> m_on_chanege;
	std::string m_nameTexture;
	std::string m_nameShaderProgram;
	int item_current_shader = 0;
	int item_current_texture = 0;
};

class UIlayoutMeshRenderer : IUIlayout
{
public:
	UIlayoutMeshRenderer();

	void on_draw_ui() override;

	void set_callback(std::function<void(const std::string nameMaterial)> on_chanege);

	void set_prop(std::shared_ptr<UIlayoutMaterial> materialUI, std::shared_ptr<UIlayoutShaderProgram> shaderUI);

private:
	std::function<void(const std::string nameMaterial)> m_on_chanege;

	std::shared_ptr<UIlayoutMaterial> m_materialUI;
	std::shared_ptr<UIlayoutShaderProgram> m_shaderUI;
};
