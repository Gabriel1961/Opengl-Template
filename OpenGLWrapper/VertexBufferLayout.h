#pragma once
#include <vector>
#include <GLEW/glew.h>
#include "Common.h"
struct Attribute
{
	unsigned int m_Type;
	unsigned int m_Count;
	unsigned char m_Normalized;
	static unsigned int GetTypeSize(unsigned int _Type)
	{
		switch (_Type) {
		case GL_FLOAT:		   return 4;
		case GL_UNSIGNED_INT:  return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
	}
};
class VertexBufferLayout
{
private:
	std::vector <Attribute> m_Attributes;
	unsigned int m_Stride;
public:
	template<typename T>
	void Push(unsigned int _Count) { ASSERT(false); }

	template<>
	void Push<float>(unsigned int _Count)
	{
		m_Attributes.push_back({ GL_FLOAT,_Count,false });
		m_Stride += _Count * sizeof(float);
	}
	template<>
	void Push<unsigned int>(unsigned int _Count)
	{
		m_Attributes.push_back({ GL_UNSIGNED_INT,_Count,false });
		m_Stride += _Count * sizeof(int);
	}
	template<>
	void Push<unsigned char>(unsigned int _Count)
	{
		m_Attributes.push_back({ GL_UNSIGNED_BYTE,_Count,false });
		m_Stride += _Count * sizeof(char);
	}
	const std::vector<Attribute>& GetAttribArray() { return m_Attributes; }
	const int GetStride() { return m_Stride; }
};