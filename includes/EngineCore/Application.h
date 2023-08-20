#pragma	once

#include "EngineCore/Event.h"
#include "EngineCore/Renderer/Camera.h"
#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"
#include "EngineCore/Renderer/Texture2D.h"
#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Line.h"
#include "EngineCore/Physics/Ray.h"

#include <memory>
#include <glm/vec2.hpp>

class Application
{
public:
	Application();
	virtual  ~Application();

	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	int start(glm::ivec2& window_size, const char* title);

	virtual bool init() { return true; };

	virtual void on_key_update(const double delta);

	virtual void on_mouse_button_event(const MouseButton button, const double pos_x, const double pos_y, const bool isPressed);

	virtual void on_ui_render() {};
protected:
	EventDispatcher m_event_dispather;
	std::unique_ptr<class Window> m_pWindow;
	bool m_pCloseWindow = true;
	bool m_maximized_window = false;
	glm::ivec2& m_window_position = glm::ivec2(100);

	Camera* m_cam;

	Ray* m_ray;

	RenderEngine::Line* m_line;

	float m_colors[4] = { 0.33f, 0.33f, 0.33f, 0.f };
	float m_cam_pos[3] = { -5.f, 0.f, 0.f };
	float m_cam_rot[3] = { 0.f, 0.f, 0.f };

	float m_sprite_pos[3] = { 5.f, -5.f, 0.f };

	float m_line_pos[3] = { 0.f, 0.f, 0.f };

	float m_light_pos[3] = { 12.f, 5.f, 0.f };
	float m_light_color[3] = { 1.f, 1.f, 1.f };

	float m_cube_pos[3] = { 10.f, 0.f, 0.f };
	float m_cube_scale[3] = { 1.f, 1.f, 1.f };
	float m_cube_rot[3] = { 0.f, 0.f, 0.f };

	float m_cam_velocity = 0.01f;
	float m_cam_rotate_velocity = 0.1f;
	float m_cam_sensetivity = 1.f;
	float m_cam_fov = 60.f;
	float m_cam_far_plane = 100.f;

	float m_add_ctrl_speed = 2.f;

	float m_ambient_factor = 0.1f;
	float m_diffuse_factor = 1.f;
	float m_specular_factor = 1.f;
	float m_shininess = 1.f;

	double m_init_mouse_pos_x = 0;
	double m_init_mouse_pos_y = 0;

	int m_mouse_pos_x = 0;
	int m_mouse_pos_y = 0;

	double m_world_mouse_pos_x = 0;
	double m_world_mouse_pos_y = 0;
	double m_world_mouse_pos_z = 0;
	
	bool m_isPerspectiveCam = true;
	bool m_isInversiveMouseY = false;
	int m_isMetalic = 0;
	bool m_drawNullIntersection = false;

	std::shared_ptr<RenderEngine::VertexArray> m_vertexArray;
	std::shared_ptr<RenderEngine::VertexArray> m_vertexArray_light;
	RenderEngine::VertexBuffer m_vertexCoordsBuffer;
	RenderEngine::VertexBuffer m_vertexNormalBuffer;
	RenderEngine::VertexBuffer m_textureCoordsBuffer;
	RenderEngine::IndexBuffer m_indexBuffer;

	std::shared_ptr<RenderEngine::Texture2D> m_pTextureAtlas;
	std::shared_ptr<RenderEngine::ShaderProgram> m_pShaderProgram;
	std::shared_ptr<RenderEngine::ShaderProgram> m_pShaderProgram_light;
};