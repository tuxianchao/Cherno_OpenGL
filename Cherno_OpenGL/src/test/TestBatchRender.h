#pragma once
#include "Test.h"
#include "../Renderer.h"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../Texture.h"
#include "../Shader.h"
#include "glm/glm.hpp"

namespace Test
{
	class TestBatchRender :public Test
	{
	private:
		float				m_ClearColor[8];
		float				m_Positions[88];
		unsigned int		m_Indices[12];
		VertexArray			m_VAO;
		VertexBuffer		m_VBO;
		IndexBuffer			m_IBO;
		VertexBufferLayout	m_Layout;
		Shader				m_Shader;
		Texture				m_Textures[2];

		glm::mat4			m_Proj;
		glm::mat4			m_View;
		glm::vec3			m_Trans;
		Renderer			m_Renderer;

	public:

		TestBatchRender();
		~TestBatchRender();



		void OnUpdate(float deltaTime) override;
		void OnRender()  override;
		void OnImGuiRenderer()  override;
	};
}