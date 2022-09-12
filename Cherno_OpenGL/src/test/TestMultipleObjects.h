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



/*
 测试案例，多个对象
*/
namespace Test
{
	class TestMultipleObjects : public Test
	{
	private:
		// data members
		float				m_ClearColor[4];
		float				m_Positions[16];
		unsigned int		m_Indices[6];

		// opengl members
		VertexArray			m_VA;
		IndexBuffer			m_IB;
		VertexBuffer		m_VB;
		VertexBufferLayout	m_Layout;
		Shader				m_Shader;
		Texture				m_Texture;
		Renderer			m_Renderer;

		// MVP members
		glm::mat4			m_Proj;
		glm::mat4			m_View;
		// model and mvp will be created per object per draw

		// Translation members
		glm::vec3			m_TranslationA;
		glm::vec3			m_TranslationB;

	public:
		TestMultipleObjects();
		~TestMultipleObjects();

		virtual void OnUpdate(float deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRenderer() override;
	};
}
