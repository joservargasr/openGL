#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const float* data,unsigned int size)
{
	GLCall(glGenBuffers(1, &uiRendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, uiRendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER,size,data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &uiRendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, uiRendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER,0));
}
