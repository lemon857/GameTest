#include "ResourceManager.h";
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ShaderProgramsMap ResourceManager::m_ShaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpritesMap ResourceManager::m_sprites;
//ResourceManager::AnimatedSpritesMap ResourceManager::m_animateSprites;
std::string ResourceManager::m_path;


void ResourceManager::unloadAllResources()
{
	m_ShaderPrograms.clear();
	m_textures.clear();
	m_sprites.clear();
	//m_animateSprites.clear();
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
		std::cerr << "No JSON file: " << JSONpath << "\n";
		return false;
	}
	rapidjson::Document doc;
	rapidjson::ParseResult result = doc.Parse(JSONstr.c_str());
	if (!result)
	{
		std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(result.Code()) << "(" << result.Offset() << ")\n";
		std::cerr << "In JSON file: " << JSONpath << "\n";
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
	auto spritesIt = doc.FindMember("sprites");
	if (spritesIt != doc.MemberEnd())
	{
		for (const auto& currenSprite : spritesIt->value.GetArray())
		{
			const std::string name = currenSprite["name"].GetString();
			const std::string shader = currenSprite["shader"].GetString();
			const std::string atlas = currenSprite["textureAtlas"].GetString();
			const std::string initSubTexture = currenSprite["initialSubTexture"].GetString();

			auto sprite = loadSprite(name, atlas, shader, initSubTexture);
			/*if (!sprite) continue;

			const auto states = currentAnimSprite["states"].GetArray();

			for (const auto& currentState : states)
			{
				const std::string stateName = currentState["stateName"].GetString();
				std::vector <std::pair<std::string, double>> frames;
				const auto framesArray = currentState["frames"].GetArray();
				frames.reserve(framesArray.Size());
				for (const auto& currentFrame : framesArray)
				{
					const std::string subTexture = currentFrame["subTexture"].GetString();
					const double duration = currentFrame["duration"].GetDouble();
					frames.emplace_back(std::pair<std::string, double>(subTexture, duration));
				}
				sprite->insertState(stateName, std::move(frames));
			}*/
		}
	}
	return true;
}
std::string ResourceManager::getFileString(const std::string& relativeFilePath)
{
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath, std::ios::in | std::ios::binary);
	if (!f.is_open()) 
	{
		std::cerr << "Failed to open file: " + relativeFilePath + "\n";
		return std::string();
	}
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}
std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty()) 
	{
		std::cerr << "Vertex shader file is empty\n";
		return nullptr;
	}
	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cerr << "Fragment shader file is empty\n";
		return nullptr;
	}

	std::shared_ptr<RenderEngine::ShaderProgram>& newShader = m_ShaderPrograms.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}

	std::cerr << "Can't load shader program:\n" << "Vertex: " << vertexPath << "\nFragment: " << fragmentPath;
	return nullptr;
}
std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_ShaderPrograms.find(shaderName);
	if (it != m_ShaderPrograms.end())
	{
		return it->second;
	}
	std::cerr << "Can't find shader program: " << shaderName << "\n";
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
		std::cerr << "Can't load image: " << texturePath << "\n";
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
	std::cerr << "Can't find texture: " << textureName << "\n";
	return nullptr;
}
std::shared_ptr<RenderEngine::Sprite>  ResourceManager::loadSprite(const std::string& spriteName, const std::string& textureName,
	const std::string& shaderName, const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Can't find texture: " << textureName << " for the sprite: " << spriteName << "\n";
		return nullptr;
	}

	auto pShaderProgram = getShaderProgram(shaderName);
	if (!pShaderProgram)
	{
		std::cerr << "Can't find shader program: " << shaderName << " for the sprite: " << spriteName << "\n";
		return nullptr;
	}
	std::shared_ptr<RenderEngine::Sprite>& newSprite = m_sprites.emplace(spriteName, std::make_shared<RenderEngine::Sprite>(pTexture, subTextureName, 
		pShaderProgram)).first->second;
	return newSprite;
}
std::shared_ptr<RenderEngine::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}
	std::cerr << "Can't find sprite: " << spriteName << "\n";
	return nullptr;
}
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTextureAtlas(
	std::string textureName,
	const std::vector<std::string> subTextures,
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

//std::shared_ptr<RenderEngine::AnimatedSprite>  ResourceManager::loadAnimatedSprite(
//	const std::string& spriteName,
//	const std::string& textureName,
//	const std::string& shaderName,
//	const unsigned int spriteWidth,
//	const unsigned int spriteHeight,
//	const float rotation,
//	const float layer,
//	const std::string& subTextureName)
//{
//	auto pTexture = getTexture(textureName);
//	if (!pTexture)
//	{
//		std::cerr << "Can't find texture: " << textureName << " for the animated sprite: " << spriteName << "\n";
//		return nullptr;
//	}
//
//	auto pShaderProgram = getShaderProgram(shaderName);
//	if (!pShaderProgram)
//	{
//		std::cerr << "Can't find shader program: " << shaderName << " for the animated sprite: " << spriteName << "\n";
//		return nullptr;
//	}
//	std::shared_ptr<RenderEngine::AnimatedSprite>& newSprite = m_animateSprites.emplace(spriteName, std::make_shared<RenderEngine::AnimatedSprite>(pTexture, subTextureName,
//		pShaderProgram, glm::vec2(0.0f, 0.0f), glm::vec2(spriteWidth, spriteHeight), rotation, layer)).first->second;
//	return newSprite;
//}
//std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::getAnimatedSprite(const std::string& spriteName)
//{
//	AnimatedSpritesMap::const_iterator it = m_animateSprites.find(spriteName);
//	if (it != m_animateSprites.end())
//	{
//		return it->second;
//	}
//	std::cerr << "Can't find animated sprite: " << spriteName << "\n";
//	return nullptr;
//}