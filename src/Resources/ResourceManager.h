#pragma once

#include <string>
#include <memory>
#include <map>

	class ShaderProgram;

	class ResourceManager {
	public:
		ResourceManager(const std::string& executablePath);
		~ResourceManager() = default;

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(const ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&&) = delete;

		std::shared_ptr<ShaderProgram> loadShader(const std::string& shaderNamem, const std::string& vertexPath, const std::string& fragmentPath);
		std::shared_ptr<ShaderProgram> getShader(const std::string& shaderNamem);
	private:
		std::string getFileString(const std::string& relativeFilePath);

		typedef std::map<const std::string, std::shared_ptr<ShaderProgram>> ShaderProgramMap;

		ShaderProgramMap m_ShaderPrograms;
		std::string m_path;
	};
