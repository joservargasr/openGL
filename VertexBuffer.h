#pragma once

class VertexBuffer {
private:
	unsigned int uiRendererID;
public:
	VertexBuffer(const float* data,unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};