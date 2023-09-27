#include "EngineCore/GUI/Square.h"

#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/System/ShadersSettings.h"

namespace GUI
{
	static int g_current_square_ID = 0;
	Square::Square(std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec2 pos, glm::vec2 scale)
		: GUI_element(pMaterial, "Square" + std::to_string(g_current_square_ID++))
		, m_vertexArray(std::make_shared<RenderEngine::VertexArray>())
		, m_vertexCoordsBuffer(new RenderEngine::VertexBuffer())
		, m_indexBuffer(new RenderEngine::IndexBuffer())
		, m_color(glm::vec3(1.f))
	{
		m_position = pos;
		m_scale = scale;
		const GLfloat vertexCoords[] = {
			//2--3  1
			//| / /	|
			//1	 3--2	
			//1--2
			//| /|
			//0--3
			//X --- Y
			-1.f,-1.f, 0.f,
			-1.f, 1.f, 0.f,
			 1.f, 1.f, 0.f,
			 1.f, -1.f, 0.f
		};

		const GLuint indexes[] = { 0, 1, 2, 2, 3, 0 };

		m_vertexCoordsBuffer->init(&vertexCoords, 3 * 4 * sizeof(GLfloat), false);
		RenderEngine::VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		m_vertexArray->addBuffer(*m_vertexCoordsBuffer, vertexCoordsLayout);

		m_indexBuffer->init(&indexes, 6);

		m_vertexArray->unbind();
		m_indexBuffer->unbind();
	}
	Square::~Square()
	{
		delete m_indexBuffer;
		delete m_vertexCoordsBuffer;
		delete m_vertexArray.get();
	}
	void Square::on_render()
	{
		glm::mat4 scaleMat(
			m_scale[0], 0, 0, 0,
			0,m_scale[1], 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		glm::mat4 translateMat(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			m_position[0], m_position[1], 0, 1);

		glm::mat4 model = translateMat * scaleMat;

		m_pMaterial->use();
		m_pMaterial->set_model_matrix(model);
		m_pMaterial->get_shader_ptr()->setVec4(SS_COLOR_PROP_NAME, glm::vec4(m_color, 1.f));

		RenderEngine::Renderer::drawTriangles(*m_vertexArray, *m_indexBuffer);
	}
	void Square::set_color(glm::vec3 color)
	{
		m_color = color;
	}
}