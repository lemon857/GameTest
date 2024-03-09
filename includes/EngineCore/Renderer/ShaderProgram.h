#pragma once

#include <string>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace RenderEngine
{
	class ShaderProgramLayout;

	class ShaderProgram {
	public:
		// Create shader program
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader, RenderEngine::ShaderProgramLayout* layout);
		~ShaderProgram();
		// Check compile shader program
		bool isCompiled() const;
		// Using shader program
		void use() const;

		void setInt(const std::string& name, const GLint& value);
		void setFloat(const std::string& name, const GLfloat& value);
		void setMatrix4(const std::string& name, const glm::mat4& matrix);
		void setVec4(const std::string& name, const glm::vec4& vec);
		void setVec3(const std::string& name, const glm::vec3& vec);
		void setVec2(const std::string& name, const glm::vec2& vec);

		RenderEngine::ShaderProgramLayout* get_layout();

		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
	private:
		bool createShader(const std::string& sousce, const GLenum shaderType, GLuint& shaderID);
		bool getLocation(const std::string& name, GLuint& location) const;
		void addLocation(const std::string& name, GLuint& location);
		bool m_isCompiled = false;
		GLuint m_ID = 0;
		RenderEngine::ShaderProgramLayout* m_layout;
		typedef std::map<std::string, GLuint> cacheMap;
		cacheMap m_cacheUniformMap;
	};
}