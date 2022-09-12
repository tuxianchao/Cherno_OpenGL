
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "test/TestClearColor.h"
#include "test/TestSwitchTexture.h"
#include "test/TestTriangle.h"
#include "test/TestMultipleObjects.h"

void KeyCallbak(GLFWwindow* window, int key, int scancode, int action, int mods);




int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "960 * 540", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyCallbak);

	glfwSwapInterval(1); // Enable vsync

	// init GLEW
	glewExperimental = true;// need for core profile
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// Problem: glewInit failed
		std::cout << "failed to init glew" << glewGetErrorString(err) << std::endl;
		return 1;
	}

	std::cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GL_VENDOR: " << glGetString(GL_VENDOR) << std::endl;


	Renderer renderer;
	// 混合
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


	// imgui
	ImGui::CreateContext();
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	const char* glsl_version = "#version 130";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	int currentSelection = -1;
	int radioSelection = 0;
	Test::Test *testCase = nullptr;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			static float f = 0.0f;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::RadioButton("1.TestClearColor", &radioSelection, 0);
			ImGui::RadioButton("2.TestSwitchTexture", &radioSelection, 1);
			ImGui::RadioButton("3.TestTriangle", &radioSelection, 2);
			ImGui::RadioButton("4.TestMultipleObjects", &radioSelection, 3);
			// ImGui::SameLine();
		}

		if (currentSelection != radioSelection)
		{
			switch (radioSelection)
			{
			case 0:
				delete testCase;
				testCase = new Test::TestClearColor();
				break;
			case 1:
				delete testCase;
				testCase = new Test::TestSwitchTexture("res/textures/phone.png", "res/textures/gold-dollar.png");
				break;
			case 2:
				delete testCase;
				testCase = new Test::TestTriangle();
				break;
			case 3:
				delete testCase;
				testCase = new Test::TestMultipleObjects();
				break;
			}
			currentSelection = radioSelection;
		}

		if(testCase != nullptr)
		{
			testCase->OnUpdate(0.0f);
			testCase->OnRender();
			testCase->OnImGuiRenderer();
		}

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// GLCall(glDeleteVertexArrays(1, &vao));
	glfwTerminate();
	return 0;
}

/*
 * 按键回调
 */
void KeyCallbak(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << "[KeyCallback]" << " key:" << key << " scancode: " << scancode << " action: " << action << " mods: " << mods << std::endl;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}