 #include "VertexArray.h"
#include "VertexBufferLayout.h"
#define INT2VOIDP(i) (void*)(uintptr_t)(i)
VertexArray::VertexArray()
{
	gc(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	gc(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddLayout(VertexBuffer& _vb, VertexBufferLayout& _vbl)
{
	Bind();
	_vb.Bind();
	const std::vector<Attribute> elements = _vbl.GetAttribArray();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const Attribute element = elements[i];
		gc(glEnableVertexAttribArray(i));
		gc(glVertexAttribPointer(i, element.m_Count, element.m_Type, element.m_Normalized,
			_vbl.GetStride(), INT2VOIDP(offset)));
		offset += element.m_Count * Attribute::GetTypeSize(element.m_Type);
	}
}

void VertexArray::Bind() const
{
	gc(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	gc(glBindVertexArray(0));
}

