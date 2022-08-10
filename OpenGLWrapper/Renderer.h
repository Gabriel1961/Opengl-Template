#pragma once
#include "Shader.h"
#include "Indexbuffer.h"
#include "VertexArray.h"
#include "Texture.h"

class Renderer
{
public:
	static void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader, unsigned int DrawMode = GL_TRIANGLES);
	static void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader, const Texture& tex, std::pair<std::string, int> SamplerNameAndSlot, unsigned int DrawMode = GL_TRIANGLES);
	static void DrawArray(const VertexArray& va, Shader& shader, uint count, GLenum drawMode);
	static inline void Clear(float r = 0, float g = 0, float b = 0, float a = 1) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};