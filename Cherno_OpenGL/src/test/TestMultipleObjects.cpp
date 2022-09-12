#pragma once


#include "TestMultipleObjects.h"
#include <GL/glew.h>
#include "../Debug.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Test
{
	TestMultipleObjects::TestMultipleObjects():
		m_ClearColor{ 0.8f, 0.3f, 0.2f, 1.0f },
		m_Positions{
			-50.0f, -50.0f, 0.0f, 0.0f, // 0
			 50.0f, -50.0f, 1.0f, 0.0f, // 1
			 50.0f,  50.0f, 1.0f, 1.0f, // 2
			-50.0f,  50.0f, 0.0f, 1.0f  // 3
	},
		m_Indices{
			0, 1, 2,
			2, 3, 0
	},
		m_VA(),
		m_IB(m_Indices, 6),
		m_VB(m_Positions, 4 * 4 * sizeof(float)),
		m_Layout(),
		m_Shader("res/shaders/Complex.shader"),
		m_Texture("res/textures/phone.png"),
		m_Renderer(),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0),
		m_TranslationB(400, 200, 0)
	{

		m_Layout.Push<float>(2);
		m_Layout.Push<float>(2);
		m_VA.AddBuffer(m_VB, m_Layout);

		m_Texture.Bind();
		m_Shader.Bind();
		m_Shader.SetUniform1i("u_Texture", 0);
	}


	TestMultipleObjects::~TestMultipleObjects()
	{
		m_VA.UnBind();
		m_Shader.UnBind();
	}

	void TestMultipleObjects::OnUpdate(float deltaTime)
	{

	}

	void TestMultipleObjects::OnRender()
	{

		m_Renderer.Clear();
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader.Bind();
			m_Shader.SetUniformMat4f("u_MVP", mvp);
			m_Renderer.Draw(m_VA, m_IB, m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader.Bind();
			m_Shader.SetUniformMat4f("u_MVP", mvp);
			m_Renderer.Draw(m_VA, m_IB, m_Shader);
		}
	}

	void TestMultipleObjects::OnImGuiRenderer()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
		ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0f, 960.0f);
	}
}
