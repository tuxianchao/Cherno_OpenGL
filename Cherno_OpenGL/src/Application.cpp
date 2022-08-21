
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


	// ��������
	// 1. ����vao: glGenVertexArrays���ɶ������飬glBindVertexArray�󶨶������飬glEnableVertexAttribArray����ָ���Ķ������ԣ�glVertexAttribPointer���ò��֣���������
	// 2. ����vbo: glGenBuffers����buffer��glBindBuffer��buffer��glBufferData��CPU�������ݵ�GPU
	// 3. ����ibo: glGenBuffers����buffer��glBindBuffer��buffer��glBufferData��cpu�������ݵ�GPU
	// 4. ����shader(����vs������ps��Ȼ�����ӳ�һ��shaderProgram)
	// 5. ʹ��shaderprogram�� glUseProgram�ƶ�shader
	// 6. ����uniform���ݣ�glUniform4f���ݸ�ָ����shader
	// 6. ��������vao����vbo����



	VertexArray va; // ���������������,���ò��ֺʹ���ߴ�
	VertexBuffer vb(positions, sizeof(float) * 4 * 2);
	VertexBufferLayout layout;
	layout.Push<float>(2);// �����������֣�����������float
	va.AddBuffer(vb, layout); // ���һ��vertexBuffer������ָ������

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