#include "VertexBuffer.h"
#include "Common.h"
VertexBuffer::VertexBuffer(int _Size, void* _VertexData , int _DrawMode ) :
m_RendererID(0),m_Size(_Size),m_OffsetSubData(0)
{
	gc(glGenBuffers(1, &m_RendererID));
	gc(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	gc(glBufferData(GL_ARRAY_BUFFER, m_Size, (const void*)_VertexData, _DrawMode));
}

VertexBuffer::~VertexBuffer()
{
	gc(glDeleteBuffers(1,&m_RendererID));
}

void VertexBuffer::AddData(void * _VertexData, int _Size)
{
	gc(glBufferSubData(GL_ARRAY_BUFFER, m_OffsetSubData, _Size, _VertexData));
	m_OffsetSubData += _Size;
}

void VertexBuffer::Bind()const
{
	gc(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind()const
{
	gc(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
