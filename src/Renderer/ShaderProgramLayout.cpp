#include "EngineCore/Renderer/ShaderProgramLayout.h"

namespace RenderEngine
{
	ShaderProgramLayout::ShaderProgramLayout()
	{

	}

	void ShaderProgramLayout::addElementLayout(std::string name, ETypeData data, double min, double max)
	{
		m_layoutElements.push_back({ name, data, min, max });
	}
}