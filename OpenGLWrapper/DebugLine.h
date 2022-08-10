#pragma once
#include "Renderer.h"
class DebugLine
{
private:
	Shader* sh;
	uint ibData[2]{ 0,1 };
	IndexBuffer ib = IndexBuffer(2, ibData);
	static std::vector<DebugLine*>* lines;
	VertexArray* va = 0;
	void Draw()
	{
		float vbData[]{ start.x,start.y,end.x,end.y };
		VertexBufferLayout vbl;
		vbl.Push<float>(2);
		VertexBuffer vb(sizeof(vbData), vbData);
		delete va;
		va = new VertexArray;
		va->AddLayout(vb, vbl);

		sh->SetUniform4f("COLOR", color);
		Renderer::Draw(*va, ib, *sh, GL_LINES);
	}
public:
	glm::vec4 color = { 1,1,1,1 };
	static void RenderLines()
	{
		for (auto x : *lines)
			x->Draw();
	}
	glm::vec2 start = { 0,0 }, end = { 0,0 };
	DebugLine(glm::vec2 start, glm::vec2 end, glm::vec4 color = { 1,1,1,1 })
		:start(start), end(end), color(color)
	{
		sh = new Shader("OpenGLWrapper/Line.shader");
		lines->push_back(this);
	}
	~DebugLine()
	{
		delete sh;
		delete va;
	}
};

