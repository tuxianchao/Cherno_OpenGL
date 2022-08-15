#include "Shader.h"

#include <string>

class Shader
{
private:
public:
	Shader(const std::string& filename);
	~Shader();

	void Bind() const;
	void UnBind() const;
};