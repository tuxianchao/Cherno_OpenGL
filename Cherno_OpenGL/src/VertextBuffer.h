#pragma once


class VertextBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertextBuffer(const void* data, unsigned int size);
	~VertextBuffer();

	void Bind() const;
	void UnBind() const;

};