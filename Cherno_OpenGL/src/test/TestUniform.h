#pragma once

#include "Test.h"
#include <string>

#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexArray.h"
#include "../Shader.h"
#include "../VertexBufferLayout.h"
#include "../Texture.h"
#include "../Renderer.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



namespace Test
{
	class TestUniform : public Test
	{
	private:
		float				m_ClearColor[4];
		float				m_ObjectColor[4];
		float				m_Positions[8];
		unsigned int		m_Indices[6];
		float				m_Step;
		int					m_Direction;
		bool				m_Oscillate;

		VertexArray			m_VA;
		IndexBuffer			m_IB;
		VertexBuffer		m_VB;
		VertexBufferLayout	m_Layout;
		Shader				m_Shader;
		Renderer			m_Renderer;

	public:
		TestUniform();
		~TestUniform();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRenderer() override;

	};
}
