#include "IndexBuffer.h"
#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(int _Count, void* _IndexData, unsigned int _DataType/*=GL_UNSIGNED_INT*/, unsigned int _DrawMode /*= GL_STATIC_DRAW*/):m_Count(_Count),m_DataType(_DataType)
{
	gc(glGenBuffers(1, &m_RendererID));
	gc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	gc(glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetTypeSize(_DataType)*m_Count, (const void*)_IndexData, _DrawMode));
}

IndexBuffer::~IndexBuffer()
{
	gc(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	gc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	gc(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

//////

