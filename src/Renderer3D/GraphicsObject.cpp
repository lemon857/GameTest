#include "EngineCore/Renderer3D/GraphicsObject.h"
#include "EngineCore/System/Log.h"

#include "EngineCore/Renderer/Renderer.h"

#include <fstream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define DEBUG_CONSOLE

bool start_with(std::string& line, const char* text)
{
	size_t texLen = strlen(text);
	if (line.size() < texLen) return false;
	for (size_t i = 0; i < texLen; i++)
	{
		if (line[i] == text[i]) continue;
		else return false;
	}
	return true;
}

namespace RenderEngine
{
	GraphicsObject::GraphicsObject(std::shared_ptr<ShaderProgram> pShaderProgram, const std::string& filePath)
		: m_pShaderProgram(std::move(pShaderProgram))
	{
		std::ifstream file;
		file.open(filePath);
		if (file.is_open())
		{
			std::vector<GLfloat> norms;
			std::vector<GLfloat> texs;
			std::string line;
			while (std::getline(file, line))
			{
				if (start_with(line, "v "))
				{
					GLfloat x, y, z;
					sscanf_s(line.c_str(), "v %f %f %f", &x, &y, &z);
					m_vertex_coords.push_back(x);
					m_vertex_coords.push_back(y);
					m_vertex_coords.push_back(z);
					// for future
					m_texture_coord.push_back(0);
					m_texture_coord.push_back(0);
					m_texture_coord.push_back(0);

					m_normal_coords.push_back(0);
					m_normal_coords.push_back(0);
					m_normal_coords.push_back(0);
#ifdef DEBUG_CONSOLE
					LOG_INFO("Vert: {0}x{1}x{2}", x, y, z);
#endif // DEBUG_CONSOLE

				}
				else if (start_with(line, "vn "))
				{
					GLfloat x, y, z;
					sscanf_s(line.c_str(), "vn %f %f %f", &x, &y, &z);
					norms.push_back(x);
					norms.push_back(y);
					norms.push_back(z);
#ifdef DEBUG_CONSOLE
					LOG_INFO("Norms: {0}x{1}x{2}", x, y, z);
#endif // DEBUG_CONSOLE
				}
				else if (start_with(line, "vt "))
				{
					GLfloat x, y;
					sscanf_s(line.c_str(), "vt %f %f", &x, &y);
					texs.push_back(x);
					texs.push_back(y);
#ifdef DEBUG_CONSOLE
					LOG_INFO("Texs: {0}x{1}", x, y);
#endif // DEBUG_CONSOLE
				}
				// WARNING region (# faces) in .obj file must be lastest
				else if (start_with(line, "f "))
				{
					int indexX, textureX, normalX;
					int indexY, textureY, normalY;
					int indexZ, textureZ, normalZ;
					sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
						&indexX, &textureX, &normalX, &indexY, &textureY, &normalY, &indexZ, &textureZ, & normalZ);

					// inexes load
					m_index_array.push_back(indexX);
					m_index_array.push_back(indexY);
					m_index_array.push_back(indexZ);

					// normals load
					m_normal_coords[indexX] = norms[normalX];
					m_normal_coords[indexY] = norms[normalY];
					m_normal_coords[indexZ] = norms[normalZ];

					// textures load
					m_texture_coord[indexX] = texs[textureX];
					m_texture_coord[indexY] = texs[textureY];
					m_texture_coord[indexZ] = texs[textureZ];

#ifdef DEBUG_CONSOLE
					LOG_INFO("Faces: {0}x{1}x{2} {3}x{4}x{5} {6}x{7}x{8}", indexX, textureX, normalX, indexY, textureY, normalY, indexZ, textureZ, normalZ);
#endif // DEBUG_CONSOLE
				}
			}

			m_vbo_vert.init(m_vertex_coords.data(), m_vertex_coords.size() * sizeof(GLfloat), false);
			VertexBufferLayout vertexCoordsLayout;
			vertexCoordsLayout.addElementLayoutFloat(3, false);
			m_vao.addBuffer(m_vbo_vert, vertexCoordsLayout);

			m_vbo_normal.init(m_normal_coords.data(), m_normal_coords.size() * sizeof(GLfloat), false);
			VertexBufferLayout normalCoordsLayout;
			normalCoordsLayout.addElementLayoutFloat(3, false);
			m_vao.addBuffer(m_vbo_normal, normalCoordsLayout);

			m_vbo_texture.init(m_texture_coord.data(), m_texture_coord.size() * sizeof(GLfloat), false);
			VertexBufferLayout textureCoordsLayout;
			textureCoordsLayout.addElementLayoutFloat(2, false);
			m_vao.addBuffer(m_vbo_texture, textureCoordsLayout);

			//m_ebo.init(m_index_array.data(), m_index_array.size() * sizeof(GLuint));
			
			m_vao.unbind();
			//m_ebo.unbind();
		}
		file.close();
	}
	void GraphicsObject::render(const glm::vec3& position, const glm::vec3& size)
	{		
		m_pShaderProgram->use();
		glm::mat4 model(1.f);

		//model = glm::translate(model, position);
		//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
		//model = glm::rotate(model, static_cast<float>(glm::radians(rotation)),  glm::vec3(0.f, 0.f, 1.f));
		//model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f));
		//model = glm::scale(model, size);

		m_pShaderProgram->setMatrix4("modelMat", model);
		Renderer::drawTriangles(m_vao, m_ebo);
	}
}