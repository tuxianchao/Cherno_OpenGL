#include "VertextBuffer.h"
#include "Renderer.h"


VertextBuffer::VertextBuffer(const void* data, unsigned int size)
{

	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
VertextBuffer::~VertextBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertextBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

}
void VertextBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}