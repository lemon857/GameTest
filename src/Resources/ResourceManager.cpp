#include "EngineCore/Resources/ResourceManager.h";
#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Texture2D.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"
#include "EngineCore/Renderer/ShaderProgramLayout.h"
#include "EngineCore/System/Log.h"
#include "EngineCore/Resources/Scene.h"
#include "EngineCore/Resources/LanguagePack.h"
#include "EngineCore/GUI/Font.h"
#include "EngineCore/System/Loaders.h"
#include "EngineCore/Sound/Sound.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

ResourceManager::ShaderProgramsMap ResourceManager::m_ShaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::MaterialsMap ResourceManager::m_materials;
ResourceManager::GrapicsObjectsMap ResourceManager::m_graphics_objects;
ResourceManager::FontsMap ResourceManager::m_fonts_map;
ResourceManager::SoundsMap ResourceManager::m_sounds_map;
ResourceManager::UniqueSoundsMap ResourceManager::m_uSounds_map;
ResourceManager::LangPacksMap ResourceManager::m_lang_packs_map;
std::shared_ptr<LanguagePack> ResourceManager::m_current_lang_pack;
//ResourceManager::SpriteRenderersMap ResourceManager::m_SpriteRenderers;
//ResourceManager::GraphObjMap ResourceManager::m_graph_objs;
//ResourceManager::AnimatorsMap ResourceManager::m_animators;
std::string ResourceManager::m_path;

void ResourceManager::unloadAllResources()
{
	m_ShaderPrograms.clear();
	m_textures.clear();
	m_materials.clear();
	m_graphics_objects.clear();
	m_fonts_map.clear();
	m_sounds_map.clear();
	m_uSounds_map.clear();
	//m_SpriteRenderers.clear();
}
void ResourceManager::setExecutablePath(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}
bool ResourceManager::load_JSON_resources(const std::string & JSONpath)
{
	const std::string JSONstr = getFileString(JSONpath);
	if (JSONstr.empty())
	{
		LOG_CRIT("No JSON file: {0}", JSONpath);
		return false;
	}
	rapidjson::Document doc;
	rapidjson::ParseResult result = doc.Parse(JSONstr.c_str());
	if (!result)
	{
		LOG_CRIT("JSON parse error: {0} ({1})", rapidjson::GetParseError_En(result.Code()),  result.Offset());
		LOG_CRIT("In JSON file: ", JSONpath);
		return false;
	}

	auto shadersIt = doc.FindMember("shaders");
	if (shadersIt != doc.MemberEnd())
	{
		for (const auto& currentShader : shadersIt->value.GetArray())
		{
			const std::string name = currentShader["name"].GetString();
			const std::string vertShader = currentShader["filePath_v"].GetString();
			const std::string fragShader = currentShader["filePath_f"].GetString();
			const std::string geomShader = (currentShader.HasMember("filePath_g") ? currentShader["filePath_g"].GetString() : "");
			const auto layoutArray = currentShader["layout"].GetArray();

			std::shared_ptr<RenderEngine::ShaderProgramLayout> layout = std::make_shared<RenderEngine::ShaderProgramLayout>();
						
			for (const auto& currentLayoutElement : layoutArray)
			{
				const std::string name = currentLayoutElement["name"].GetString();
				const int type = currentLayoutElement["typeInt"].GetInt();
				const double min = currentLayoutElement["minVal"].GetDouble();
				const double max = currentLayoutElement["maxVal"].GetDouble();
				layout->addElementLayout(name, (ETypeData)type, min, max);
			}

			loadShaders(name, vertShader, fragShader, geomShader, layout);
		}
	}
	auto atlasesIt = doc.FindMember("textureAtlases");
	if (atlasesIt != doc.MemberEnd())
	{
		for (const auto& currentAtlas : atlasesIt->value.GetArray())
		{
			const std::string name = currentAtlas["name"].GetString();
			const std::string path = currentAtlas["filePath"].GetString();
			const unsigned int width = currentAtlas["width"].GetUint();
			const unsigned int height = currentAtlas["height"].GetUint();
			const unsigned int subTextureWidth = currentAtlas["subTexture_w"].GetUint();
			const unsigned int subTextureHeight = currentAtlas["subTexture_h"].GetUint();

			const auto subTexturesArray = currentAtlas["subTextures"].GetArray();
			std::vector<std::string> subTextures;
			subTextures.reserve(subTexturesArray.Size());

			for (const auto& currentSubTexture : subTexturesArray)
			{
				subTextures.emplace_back(currentSubTexture.GetString());
			}

			loadTextureAtlas(name, subTextures, path, width, height, subTextureWidth, subTextureHeight);
		}
	}
	auto materialsIt = doc.FindMember("materials");
	if (materialsIt != doc.MemberEnd())
	{
		for (const auto& currentMaterial : materialsIt->value.GetArray())
		{
			const std::string name = currentMaterial["name"].GetString();
			const std::string shaderName = currentMaterial["nameShader"].GetString();
			const std::string textureName = currentMaterial["nameTexture"].GetString();

			loadMaterial(name, textureName, shaderName);
		}
	}
	auto fontsIt = doc.FindMember("fonts");
	if (fontsIt != doc.MemberEnd())
	{
		for (const auto& currentFont : fontsIt->value.GetArray())
		{
			const std::string name = currentFont["name"].GetString();
			const std::string path = currentFont["path"].GetString();
			const unsigned int size = currentFont["size"].GetUint();
			load_font(path, name, size);
		}
	}
	auto soundsIt = doc.FindMember("sounds");
	if (soundsIt != doc.MemberEnd())
	{
		for (const auto& currentSound : soundsIt->value.GetArray())
		{
			const std::string name = currentSound["name"].GetString();
			const std::string path = currentSound["path"].GetString();
			load_sound(path, name);
			load_unique_sound(path, name);
		}
	}
	auto modelsIt = doc.FindMember("models");
	if (modelsIt != doc.MemberEnd())
	{
		for (const auto& currentModel: modelsIt->value.GetArray())
		{
			const std::string name = currentModel["name"].GetString();
			const std::string path = currentModel["path"].GetString();
			const std::string type = currentModel["type"].GetString();
			while (loadGraphicsObject(name, path, type) == nullptr)
			{
				//LOG_WARN("Failed load OBJ model");
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
		}
	}
	auto langIt = doc.FindMember("languages");
	if (langIt != doc.MemberEnd())
	{
		for (const auto& currentLang : langIt->value.GetArray())
		{
			const std::string name = currentLang["name"].GetString();
			const std::string path = currentLang["path"].GetString();
			load_lang_pack(path, name);
		}
	}
	LOG_INFO("Loadind data from JSON file complete");
	return true;
}
bool ResourceManager::load_JSON_models(const std::string& JSONpath)
{
	const std::string JSONstr = getFileString(JSONpath);
	if (JSONstr.empty())
	{
		LOG_CRIT("No JSON file: {0}", JSONpath);
		return false;
	}
	rapidjson::Document doc;
	rapidjson::ParseResult result = doc.Parse(JSONstr.c_str());
	if (!result)
	{
		LOG_CRIT("JSON parse error: {0} ({1})", rapidjson::GetParseError_En(result.Code()), result.Offset());
		LOG_CRIT("In JSON file: ", JSONpath);
		return false;
	}

	if (m_graphics_objects.empty()) m_graphics_objects.clear();

	auto modelsIt = doc.FindMember("models");
	if (modelsIt != doc.MemberEnd())
	{
		for (const auto& currentModel : modelsIt->value.GetArray())
		{
			const std::string name = currentModel["name"].GetString();
			const std::string path = currentModel["path"].GetString();
			const std::string type = currentModel["type"].GetString();
			while (loadGraphicsObject(name, path, type) == nullptr)
			{
				LOG_WARN("Failed load OBJ model");
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
		}
	}
	
	LOG_INFO("Reloadind models from JSON file complete");
	return true;
}
bool ResourceManager::load_INI_settings(const std::string& INIpath, INIdata& data, const bool isWrite)
{
	loaders::ErrorCode a = loaders::load_ini(m_path + "/" + INIpath, data, isWrite);
	if (a != loaders::ErrorCode::null)
	{
		LOG_ERROR("Error open file: {0}", INIpath);
		return false;
	}
	return true;
}
std::shared_ptr<GUI::Font> ResourceManager::load_font(std::string relativePath, std::string font_name, unsigned int font_size)
{
	std::shared_ptr<GUI::Font> font = std::make_shared<GUI::Font>();

	if (font->load(m_path + "/" + relativePath, font_size))
	{
		m_fonts_map.emplace(font_name, font);

		LOG_INFO("Success load font: {0}", font_name);

		return font;
	}

	LOG_ERROR("Error load font: {0}", font_name);

	return nullptr;
}
std::shared_ptr<GUI::Font> ResourceManager::get_font(std::string font_name)
{
	FontsMap::const_iterator it = m_fonts_map.find(font_name);
	if (it != m_fonts_map.end())
	{
		return it->second;
	}
	LOG_ERROR("Can't find font: {0}", font_name);
	return nullptr;
}
std::shared_ptr<Sound> ResourceManager::load_sound(std::string relativePath, std::string name)
{
	std::shared_ptr<Sound> sound = std::make_shared<Sound>();
	if (sound->init((m_path + "/" + relativePath).c_str()) != 0)
	{
		LOG_ERROR("Error load sound: {0}", name);
		return nullptr;
	}
	m_sounds_map.emplace(name, sound);
	return sound;
}
std::shared_ptr<Sound> ResourceManager::get_sound(std::string name)
{
	SoundsMap::const_iterator it = m_sounds_map.find(name);
	if (it != m_sounds_map.end())
	{
		return it->second;
	}
	LOG_ERROR("Can't find sound: {0}", name);
	return nullptr;
}
void ResourceManager::load_unique_sound(std::string relativePath, std::string name)
{
	m_uSounds_map.emplace(name, m_path + "/" + relativePath);
}
std::unique_ptr<Sound> ResourceManager::get_unique_sound(std::string name)
{
	UniqueSoundsMap::const_iterator it = m_uSounds_map.find(name);
	if (it != m_uSounds_map.end())
	{
		std::unique_ptr<Sound> sound = std::make_unique<Sound>();
		if (sound->init(it->second.c_str()) != 0)
		{
			LOG_ERROR("Error load unique sound: {0}", name);
			return nullptr;
		}
		return std::move(sound);
	}
	LOG_ERROR("Can't find unique sound: {0}", name);
	return nullptr;
}
bool ResourceManager::load_scene(std::string relativePath, Scene& scene)
{
	std::ifstream fin;

	linked_list<ObjectsPart*> objs;

	fin.open(m_path + "/" + relativePath, std::ifstream::app);
	if (!fin.is_open())
	{
		LOG_ERROR("File {0} doesn't open for read", relativePath);
		return false;
	}
	else
	{
		LOG_INFO("Complete load scene in {0}", relativePath);
		fin.read((char*)&objs, sizeof(objs));
	}

	fin.close();

	for (size_t i = 0; i < scene.get_readonly_list().size(); i++)
	{
		scene.get_list().push_back(objs[i]);
	}

	return true;
}
bool ResourceManager::save_scene(std::string relativePath, const Scene& scene)
{
	std::ofstream fout;
	
	fout.open(m_path + "/" + relativePath, std::ofstream::app);

	if (!fout.is_open())
	{
		LOG_ERROR("File {0} doesn't open for write", relativePath);
		return false;
	}
	else
	{
		LOG_INFO("Complete save scene in {0}", relativePath);
		//fout.write((char*)&objs, sizeof(objs));
	}

	fout.close();
	return true;
}
std::shared_ptr<GraphicsObject> ResourceManager::loadGraphicsObject(const std::string& name, const std::string& relativePath, const std::string& type)
{
	GraphicsObject* model;
	if (type == "obj")
	{
		model = loaders::load_obj_file(m_path + "/" + relativePath);

		if (model == nullptr)
		{
			LOG_WARN("Failed load OBJ file: {0}", relativePath);
			return nullptr;
		}
		else
		{
			std::shared_ptr<GraphicsObject> newOBJ =
				m_graphics_objects.emplace(name, std::make_shared<GraphicsObject>(std::move(model))).first->second;
			LOG_INFO("Success load OBJ file: {0}", relativePath);
			return newOBJ;
		}
	}
	LOG_ERROR("Error type model: {0}", type);
	return nullptr;
}
std::shared_ptr<GraphicsObject> ResourceManager::getGraphicsObject(const std::string& name)
{
	GrapicsObjectsMap::const_iterator it = m_graphics_objects.find(name);
	if (it != m_graphics_objects.end())
	{
		return it->second;
	}
	LOG_ERROR("Can't find OBJ model: {0}", name);
	return nullptr;
}
std::shared_ptr<LanguagePack> ResourceManager::load_lang_pack(std::string relativePath, std::string pack_name)
{
	std::shared_ptr<LanguagePack> pack = std::make_shared<LanguagePack>();
	pack->load_pack(m_path + "/" + relativePath);
	m_lang_packs_map.emplace(pack_name, pack);
	return pack;
}
std::shared_ptr<LanguagePack> ResourceManager::get_lang_pack(std::string pack_name)
{
	LangPacksMap::const_iterator it = m_lang_packs_map.find(pack_name);
	if (it != m_lang_packs_map.end())
	{
		return it->second;
	}
	LOG_ERROR("Can't find lang pack: {0}", pack_name);
	return nullptr;
}
std::string ResourceManager::getFileString(const std::string& relativeFilePath)
{
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath, std::ios::in | std::ios::binary);
	if (!f.is_open()) 
	{
		LOG_ERROR("Failed to open file: {0}", relativeFilePath);
		f.close();
		return std::string();
	}
	std::stringstream buffer;
	buffer << f.rdbuf();
	f.close();
	return buffer.str();
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath,
	const std::string& geometryPath,
	std::shared_ptr<RenderEngine::ShaderProgramLayout> layout)
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty()) 
	{
		LOG_ERROR("Vertex shader file is empty");
		return nullptr;
	}
	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		LOG_ERROR("Fragment shader file is empty");
		return nullptr;
	}
	std::string geometryString = "";
	if (!geometryPath.empty())
	{
		geometryString = getFileString(fragmentPath);
		if (geometryString.empty())
		{
			LOG_ERROR("Geometry shader file is empty");
			return nullptr;
		}
	}
	std::shared_ptr<RenderEngine::ShaderProgram> shader;
	ShaderProgramsMap::const_iterator it = m_ShaderPrograms.find(shaderName);
	if (it != m_ShaderPrograms.end()) shader = it->second;
	else shader = nullptr;
	if (shader == nullptr)
	{
		std::shared_ptr<RenderEngine::ShaderProgram>& newShader =
			m_ShaderPrograms.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString, geometryString, std::move(layout))).first->second;

		if (newShader->isCompiled())
		{
			LOG_INFO("Complete load & compile shader program: {0}", shaderName);
			return newShader;
		}
		else
		{
			if (geometryPath.empty())
			{
				LOG_ERROR("Can't load shader program: \nVertex: {0} \nFragment: {1}", vertexPath, fragmentPath);
				return nullptr;
			}
			LOG_ERROR("Can't load shader program: \nVertex: {0} \nFragment: {1} \nGeometry: {2}", vertexPath, fragmentPath, geometryPath);
			return nullptr;
		}
	}
	else
	{
		// hot reload resourses here ------------------------------	
	}

	return nullptr;
}
std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_ShaderPrograms.find(shaderName);
	if (it != m_ShaderPrograms.end())
	{
		return it->second;
	}
	LOG_ERROR("Can't find shader program: {0}", shaderName);
	return nullptr;
}
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channels = 0;
	int width = 0;
	int height = 0;
	unsigned char* pixels = loaders::load_image_png(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels);

	if (!pixels)
	{
		LOG_ERROR("Can't load image: {0}", texturePath);
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Texture2D> newTexture = m_textures.emplace(textureName,
		std::make_shared<RenderEngine::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	loaders::clear_image(pixels);

	return newTexture;
}
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}
	LOG_ERROR("Can't find texture: {0}", textureName);
	return nullptr;
}
std::shared_ptr<RenderEngine::Material> ResourceManager::loadMaterial(const std::string& materialName, const std::string& textureName, const std::string& shaderName)
{
	auto shader = getShaderProgram(shaderName);
	auto texture = getTexture(textureName);

	if (shader == nullptr)
	{
		LOG_ERROR("Can't find shader program {0} for material {1}", shaderName, materialName);
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Material> newMaterial =
	m_materials.emplace(materialName, std::make_shared<RenderEngine::Material>(shader, texture)).first->second;

	return newMaterial;
}
std::shared_ptr<RenderEngine::Material> ResourceManager::getMaterial(const std::string& materialName)
{
	MaterialsMap::const_iterator it = m_materials.find(materialName);
	if (it != m_materials.end())
	{
		return it->second;
	}
	LOG_ERROR("Can't find material: {0}", materialName);
	return nullptr;
}
std::vector<std::string> ResourceManager::getNamesShadersProgram()
{
	std::vector<std::string> data;

	for (const auto& curShader : m_ShaderPrograms)
	{
		data.push_back(curShader.first);
	}

	return data;
}
std::vector<std::string> ResourceManager::getNamesTextures2D()
{
	std::vector<std::string> data;

	for (const auto& curTexture : m_textures)
	{
		data.push_back(curTexture.first);
	}

	return data;
}
std::vector<std::string> ResourceManager::getNamesMaterials()
{
	std::vector<std::string> data;

	for (const auto& curMaterial : m_materials)
	{
		data.push_back(curMaterial.first);
	}

	return data;
}
std::vector<std::string> ResourceManager::getNamesObjs()
{
	std::vector<std::string> data;

	for (const auto& curObj : m_graphics_objects)
	{
		data.push_back(curObj.first);
	}

	return data;
}
std::vector<std::string> ResourceManager::getNamesFilesInDirectory(std::string relativeFilePath)
{
	std::vector<std::string> data;
	std::filesystem::path path;

	std::string tempStr;
	std::string temp2Str;
	std::string param = m_path + "/";

	for (auto& p : std::filesystem::directory_iterator(m_path + "/" + relativeFilePath))
	{
		path = p;
		tempStr = path.std::filesystem::path::generic_string();

		temp2Str = tempStr.substr(param.size());

		data.push_back(temp2Str);
	}
	return data;
}
std::string ResourceManager::getNameShaderProgram(std::shared_ptr<RenderEngine::ShaderProgram> pShader)
{
	std::string outStr = "";
	for (const auto& curShader : m_ShaderPrograms)
	{
		if (curShader.second == pShader)
		{
			outStr = curShader.first;
			break;
		}
	}
	return outStr;
}
std::string ResourceManager::getNameTexture2D(std::shared_ptr<RenderEngine::Texture2D> pTexture)
{
	std::string outStr = "";
	for (const auto& curTexture : m_textures)
	{
		if (curTexture.second == pTexture)
		{
			outStr = curTexture.first;
			break;
		}
	}
	return outStr;
}
std::shared_ptr<LanguagePack> ResourceManager::get_current_lang_pack()
{
	return m_current_lang_pack;
}
void ResourceManager::set_current_lang_pack(std::shared_ptr<LanguagePack> lang_pack)
{
	m_current_lang_pack = std::move(lang_pack);
}
//std::shared_ptr<RenderEngine::SpriteRenderer>  ResourceManager::loadSpriteRenderer(const std::string& SpriteRendererName, const std::string& textureName,
//	const std::string& shaderName, const std::string& subTextureName)
//{
//	auto pTexture = getTexture(textureName);
//	if (!pTexture)
//	{
//		LOG_ERROR("Can't find texture: {0} for the SpriteRenderer: {1}", textureName, SpriteRendererName);
//		return nullptr;
//	}
//
//	auto pShaderProgram = getShaderProgram(shaderName);
//	if (!pShaderProgram)
//	{
//		LOG_ERROR("Can't find shader program: {0} for the SpriteRenderer: {1}", shaderName, SpriteRendererName);
//		return nullptr;
//	}
//	std::shared_ptr<RenderEngine::SpriteRenderer>& newSpriteRenderer = m_SpriteRenderers.emplace(SpriteRendererName, std::make_shared<RenderEngine::SpriteRenderer>(pTexture, subTextureName, 
//		pShaderProgram)).first->second;
//	return newSpriteRenderer;
//}
//std::shared_ptr<RenderEngine::SpriteRenderer> ResourceManager::getSpriteRenderer(const std::string& SpriteRendererName)
//{
//	SpriteRenderersMap::const_iterator it = m_SpriteRenderers.find(SpriteRendererName);
//	if (it != m_SpriteRenderers.end())
//	{
//		return it->second;
//	}
//	LOG_ERROR("Can't find SpriteRenderer: {0}", SpriteRendererName);
//	return nullptr;
//}
//std::shared_ptr<RenderEngine::GraphicsObject> ResourceManager::loadGraphicsObject(const std::string& objName, const std::string& shaderName, const std::string& source)
//{
//	auto pShaderProgram = getShaderProgram(shaderName);
//	if (!pShaderProgram)
//	{
//		LOG_ERROR("Can't find shader program: {0} for the grapics object: {1}", shaderName, objName);
//		return nullptr;
//	}
//
//	std::shared_ptr<RenderEngine::GraphicsObject>& graphObj = m_graph_objs.emplace(objName, std::make_shared<RenderEngine::GraphicsObject>
//		(pShaderProgram, m_path + "/" + source)).first->second;
//
//	return graphObj;
//}
//std::shared_ptr<RenderEngine::GraphicsObject> ResourceManager::getGraphicsObject(const std::string& objName)
//{
//	GraphObjMap::const_iterator it = m_graph_objs.find(objName);
//	if (it != m_graph_objs.end())
//	{
//		return it->second;
//	}
//	LOG_ERROR("Can't find graphics object: {0}", objName);
//	return nullptr;
//}
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTextureAtlas(
	std::string textureName,
	std::vector<std::string> subTextures,
	std::string texturePath,
	const unsigned int width,
	const unsigned int height,
	const unsigned int subTextureWidth,
	const unsigned int subTextureHeight)
{
	auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));
	if (pTexture) 
	{
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = height;
		for (auto& currentSubTextureName : subTextures)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / width, static_cast<float>(currentTextureOffsetY - subTextureHeight) / height);
			glm::vec2 rightTop(static_cast<float>(currentTextureOffsetX + subTextureWidth) / width, static_cast<float>(currentTextureOffsetY) / height);
			pTexture->addSubTexture(std::move(currentSubTextureName), leftBottomUV, rightTop);
			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= width)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
		}
	}
	return pTexture;
}

//std::shared_ptr<RenderEngine::Animator>  ResourceManager::loadAnimator(
//	const std::string& animatorName,
//	const std::string& SpriteRendererName)
//{
//	std::shared_ptr<RenderEngine::Animator>& newAnimator = m_animators.emplace(animatorName, std::make_shared<RenderEngine::Animator>(SpriteRendererName)).first->second;
//	return newAnimator;
//}
//std::shared_ptr<RenderEngine::Animator> ResourceManager::getAnimator(const std::string& animatorName)
//{
//	AnimatorsMap::const_iterator it = m_animators.find(animatorName);
//	if (it != m_animators.end())
//	{
//		return it->second;
//	}
//	std::cerr << "Can't find animator: " << animatorName << "\n";
//	return nullptr;
//}