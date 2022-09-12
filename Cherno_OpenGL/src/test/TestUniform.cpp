#pragma once


#include "TestUniform.h"
#include <GL/glew.h>
#include "../Debug.h"
#include "imgui/imgui.h"


namespace Test
{
	TestUniform::TestUniform():
		m_ClearColor{ 0.8f, 0.3f, 0.2f, 1.0f },
		m_ObjectColor{ 0.2f, 0.3f, 0.8f, 1.0f },
		m_Positions{
			-0.5f, -0.5f, // 0
			 0.5f, -0.5f, // 1
			 0.5f,  0.5f, // 2
			-0.5f,  0.5f  // 3
		},
		m_Indices{
			0, 1, 2,
			2, 3, 0
		},
		m_Step(0.05),
		m_Direction(1),
		m_Oscillate(false),
		m_VA(),
		m_VB(m_Positions, 4 * 2 * sizeof(float)),
		m_IB(m_Indices, 6),
		m_Layout(),
		m_Shader("res/shaders/Uniform.shader")
	{

		m_Layout.Push<float>(2);
		m_VA.AddBuffer(m_VB, m_Layout);
	}


	TestUniform::~TestUniform()
	{
		m_VA.UnBind();
		m_VB.UnBind();
		m_Shader.UnBind();
	}

	void TestUniform::OnUpdate(float deltaTime)
	{

		// Ideally we would actually specify a "rate" instead of a step, and then we would normalize the points stepped based on the
		// input deltaTime, i.e. if rate = 100 points / sec, or 100 points / 1000 ms, or 0.1 points / ms, then we pass in delta time
		// in ms and the points we should step are 0.1 * deltaTime * direction.
		if (m_Oscillate)
		{
			// increment red
			float* red = m_ObjectColor;
			if (*red < 0.0f)
				m_Direction = 1;
			if (*red > 1.0f)
				m_Direction = -1;
			*red += m_Step * m_Direction;
		}
	}

	void TestUniform::OnRender()
	{

		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));

		m_Shader.Bind();
		m_Shader.SetUniform4f("u_Color",
			m_ObjectColor[0],
			m_ObjectColor[1],
			m_ObjectColor[2],
			m_ObjectColor[3]);

		m_Renderer.Draw(m_VA, m_IB, m_Shader);

		// 3 indices starting at 0 -> 1 triangle
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
	}

	void TestUniform::OnImGuiRenderer()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
		ImGui::ColorEdit4("Object Color", m_ObjectColor);
		ImGui::Checkbox("Oscillate Red", &m_Oscillate);
		ImGui::SliderFloat("Red Oscillation Speed", &m_Step, 0.0f, 0.25f);
	}
}
