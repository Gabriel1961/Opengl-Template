#pragma once
#include "Common.h"
#define GL_STATIC_DRAW 0x88E4
class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_DataType;
	int m_Count;
public:
	IndexBuffer() {};
	IndexBuffer(int _Count, void* _IndexData,unsigned int _DataType=GL_UNSIGNED_INT, unsigned int _DrawMode = GL_STATIC_DRAW);
	~IndexBuffer();
	void Bind()const;
	void Unbind()const;
	static unsigned int GetTypeSize(unsigned int _Type)
	{
		switch (_Type) {
		case GL_UNSIGNED_SHORT:   return 2; 
		case GL_UNSIGNED_INT:     return 4;
		case GL_UNSIGNED_BYTE:    return 1;
		}
		ASSERT(false);
	}
	unsigned int GetCount()const{ return m_Count; }
	unsigned int GetType()const { return m_DataType; }
};

