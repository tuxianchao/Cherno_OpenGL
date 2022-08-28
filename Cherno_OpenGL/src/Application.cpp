
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

	glfwSwapInterval(1);

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

	float positions[] = {
		0.0f, 0.0f, 0.0f, 0.0f, 100.0f, // 0
		200.0f, 0.0f, 1.0f, 0.0f, 100.0f, // 1
		200.0f, 200.0f, 1.0f, 1.0f, 100.0f, // 2
		0.0f, 200.0f, 0.0f, 1.0f, 100.0f  // 3
	};

	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};


	// 典型流程
	// 1. 创建vao: glGenVertexArrays生成顶点数组，glBindVertexArray绑定顶点数组，glEnableVertexAttribArray启用指定的顶点属性，glVertexAttribPointer设置布局，传递数据
	// 2. 创建vbo: glGenBuffers生成buffer，glBindBuffer绑定buffer，glBufferData从CPU拷贝数据到GPU
	// 3. 创建ibo: glGenBuffers生成buffer，glBindBuffer绑定buffer，glBufferData从cpu拷贝数据到GPU
	// 4. 创建shader(编译vs，编译ps，然后链接成一个shaderProgram)
	// 5. 使用shaderprogram： glUseProgram制定shader
	// 6. 传递uniform数据：glUniform4f传递给指定的shader
	// 6. 擦除，绑定vao，绑定vbo，画



	VertexArray va; // 创建顶点数组对象,设置布局和传入尺寸
	VertexBuffer vb(positions, sizeof(float) * 4 * 5);
	VertexBufferLayout layout;
	layout.Push<float>(2);// 设置两个布局，数据类型是float
	layout.Push<float>(2);
	layout.Push<float>(1);

	va.AddBuffer(vb, layout); // 添加一个vertexBuffer，并且指明布局

	IndexBuffer ib(indices, 2 * 3);

	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

	glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);

	glm::vec4 result = proj * vp;

	Shader shader("res/shaders/Basic.shader");
	// Texture texture("res/textures/phone.png");
	Texture texture("res/textures/gold-dollar.png");
	texture.Bind();

	shader.Bind();
	shader.SetUniform1i("u_Texture", 0);
	shader.SetUniformMat4f("u_MVP", proj);

	Renderer renderer;

	// 混合
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		shader.Bind();
		renderer.Draw(va, ib, shader);

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