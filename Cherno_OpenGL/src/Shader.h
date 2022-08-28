#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map> // hash map
#include "glm/glm.hpp"

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
	std::unordered_map<std::string, int> m_UniformLocaltionCache;

public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	// set Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, unsigned int value);
	void SetUniformMat4f(const std::string& name, const glm::mat4& value);


private:
	bool CompileShader();
	int GetUniformLocation(const std::string& name);
	unsigned int CreateShader(const std::string& vertextShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filePath);
};