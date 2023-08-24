#pragma once

#include <string>
#include <memory>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace RenderEngine
{
	class ShaderProgramLayout;

	class ShaderProgram {
	public:
		// Создание шейдерной программы из двух, вертексной и фрагментной
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader, std::shared_ptr<RenderEngine::ShaderProgramLayout> layout);
		~ShaderProgram();
		// Проверка на скомпилированность шейдера 
		bool isCompiled() const;
		// Использование шейдера
		void use() const;
		void setInt(const std::string& name, const GLint& value);
		void setFloat(const std::string& name, const GLfloat& value);
		void setMatrix4(const std::string& name, const glm::mat4& matrix);
		void setVec4(const std::string& name, const glm::vec4& vec);
		void setVec3(const std::string& name, const glm::vec3& vec);

		std::shared_ptr<RenderEngine::ShaderProgramLayout> get_layout();

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
		std::shared_ptr<RenderEngine::ShaderProgramLayout> m_layout;
		typedef std::map<std::string, GLuint> cacheMap;
		cacheMap m_cacheUniformMap;
	};
}