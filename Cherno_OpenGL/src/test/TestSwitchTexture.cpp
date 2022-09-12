#pragma once


#include "TestSwitchTexture.h"
#include <GL/glew.h>
#include "../Debug.h"
#include "imgui/imgui.h"


namespace Test
{
	TestSwitchTexture::TestSwitchTexture(const std::string& textureFilePathA, const std::string& textureFilePathB) :
		m_ActiveA(true),
		m_MixColor{ 0.2f,0.3f,0.8f,1.0f },
		m_ClearColor{ 0.2f,0.3f,0.8f,1.0f },
		m_Renderer(),
		m_Positions{
		-50.0f,	 -50.0f, 0.0f, 0.0f, // 0
		 50.0f,  -50.0f, 1.0f, 0.0f, // 1
		 50.0f,   50.0f, 1.0f, 1.0f, // 2
		-50.0f,	  50.0f, 0.0f, 1.0f,  // 3
		},
		m_Indices{
			0,1,2,
			2,3,0
		},
		m_VA(),
		m_VB(m_Positions, (sizeof(float) * 4 * 4)),
		m_Layout(),
		m_Ib(m_Indices, 2 * 3),
		m_Shader("res/shaders/SwitchTexture.shader"),
		m_TextureA(textureFilePathA),
		m_TextureB(textureFilePathB)
	{

		m_Trans = glm::vec3(100.0f, 100.0f, 0.0f);

		m_Layout.Push<float>(2);
		m_Layout.Push<float>(2);
		m_VA.AddBuffer(m_VB, m_Layout);

		m_Shader.Bind();

		m_TextureA.Bind();
		m_TextureA.UnBind();
		m_TextureB.Bind(1);
		m_TextureB.UnBind();
	}


	TestSwitchTexture::~TestSwitchTexture()
	{
		m_VA.UnBind();
		m_VB.UnBind();
		m_Shader.UnBind();
		m_TextureA.UnBind();
		m_TextureB.UnBind();
	}

	void TestSwitchTexture::OnUpdate(float deltaTime)
	{

	}

	void TestSwitchTexture::OnRender()
	{
		// 颜色
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		glm::mat4 ident = glm::mat4(1.0f);
		glm::mat4 view = glm::translate(ident, m_Trans);
		glm::mat4 mvp = proj * view * model;

		m_Shader.Bind();
		// mvp
		m_Shader.SetUniformMat4f("u_MVP", mvp);
		// 混合颜色
		m_Shader.SetUniform4f("u_Color", m_MixColor[0], m_MixColor[1], m_MixColor[2], m_MixColor[3]);

		// 纹理选择
		if (m_ActiveA)
		{
			m_TextureA.Bind();
			m_Shader.SetUniform1i("u_Texture", 0);
		}
		else
		{
			m_TextureB.Bind(1);
			m_Shader.SetUniform1i("u_Texture", 1);
		}
		m_Renderer.Draw(m_VA, m_Ib, m_Shader);
	}

	void TestSwitchTexture::OnImGuiRenderer()
	{

		ImGui::ColorEdit4("Clear Color", m_ClearColor);
		ImGui::ColorEdit4("Mix Color", m_MixColor);
		ImGui::Checkbox("Active TextureA", &m_ActiveA);
		ImGui::SliderFloat3("trans A", &m_Trans.x, 0.0f, 960.0f);

	}
}
