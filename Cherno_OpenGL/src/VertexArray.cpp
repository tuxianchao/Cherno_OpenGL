#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	// 在OpenGL生成顶点数组对象
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}
VertexArray::~VertexArray()
{
	// Clean
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind(); // bind VAO
	vb.Bind(); // bind VBO

	// 遍历有多少个布局，依次激活
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for(unsigned int i = 0 ; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

}


void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unBind() const
{
	// bind成0自然也就解开了
	GLCall(glBindVertexArray(0));
}