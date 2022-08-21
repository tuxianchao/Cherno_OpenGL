#include "Shader.h"
#include "Debug.h"


Shader::Shader(const std::string &filePath): m_filePath(filePath), m_RenderID(0)
{

	ShaderProgramSource shaderSource = ParseShader(m_filePath);
	std::cout << shaderSource.VertexSource << std::endl;
	std::cout << shaderSource.FragmentSource << std::endl;
	m_RenderID = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
}
Shader::~Shader()
{

	GLCall(glDeleteShader(m_RenderID));

}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RenderID));
}
void Shader::UnBind() const
{

	GLCall(glUseProgram(0));

}

bool Shader::CompileShader()
{
	return true;
}



ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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


unsigned int Shader::CreateShader(const std::string& vertextShader, const std::string& fragmentShader)
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



// set Uniforms
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{

	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));

}


void Shader::SetUniform1i(const std::string& name, unsigned int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));

}
unsigned int Shader::GetUniformLocation(const std::string& name)
{


	if (m_UniformLocaltionCache.find(name) != m_UniformLocaltionCache.end())
	{
		return m_UniformLocaltionCache[name];
	}

	GLCall(unsigned int location = glGetUniformLocation(m_RenderID, name.c_str()));

	// ASSERT(location != -1);
	if (location == -1)
	{
		std::cout << "warning shader unifrom" << name << "does't exist." << std::endl;
	}
	else
	{
		m_UniformLocaltionCache[name] = location;
	}
	return location;
}