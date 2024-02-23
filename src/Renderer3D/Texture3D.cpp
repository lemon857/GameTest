#include "EngineCore/Renderer3D/Texture3D.h"
#include "EngineCore/System/Loaders.h"
#include "EngineCore/System/Log.h"
#include "EngineCore/Resources/ResourceManager.h"

#include <glad/glad.h>

RenderEngine::Texture3D::Texture3D(std::vector<std::string> faces_rel_path)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces_rel_path.size(); i++)
	{
		unsigned char* data = loaders::load_image((ResourceManager::getExeFilePath() + "/" + faces_rel_path[i]).c_str(), &width, &height, &nrChannels, false);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, nrChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA,  GL_UNSIGNED_BYTE, data
			);
		}
		else
		{
			LOG_ERROR("Loading textuer failed: {0}", faces_rel_path[i]);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

RenderEngine::Texture3D::~Texture3D()
{
}

RenderEngine::Texture3D& RenderEngine::Texture3D::operator=(Texture3D&& texture2D) noexcept
{
	glDeleteTextures(1, &m_ID);

	m_ID = texture2D.m_ID;
	texture2D.m_ID = 0;
	return *this;
}

RenderEngine::Texture3D::Texture3D(Texture3D&& texture2D) noexcept
{
	m_ID = texture2D.m_ID;
	texture2D.m_ID = 0;
}

void RenderEngine::Texture3D::bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}
