
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
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

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

	float positions[12] = {
	   -0.5f,   -0.5f,  // 0
	   0.5f,    -0.5f,   // 1
	   0.5f,    0.5f,  // 2
	   -0.5f,   0.5f    // 3
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
	VertexBuffer vb(positions, sizeof(float) * 4 * 2);
	VertexBufferLayout layout;
	layout.Push<float>(2);// 设置两个布局，数据类型是float
	va.AddBuffer(vb, layout); // 添加一个vertexBuffer，并且指明布局

	IndexBuffer ib(indices, 2 * 3);

	Shader shader("res/shaders/Basic.shader");


	float colorR = 0.8f;
	float colorG = 0.3f;
	float colorB = 0.8f;
	float colorAlpha = 1.0f;
	shader.Bind();
	shader.SetUniform4f("u_Color", colorR, colorG, colorB, colorAlpha);
	shader.UnBind();

	Renderer renderer;


	float incr = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		shader.Bind();
		shader.SetUniform4f("u_Color", colorR, colorG, colorB, colorAlpha);
		renderer.Draw(va, ib, shader);

		if (colorR > 1.0f) {
			incr = -0.05f;
		}
		else if (colorR < 0.0f)
		{
			incr = 0.05f;
		}
		colorR += incr;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// GLCall(glDeleteVertexArrays(1, &vao));
	glfwTerminate();
	return 0;
}