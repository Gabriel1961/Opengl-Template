#pragma once
#define GL_STATIC_DRAW 0x88E4

class VertexBuffer
{
private:
	unsigned int m_RendererID;
	int m_Size;
	int m_OffsetSubData;
public:
	VertexBuffer() {}
	VertexBuffer(int _Size,void* _VertexData= nullptr ,int _DrawMode = GL_STATIC_DRAW);
	~VertexBuffer();
	void AddData(void* _VertexData, int _Size);
	void Bind()const;
	void Unbind()const;
};