#include "EngineCore/Resources/ResourceManager.h";
#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Texture2D.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"
#include "EngineCore/Renderer/Animation.h"
#include "EngineCore/Renderer/Animator.h"
#include "EngineCore/System/Log.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "EngineCore/Resources/stb_image.h"

ResourceManager::ShaderProgramsMap ResourceManager::m_ShaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
//ResourceManager::SpriteRenderersMap ResourceManager::m_SpriteRenderers;
//ResourceManager::GraphObjMap ResourceManager::m_graph_objs;
//ResourceManager::AnimatorsMap ResourceManager::m_animators;
std::string ResourceManager::m_path;


void ResourceManager::unloadAllResources()
{
	m_ShaderPrograms.clear();
	m_textures.clear();
	//m_SpriteRenderers.clear();
}
void ResourceManager::setExecutablePath(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);
}
bool ResourceManager::loadJSONresources(const std::string & JSONpath)
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
			loadShaders(name, vertShader, fragShader);
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
	/*auto SpriteRenderersIt = doc.FindMember("SpriteRenderers");
	if (SpriteRenderersIt != doc.MemberEnd())
	{
		for (const auto& currenSpriteRenderer : SpriteRenderersIt->value.GetArray())
		{
			const std::string name = currenSpriteRenderer["name"].GetString();
			const std::string shader = currenSpriteRenderer["shader"].GetString();
			const std::string atlas = currenSpriteRenderer["textureAtlas"].GetString();
			const std::string initSubTexture = currenSpriteRenderer["initialSubTexture"].GetString();

			loadSpriteRenderer(name, atlas, shader, initSubTexture);			
		}
	}
	auto objectsIt = doc.FindMember("objects");
	if (objectsIt != doc.MemberEnd())
	{
		for (const auto& currenObject : objectsIt->value.GetArray())
		{
			const std::string name = currenObject["name"].GetString();
			const std::string shader = currenObject["shader"].GetString();
			const std::string source = currenObject["source"].GetString();
			
			loadGraphicsObject(name, shader, source);
		}
	}*/
	LOG_INFO("Loadind data in JSON file complete");
	/*auto animatorsIt = doc.FindMember("animators");
	if (animatorsIt != doc.MemberEnd())
	{
		for (const auto& currenAnimator : animatorsIt->value.GetArray())
		{
			const std::string name = currenAnimator["name"].GetString();
			const std::string SpriteRendererName = currenAnimator["SpriteRendererName"].GetString();
			auto pAnimator = loadAnimator(name, SpriteRendererName);

			const auto animationsArray = currenAnimator["Animations"].GetArray();

			for (const auto& currentAnimation : animationsArray)
			{
				const std::string animationName = currentAnimation["name"].GetString();
				const int callbackAction = currentAnimation["callbackAction"].GetInt();
				const int typeAnimation = currentAnimation["typeAnimation"].GetInt();
				const auto subTextureNamesArray = currentAnimation["subTextureNames"].GetArray();
				const auto durationsArray = currentAnimation["durations"].GetArray();

				std::vector<std::string> subTextureNames;
				std::vector<double> durations;
				
				for (size_t i = 0; i < subTextureNamesArray.Size(); i++)
				{
					subTextureNames.push_back(subTextureNamesArray[i].GetString());
					durations.push_back(durationsArray[i].GetDouble());
				}
				
				pAnimator->addAnimation(animationName, std::make_shared<RenderEngine::Animation>(subTextureNames, durations, 
					RenderEngine::ECallbackAction(callbackAction), RenderEngine::ETypeAnimation(typeAnimation)));
			}
		}
	}*/
	return true;
}
bool ResourceManager::loadINIsettings(const std::string& INIpath, INIdata& data, const bool isWrite)
{
	std::ifstream f;
	f.open(m_path + "/" + INIpath, std::ios::in | std::ios::binary);

	if (f.is_open() && !isWrite)
	{

		std::stringstream buf;
		buf << f.rdbuf();
		f.close();

		std::string INIstr = buf.str();
		if (!INIstr.empty() && !isWrite)
		{
			const size_t newLine = INIstr.find('\n');
			const size_t newLine2 = INIstr.substr(newLine + 1, INIstr.size() - newLine + 1).find('\n');

			const std::string size = INIstr.substr(0, newLine - 1);
			const std::string pos = INIstr.substr(newLine + 1, newLine2 - 1);

			const std::string width = size.substr(0, size.find(' '));
			const std::string height = size.substr(size.find(' ') + 1, size.size());

			const std::string posX = pos.substr(0, pos.find(' '));
			const std::string posY = pos.substr(pos.find(' ') + 1, pos.size());

			const std::string maximized = INIstr.substr(newLine + newLine2 + 2, INIstr.size() - newLine2 - newLine);

			LOG_INFO("Succsess read file {0}", INIpath);
			LOG_INFO("Data {2} {0}x{1} {3}x{4}", width, height, maximized, posX, posY);

			data.window_size = glm::ivec2(std::stoi(width), std::stoi(height));
			data.window_position = glm::ivec2(std::stoi(posX), std::stoi(posY));
			data.maximized_window = std::stoi(maximized) == 1 ? true : false;
			return true;
		}
	}
	std::ofstream writeStream;
	writeStream.open(m_path + "/" + INIpath);

	if (writeStream.is_open())
	{
		LOG_INFO("Succsess open file {0} for write data", INIpath);
		writeStream << std::to_string(data.window_size.x) << " " << std::to_string(data.window_size.y) << "\n";
		writeStream << std::to_string(data.window_position.x) << " " << std::to_string(data.window_position.y) << "\n";
		writeStream << std::to_string(data.maximized_window == true ? 1 : 0);
		writeStream.close();
		return true;
	}
	else
	{
		LOG_ERROR("Error open file: {0}", INIpath);
		writeStream.close();
		return false;
	}
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
std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
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

	std::shared_ptr<RenderEngine::ShaderProgram>& newShader = m_ShaderPrograms.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}

	LOG_ERROR("Can't load shader program: \nVertex: {0} \nFragment: {1}", vertexPath, fragmentPath);
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
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

	if (!pixels)
	{
		LOG_ERROR("Can't load image: {0}", texturePath);
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Texture2D> newTexture = m_textures.emplace(textureName,
		std::make_shared<RenderEngine::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);

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