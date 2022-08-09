
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


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

    // create vertex buffer and copy data
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, positions, GL_STATIC_DRAW);

    // define vertext layout
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    // create index buffer
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 2 * 3, indices, GL_STATIC_DRAW);


    // create & user shader
    ShaderProgramSource shaderSource = parseShader("res/shaders/Basic.shader");
    std::cout << shaderSource.VertexSource << std::endl;
    std::cout << shaderSource.FragmentSource << std::endl;
    unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw  triangle
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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