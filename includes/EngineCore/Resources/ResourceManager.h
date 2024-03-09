#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

#include <glm/vec2.hpp>

class Scene;
class Sound;
class LanguagePack;

struct INIdata;
struct GraphicsObject;

namespace GUI
{
	class Font;
}

namespace RenderEngine
{
	class ShaderProgram;
	class ShaderProgramLayout;
	class Texture2D;
	class Texture3D;
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

		static std::string getFileString(const std::string& relativeFilePath);

		static std::string getExeFilePath() { return m_path; };

		static bool load_JSON_resources(const std::string& JSONrelativePath);

		static bool load_JSON_models(const std::string& JSONpath);

		static bool load_INI_settings(const std::string& INIrelativePath, INIdata& data, const bool isWrite);

		static bool load_scene(std::string relativePath, Scene& scene);

		static bool save_scene(std::string relativePath, const Scene& scene);

		static GraphicsObject* loadGraphicsObject(const std::string& name, const std::string& relativePath, const std::string& type);
		static GraphicsObject* getGraphicsObject(const std::string& name);

		static LanguagePack* load_lang_pack(std::string relativePath, std::string pack_name);
		static LanguagePack* get_lang_pack(std::string pack_name);

		static GUI::Font* load_font(std::string relativePath, std::string font_name, unsigned int font_size);
		static GUI::Font* get_font(std::string font_name);

		static Sound* load_sound(std::string relativePath, std::string name);
		static Sound* get_sound(std::string name);

		static void load_unique_sound(std::string relativePath, std::string name);
		static std::unique_ptr<Sound> get_unique_sound(std::string name);

		static RenderEngine::ShaderProgram* loadShaders(
			const std::string& shaderName, 
			const std::string& vertexPath, 
			const std::string& fragmentPath,
			const std::string& geometryPath,
			RenderEngine::ShaderProgramLayout* layout);
		static RenderEngine::ShaderProgram* getShaderProgram(
			const std::string& shaderName);

		static RenderEngine::Texture2D*  loadTexture(
			const std::string& textureName, 
			const std::string& texturePath);
		static RenderEngine::Texture2D* getTexture(const std::string& textureName);

		static RenderEngine::Texture3D*  loadTexture3D(
			const std::string& textureName,
			const std::vector<std::string>& texturePath);
		static RenderEngine::Texture3D* getTexture3D(const std::string& textureName);

		static RenderEngine::Material* loadMaterial(
			const std::string& materialName,
			const std::string& textureName,
			const std::string& shaderName);
		static RenderEngine::Material* getMaterial(const std::string& materialName);

		static std::vector<std::string> getNamesShadersProgram();
		static std::vector<std::string> getNamesTextures2D();
		static std::vector<std::string> getNamesMaterials();
		static std::vector<std::string> getNamesObjs();
		static std::vector<std::string> getNamesFilesInDirectory(std::string relativeFilePath);

		static std::string getNameShaderProgram(RenderEngine::ShaderProgram* pShader);
		static std::string getNameTexture2D(RenderEngine::Texture2D* pTexture);

		static LanguagePack* get_current_lang_pack();
		static void set_current_lang_pack(LanguagePack* lang_pack);

		/*static RenderEngine::SpriteRenderer>  loadSpriteRenderer(
			const std::string& SpriteRendererName, 
			const std::string& textureName,
			const std::string& shaderName,
			const std::string& subTextureName = "default");
		static RenderEngine::SpriteRenderer> getSpriteRenderer(const std::string& SpriteRendererName);

		static RenderEngine::GraphicsObject>  loadGraphicsObject(
			const std::string& objName,
			const std::string& shaderName,
			const std::string& source);
		static RenderEngine::GraphicsObject> getGraphicsObject(const std::string& objName);*/

		/*static RenderEngine::Animator>  loadAnimator(
			const std::string& animatorName,
			const std::string& SpriteRendererName);*/
		//static RenderEngine::Animator> getAnimator(const std::string& animatorName);
		

		static RenderEngine::Texture2D* loadTextureAtlas(
			std::string textureName,
			std::vector<std::string> subTextures,
			std::string texturePath,
			const unsigned int width,
			const unsigned int height,
			const unsigned int subTextureWidth,
			const unsigned int subTextureHeight);
	private:

		typedef std::map<const std::string, RenderEngine::ShaderProgram*> ShaderProgramsMap;
		static ShaderProgramsMap m_ShaderPrograms;

		typedef std::map<const std::string, RenderEngine::Texture2D*> TexturesMap;
		static TexturesMap m_textures;

		typedef std::map<const std::string, RenderEngine::Texture3D*> CubeTexturesMap;
		static CubeTexturesMap m_cube_textures;

		typedef std::map<const std::string, RenderEngine::Material*> MaterialsMap;
		static MaterialsMap m_materials;

		typedef std::map<const std::string, GraphicsObject*> GrapicsObjectsMap;
		static GrapicsObjectsMap m_graphics_objects;

		typedef std::map<const std::string, GUI::Font*> FontsMap;
		static FontsMap m_fonts_map;

		typedef std::map<const std::string, Sound*> SoundsMap;
		static SoundsMap m_sounds_map;

		typedef std::map<const std::string, std::string> UniqueSoundsMap;
		static UniqueSoundsMap m_uSounds_map;

		typedef std::map<const std::string, LanguagePack*> LangPacksMap;
		static LangPacksMap m_lang_packs_map;
		//typedef std::map<const std::string, RenderEngine::SpriteRenderer>> SpriteRenderersMap;
		//static SpriteRenderersMap m_SpriteRenderers;

		//typedef std::map<const std::string, RenderEngine::Animator>> AnimatorsMap;
		//static AnimatorsMap m_animators;

		//typedef std::map<const std::string, RenderEngine::GraphicsObject>> GraphObjMap;
		//static GraphObjMap m_graph_objs;

		static std::string m_path;

		static LanguagePack* m_current_lang_pack;
	};
