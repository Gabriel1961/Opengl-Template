#pragma once
#include <Common.h>
#include <Renderer.h>
#include <Transform.h>
#include <string>
static const char* DEFAULT_SHADER = "OpenGLWrapper/Quad.shader";
static uint ibData[6]{ 0,1,2, 2,3,0 };
static float vertData[16]
{
	-1,-1, 0,0,
	1,-1,  1,0,
	1,1,   1,1,
	-1,1,  0,1,
};
class Quad
{
protected:

	void _Render()
	{
		if (useTex)
		{
			shader->SetUniform1i("samp", texSlot);
			shader->SetUniform1i("useTex", useTex);
		}
		this->shader->SetUniform4f("color", color);
		this->shader->SetUniformMat4f("transform", transform.GetMatrix());
		Renderer::Draw(*va, *ib, *shader, GL_TRIANGLES);
	}

public:
	VertexArray* va;
	IndexBuffer* ib;
	VertexBuffer* vb;
	Shader* shader;
	TransformGroup transform;
	glm::vec4 color = glm::vec4(1);
	int texSlot = 0;
	bool useTex = false;

	Quad(bool useTex, int texSlot, std::string shader = DEFAULT_SHADER) : useTex(useTex), texSlot(texSlot)
	{
		vb = new VertexBuffer(sizeof(vertData), vertData);
		ib = new IndexBuffer(6, ibData, GL_UNSIGNED_INT);
		VertexBufferLayout vbl;
		vbl.Push<float>(2);
		vbl.Push<float>(2);
		va = new VertexArray();
		va->AddLayout(*vb, vbl);
		this->shader = new Shader(shader);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="color"></param>
	/// <param name="shader">Custom shader can be made 
	/// Req Vertex Shader: uniform mat4 transform; and applying it on vertexCoords
	/// Req Fragment Shader: uniform vec4 color; and applying it to outColor
	/// </param>
	Quad(glm::vec4 color = glm::vec4(1), std::string shader = DEFAULT_SHADER)
		:color(color)
	{
		vb = new VertexBuffer(sizeof(vertData), vertData);
		ib = new IndexBuffer(6, ibData, GL_UNSIGNED_INT);
		VertexBufferLayout vbl;
		vbl.Push<float>(2);
		vbl.Push<float>(2);
		va = new VertexArray();
		va->AddLayout(*vb, vbl);
		this->shader = new Shader(shader);
	}

	virtual void Render()
	{
		_Render();
	}

	~Quad()
	{
		delete va;
		delete ib;
		delete vb;
		delete shader;
	}
};
