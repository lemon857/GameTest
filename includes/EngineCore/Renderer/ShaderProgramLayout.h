#pragma once

#include <string>
#include <vector>

enum ETypeData
{
	Int = 0,
	Float,
	Vec3,
	Vec4,
	Mat4,
	ModelMat4,
	VPMat4
};

namespace RenderEngine
{
	struct ShaderProgramLayoutElement
	{
		std::string name;
		ETypeData type;
		double min;
		double max;
	};
	class ShaderProgramLayout
	{
	public:
		ShaderProgramLayout();

		void addElementLayout(std::string name, ETypeData type, double min, double max);

		const std::vector<ShaderProgramLayoutElement>& getLayoutElements() const { return m_layoutElements; };

	private:
		std::vector<ShaderProgramLayoutElement> m_layoutElements;
	};

}