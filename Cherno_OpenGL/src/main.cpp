
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLCheckError(#x, __FILE__, __LINE__ ))


static void  GLClearError()
{
	// 把错误信息取完
	while (glGetError() != GL_NO_ERROR);
}


static bool GLCheckError(const char* function, const char* file, int line)
{

	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL error] " << error << " " << function << " " << file << " " << line << std::endl;
		return false;
	}
	return true;
}

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filePath)
{

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FGRAGMENT = 1
	};

	std::fstream stream(filePath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				// set mode to vertex
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// set mode to fragment
				type = ShaderType::FGRAGMENT;
			}
		}
		else
		{
			ss[int(type)] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	// check compile status
	int success;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		int len;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
		char* message = (char*)alloca(sizeof(char) * len); //直接栈上分配
		GLCall(glGetShaderInfoLog(id, len, &len, message));
		std::cout << "Error: Compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "Shader." << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}


static unsigned int CreateShader(const std::string& vertextShader, const std::string& fragmentShader)
{

	GLCall(unsigned int shaderProgram = glCreateProgram());                      // 创建shaderProgram
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertextShader);    // 编译顶点shader
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); // 编译像素shader

	GLCall(glAttachShader(shaderProgram, vs)); // 附加到shaderProgram
	GLCall(glAttachShader(shaderProgram, fs)); // 附加到shaderProgram
	GLCall(glLinkProgram(shaderProgram));      // 链接
	GLCall(glValidateProgram(shaderProgram));  // https://docs.gl/gl4/glValidateProgram
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	// check link status
	int success;
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		int len;
		GLCall(glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &len));
		char* message = (char*)alloca(sizeof(char) * len); //直接栈上分配
		GLCall(glGetProgramInfoLog(shaderProgram, len, &len, message));
		std::cout << "Error:Link Shader." << std::endl;
		std::cout << message << std::endl;
		return 0;
	}
	std::cout << "create shader success" << std::endl;

	return shaderProgram;
}

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

	// create vao
	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));


	// create vertex buffer and copy data
	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, positions, GL_STATIC_DRAW));

	// define vertext layout
	//  会把0位置的顶点属性和顶点数组链接在一起
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	// create index buffer
	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 2 * 3, indices, GL_STATIC_DRAW));


	// create & user shader
	ShaderProgramSource shaderSource = parseShader("res/shaders/Basic.shader");
	std::cout << shaderSource.VertexSource << std::endl;
	std::cout << shaderSource.FragmentSource << std::endl;
	unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
	GLCall(glUseProgram(shader));

	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	float colorR = 0.8f;
	float colorG = 0.3f;
	float colorB = 0.8f;
	float colorAlpha = 1.0f;

	// clear
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	float incr = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		glUseProgram(shader);
		GLCall(glUniform4f(location, colorR, colorG, colorB, colorAlpha));

		// 指定一个vao来接受顶点属性的东西，代替掉使用默认的
		glBindVertexArray(vao);
		// ibo 索引缓冲
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		// Draw  triangle
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		if (colorR > 1.0f) {
			incr = -0.05f;
		}
		else if(colorR < 0.0f)
		{
			incr = 0.05f;
		}

		colorR += incr;

		// glBegin(GL_TRIANGLES);
		// glVertex2f(-0.5f, -0.5f);
		// glVertex2f(0.5f, -0.5f);
		// glVertex2f(0.0f, 0.5f);
		// glEnd();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}