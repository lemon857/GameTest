#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

namespace RenderEngine
{
	class ShaderProgram {
	public:
		// Создание шейдерной программы из двух, вертексной и фрагментной
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();
		// Проверка на скомпилированность шейдера 
		bool isCompiled() const;
		// Использование шейдера
		void use() const;
		void setInt(const std::string& name, const GLint& value) const;
		void setFloat(const std::string& name, const GLfloat& value) const;
		void setMatrix4(const std::string& name, const glm::mat4& matrix) const;
		void setVec4(const std::string& name, const glm::vec4& vec) const;

		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
	private:
		bool createShader(const std::string& sousce, const GLenum shaderType, GLuint& shaderID);
		bool m_isCompiled = false;
		GLuint m_ID = 0;
	};
}