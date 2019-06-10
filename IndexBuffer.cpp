#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
	GLCall(glGenBuffers(1, &uiRendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiRendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,count*sizeof(unsigned int),data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &uiRendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiRendererID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0));
}
