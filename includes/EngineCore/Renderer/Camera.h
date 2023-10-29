#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <memory>
#include <vector>

class Camera
{
public:
	enum class ProjectionMode
	{
		Orthographic,
		Perspective
	};
	Camera(const glm::vec3& position = { 0, 0, 0 },
		const glm::vec3& rotation = { 0, 0, 0 },
		const ProjectionMode mode = ProjectionMode::Perspective);

	void set_position(const glm::vec3& position);
	void set_rotation(const glm::vec3& rotation);
	void set_position_rotation(const glm::vec3& position, const glm::vec3& rotation);
	void set_projection_mode(const ProjectionMode mode);

	void set_far_clip_plane(const float far);
	void set_near_clip_plane(const float near);
	void set_viewport_size(const float width, const float height);
	void set_field_of_view(const float fov);
	void set_size_of_view(const float sov);

	glm::mat4 get_view_matrix();
	glm::mat4 get_projection_matrix();
	glm::mat4 get_ui_matrix();

	glm::vec3 get_position() const { return m_position; };
	glm::vec3 get_rotation() const { return m_rotation; };
	glm::vec2 get_viewport_size() const { return glm::vec2(m_viewport_width, m_viewport_height); };
	glm::vec3 get_world_mouse_position(glm::vec2 mouse_position, glm::vec2 window_size);

	void move_forward(const float delta);
	void move_right(const float delta);
	void move_up(const float delta);
	/// <summary>
	/// Move and rotate camera
	/// </summary>
	/// <param name="movement_delta">: X - right, Y - yp, Z - forward</param>
	/// <param name="rotation_delta">: X - roll, Y - yaw, Z - roll</param>
	void add_movement_and_rotation(const glm::vec3& movement_delta, const glm::vec3& rotation_delta);

private:
	void update_veiw_matrix();
	void update_projection_matrix();
	void update_ui_matrix();

	glm::vec3 m_position;
	glm::vec3 m_rotation;	

	glm::vec3 m_direction;
	glm::vec3 m_right;
	glm::vec3 m_up;

	float m_far_clip_plane{ 100.f };
	float m_near_clip_plane{ 0.1f };
	float m_viewport_width{ 800.1f };
	float m_viewport_height{ 600.1f };
	float m_field_of_view{ 60.f };
	float m_size_of_view{ 1.f };

	static constexpr glm::vec3 s_world_up{ 0.f, 1.f, 0.f };
	static constexpr glm::vec3 s_world_rigt{ -1.f, 0.f, 0.f };
	static constexpr glm::vec3 s_world_forward{ 0.f, 0.f, 1.f };

	glm::mat4 m_veiw_matrix;
	glm::mat4 m_projection_matrix;
	glm::mat4 m_ui_matrix;

	ProjectionMode m_projection_mode;

	bool m_update_view_matrix = false;
	bool m_update_projection_matrix = false;
	bool m_update_ui_matrix = true;
};