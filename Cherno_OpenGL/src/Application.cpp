
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
	window = glfwCreateWindow(900, 600, "Hello World", NULL, NULL);
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

	float positions[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 100.0f, // 0
		 0.5f, -0.5f, 1.0f, 0.0f, 100.0f, // 1
		 0.5f,  0.5f, 1.0f, 1.0f, 100.0f, // 2
		-0.5f,  0.5f, 0.0f, 1.0f, 100.0f  // 3
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
	VertexBuffer vb(positions, sizeof(float) * 4 * 5);
	VertexBufferLayout layout;
	layout.Push<float>(2);// �����������֣�����������float
	layout.Push<float>(2);
	layout.Push<float>(1);

	va.AddBuffer(vb, layout); // ���һ��vertexBuffer������ָ������

	IndexBuffer ib(indices, 2 * 3);

	Shader shader("res/shaders/Basic.shader");
	// Texture texture("res/textures/phone.png");
	Texture texture("res/textures/gold-dollar.png");
	texture.Bind();

	shader.Bind();
	shader.SetUniform1i("u_Texture", 0);

	Renderer renderer;
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
