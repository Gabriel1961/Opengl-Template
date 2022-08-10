#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();
	void AddLayout(VertexBuffer& _vb, VertexBufferLayout& _vbl);
	void Bind()const;
	void Unbind()const;
};