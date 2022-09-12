#pragma once

#include "Test.h"
#include <string>

#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexArray.h"
#include "../Shader.h"
#include "../VertexBufferLayout.h"
#include "../Texture.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



/*
 ²âÊÔ°¸Àý£¬Èý½ÇÐÎ
*/
namespace Test
{
	class TestTriangle : public Test
	{
	private:
		float					m_ClearColor[4];
		float					m_Positions[6];

		VertexArray				m_VA;
		VertexBuffer			m_VB;
		VertexBufferLayout		m_Layout;
		Shader					m_Shader;

	public:
		TestTriangle();
		~TestTriangle();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRenderer() override;

		inline VertexArray GetVA() const { return m_VA; }
	};
}
