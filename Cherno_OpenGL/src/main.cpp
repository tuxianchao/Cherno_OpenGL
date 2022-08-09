
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // check compile status
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* message = (char*)alloca(sizeof(char) * len); //直接栈上分配
        glGetShaderInfoLog(id, len, &len, message);
        std::cout << "Error: Compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "Shader." << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}


static unsigned int CreateShader(const std::string& vertextShader, const std::string& fragmentShader)
{

    unsigned int shaderProgram = glCreateProgram();                      // 创建shaderProgram
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertextShader);    // 编译顶点shader
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); // 编译像素shader

    glAttachShader(shaderProgram, vs); // 附加到shaderProgram
    glAttachShader(shaderProgram, fs); // 附加到shaderProgram
    glLinkProgram(shaderProgram);      // 链接
    glValidateProgram(shaderProgram);  // https://docs.gl/gl4/glValidateProgram
    glDeleteShader(vs);
    glDeleteShader(fs);

    // check link status
    int success;
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        int len;
        glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &len);
        char* message = (char*)alloca(sizeof(char) * len); //直接栈上分配
        glGetProgramInfoLog(shaderProgram, len, &len, message);
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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


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

    float positions[6] = {
       -0.5f, -0.5f,
       0.0f, 0.5f,
       0.5f, -0.5f,
    };

    // 定义和传递数据给OpenGL
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);


    // 启用顶点属性,并且设置顶点的数据布局
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制
        glDrawArrays(GL_TRIANGLES, 0, 3);

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