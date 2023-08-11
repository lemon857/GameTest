#pragma once

#include <glm/vec3.hpp>
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

	glm::mat4 get_view_matrix() const { return m_veiw_matrix; };
	glm::mat4 get_projection_matrix() const { return m_projection_matrix; };

	void move_forward(const float delta);
	void move_right(const float delta);
	void move_up(const float delta);
	/// <summary>
	/// Move and rotate camera
	/// </summary>
	/// <param name="movement_delta">: X - forward, Y - right, Z - up</param>
	/// <param name="rotation_delta">: X - roll, Y - pitch, Z - yaw</param>
	void add_movement_and_rotate(const glm::vec3& movement_delta, const glm::vec3& rotation_delta);

private:
	void update_veiw_matrix();
	void update_projection_matrix();

	glm::vec3 m_position;
	glm::vec3 m_rotation;	

	glm::vec3 m_direction;
	glm::vec3 m_right;
	glm::vec3 m_up;

	glm::mat4 m_veiw_matrix;
	glm::mat4 m_projection_matrix;

	static constexpr glm::vec3 s_world_up{ 0.f, 0.f, 1.f };
	static constexpr glm::vec3 s_world_rigt{ 0.f, -1.f, 0.f };
	static constexpr glm::vec3 s_world_forward{ 1.f, 0.f, 0.f };

	ProjectionMode m_projection_mode;
};