#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <glm/vec2.hpp>

struct INIdata
{
	glm::ivec2& window_size;
	glm::ivec2& window_position;
	bool& maximized_window;
};

struct Font_Glyph {
	unsigned int TextureID; // ID текстуры глифа
	glm::ivec2   Size;      // Размеры глифа
	glm::ivec2   Bearing;   // Смещение верхней левой точки глифа
	unsigned int Advance;   // Горизонтальное смещение до начала следующего глифа
	~Font_Glyph();
};

class Scene;

struct GraphicsObject;

namespace RenderEngine
{
	class ShaderProgram;
	class ShaderProgramLayout;
	class Texture2D;
	//class SpriteRenderer;
	class Animator;
	class GraphicsObject;
	class Material;
}

	class ResourceManager {
	public:
		static void setExecutablePath(const std::string& executablePath);
		static void unloadAllResources();

		~ResourceManager() = delete;
		ResourceManager() = delete;

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(const ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&&) = delete;

		static bool start_with(std::string& line, const char* text);

		static std::string getFileString(const std::string& relativeFilePath);

		static std::string getExeFilePath() { return m_path; };

		static bool load_JSON_resources(const std::string& JSONrelativePath);

		static bool load_INI_settings(const std::string& INIrelativePath, INIdata& data, const bool isWrite);

		static Font_Glyph get_character(const std::string& fontName, const char sym);

		static bool load_font(std::string relativePath, std::string font_name, unsigned int font_size);

		static bool load_scene(std::string relativePath, Scene& scene);

		static bool save_scene(std::string relativePath, const Scene& scene);

		static std::shared_ptr<GraphicsObject> load_OBJ_file(const std::string& OBJrelativePath, bool is_reload = false);

		static std::shared_ptr<RenderEngine::ShaderProgram> loadShaders(
			const std::string& shaderName, 
			const std::string& vertexPath, 
			const std::string& fragmentPath,
			std::shared_ptr<RenderEngine::ShaderProgramLayout> layout);
		static std::shared_ptr<RenderEngine::ShaderProgram> getShaderProgram(
			const std::string& shaderName);

		static std::shared_ptr<RenderEngine::Texture2D>  loadTexture(
			const std::string& textureName, 
			const std::string& texturePath);
		static std::shared_ptr<RenderEngine::Texture2D> getTexture(const std::string& textureName);

		static std::shared_ptr<RenderEngine::Material>  loadMaterial(
			const std::string& materialName,
			const std::string& textureName,
			const std::string& shaderName);
		static std::shared_ptr<RenderEngine::Material> getMaterial(const std::string& materialName);

		static std::vector<std::string> getNamesShadersProgram();
		static std::vector<std::string> getNamesTextures2D();
		static std::vector<std::string> getNamesMaterials();
		static std::vector<std::string> getNamesObjs();
		static std::vector<std::string> getNamesFilesInDirectory(std::string relativeFilePath);

		static std::string getNameShaderProgram(std::shared_ptr<RenderEngine::ShaderProgram> pShader);
		static std::string getNameTexture2D(std::shared_ptr<RenderEngine::Texture2D> pTexture);

		/*static std::shared_ptr<RenderEngine::SpriteRenderer>  loadSpriteRenderer(
			const std::string& SpriteRendererName, 
			const std::string& textureName,
			const std::string& shaderName,
			const std::string& subTextureName = "default");
		static std::shared_ptr<RenderEngine::SpriteRenderer> getSpriteRenderer(const std::string& SpriteRendererName);

		static std::shared_ptr<RenderEngine::GraphicsObject>  loadGraphicsObject(
			const std::string& objName,
			const std::string& shaderName,
			const std::string& source);
		static std::shared_ptr<RenderEngine::GraphicsObject> getGraphicsObject(const std::string& objName);*/

		/*static std::shared_ptr<RenderEngine::Animator>  loadAnimator(
			const std::string& animatorName,
			const std::string& SpriteRendererName);*/
		//static std::shared_ptr<RenderEngine::Animator> getAnimator(const std::string& animatorName);
		

		static std::shared_ptr<RenderEngine::Texture2D> loadTextureAtlas(
			std::string textureName,
			std::vector<std::string> subTextures,
			std::string texturePath,
			const unsigned int width,
			const unsigned int height,
			const unsigned int subTextureWidth,
			const unsigned int subTextureHeight);
	private:

		typedef std::map<const std::string, std::shared_ptr<RenderEngine::ShaderProgram>> ShaderProgramsMap;
		static ShaderProgramsMap m_ShaderPrograms;

		typedef std::map<const std::string, std::shared_ptr<RenderEngine::Texture2D>> TexturesMap;
		static TexturesMap m_textures;

		typedef std::map<const std::string, std::shared_ptr<RenderEngine::Material>> MaterialsMap;
		static MaterialsMap m_materials;

		typedef std::map<const std::string, std::shared_ptr<GraphicsObject>> CacheOBJMap;
		static CacheOBJMap m_obj_files;

		typedef std::map<const char, Font_Glyph> TTFMap;
		typedef std::map<const std::string, TTFMap> FontsMap;
		static FontsMap m_fonts_map;

		//typedef std::map<const std::string, std::shared_ptr<RenderEngine::SpriteRenderer>> SpriteRenderersMap;
		//static SpriteRenderersMap m_SpriteRenderers;

		//typedef std::map<const std::string, std::shared_ptr<RenderEngine::Animator>> AnimatorsMap;
		//static AnimatorsMap m_animators;

		//typedef std::map<const std::string, std::shared_ptr<RenderEngine::GraphicsObject>> GraphObjMap;
		//static GraphObjMap m_graph_objs;

		static std::string m_path;
	};
