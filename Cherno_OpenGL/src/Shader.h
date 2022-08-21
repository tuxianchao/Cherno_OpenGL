#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map> // hash±í

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

enum class ShaderType
{
	NONE = -1,
	VERTEX = 0,
	FGRAGMENT = 1
};



class Shader
{
private:
	std::string m_filePath;
	unsigned int m_RenderID;
	// cacheing for uniforms
	std::unordered_map<std::string, unsigned int> m_UniformLocaltionCache;

public:

	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	// set Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, unsigned int value);

private:

	bool CompileShader();

	unsigned int GetUniformLocation(const std::string& name);

	unsigned int CreateShader(const std::string& vertextShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filePath);
};