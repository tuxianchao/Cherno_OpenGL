#pragma once


#include "TestTriangle.h"
#include <GL/glew.h>
#include "../Debug.h"
#include "imgui/imgui.h"


namespace Test
{
	TestTriangle::TestTriangle():
		m_ClearColor{ 0.8f, 0.3f, 0.2f, 1.0f },
		m_Positions{
			-0.5f, -0.5,
			 0.5f, -0.5f,
			 0.0f,	0.5f
		},
		m_VA(),
		m_VB(m_Positions, sizeof(float) * 3 * 2),
		m_Layout(),
		m_Shader("res/shaders/Simple.shader")
	{

		m_Layout.Push<float>(2);
		m_VA.AddBuffer(m_VB, m_Layout);
	}


	TestTriangle::~TestTriangle()
	{
		m_VA.unBind();
		m_VB.UnBind();
		m_Shader.UnBind();
	}

	void TestTriangle::OnUpdate(float deltaTime)
	{

	}

	void TestTriangle::OnRender()
	{

		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));

		m_VA.Bind();
		m_Shader.Bind();

		// 3 indices starting at 0 -> 1 triangle
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
	}

	void TestTriangle::OnImGuiRenderer()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}
}
