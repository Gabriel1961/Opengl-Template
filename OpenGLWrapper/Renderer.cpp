#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader, unsigned int DrawMode)
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	gc(glDrawElements(DrawMode, ib.GetCount(), ib.GetType(), nullptr));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader, const Texture& tex, std::pair<std::string, int> SamplerNameAndSlot, unsigned int DrawMode)
{
	shader.Bind();
	shader.SetUniform1i(SamplerNameAndSlot.first, SamplerNameAndSlot.second);
	tex.Bind(SamplerNameAndSlot.second);
	va.Bind();
	ib.Bind();
	gc(glDrawElements(DrawMode, ib.GetCount(), ib.GetType(), nullptr));
}

void Renderer::DrawArray(const VertexArray& va, Shader& shader, uint count, GLenum drawMode)
{
	shader.Bind();
	va.Bind();
	glDrawArrays(drawMode, 0, count);
}
