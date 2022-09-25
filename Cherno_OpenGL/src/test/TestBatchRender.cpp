#include "TestBatchRender.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"


namespace Test
{
	TestBatchRender::TestBatchRender() :
		m_ClearColor{ 0.8f, 0.3f, 0.2f, 1.0f },
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
		m_Trans(glm::vec3(0, 0, 0)),
		m_Positions{
			100.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			200.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			200.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			100.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

			300.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			400.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			400.0f, 200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			300.0f, 200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		},
		m_Indices{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		},
		m_Renderer(),
		m_VAO(),
		m_VBO(m_Positions, 11 * 8 * sizeof(float)),
		m_IBO(m_Indices, 12),
		m_Layout(),
		m_Shader("res/shaders/Batch.shader"),
		m_Textures{ Texture("res/textures/phone.png"),Texture("res/textures/gold-dollar.png") }
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_Layout.Push<float>(4); // 坐标 x, y, z, w  w 齐次坐标, 对xyz进行缩放
		m_Layout.Push<float>(4); // 颜色数据
		m_Layout.Push<float>(2); // 纹理坐标
		m_Layout.Push<float>(1); // 纹理插槽
		m_VAO.AddBuffer(m_VBO, m_Layout);

		m_Shader.Bind();
		for (size_t i = 0; i < 2; i++)
		{
			m_Textures[i].Bind();
		}

		int samplers[2] = { 0, 1 };
		m_Shader.SetUnifrom1iv("u_Textures", 2, samplers);




	}

	TestBatchRender::~TestBatchRender()
	{

	}


	void TestBatchRender::OnUpdate(float deltaTime)
	{
	}

	void TestBatchRender::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));


		glm::mat4 mvp = m_Proj * m_View;

		m_Shader.Bind();
		m_Shader.SetUniformMat4f("u_MVP", mvp);

		m_Renderer.Draw(m_VAO, m_IBO, m_Shader);
	}

	void TestBatchRender::OnImGuiRenderer()
	{
		// ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}