#pragma once
#include "Common.h"
#include <string>
#include <vector>
class Shader;
/// <summary>
/// Base class for UniformBlockList and UniformBlockItem
/// </summary>
class UniformBlock
{
public:
	uint ID;
	virtual void BindUniformBlock(uint shaderProgramId) const = 0;
	virtual void UpdateBlock()const = 0;
};

static uint GetNewBindingIdx()
{
	static uint nextIdx = 0;
	nextIdx++;
	return nextIdx - 1;
}

/// <summary>
/// This class wraps the ubo and it updates it every time it needs to render
/// </summary>
/// <typeparam name="T">Type of the data that is inside the vector</typeparam>
template<typename T>
class UniformBlockList : public UniformBlock
{
public:
	uint bindingIdx;
	std::string name;
	std::vector<T>& objects;
	//Constructors
	UniformBlockList(const std::string& uboName, std::vector<T>& objects) :data(objects), name(uboName), objects(objects)
	{
		bindingIdx = GetNewBindingIdx();
		gc(glGenBuffers(1, &ID));
		gc(glBindBuffer(GL_UNIFORM_BUFFER, ID));
		gc(glBufferData(GL_UNIFORM_BUFFER, sizeof(T) * objects.size(), &objects[0], GL_DYNAMIC_DRAW));
		gc(glBindBufferBase(GL_UNIFORM_BUFFER, bindingIdx, ID));
	}

	//Methods
	void BindUniformBlock(uint shaderProgramId) const override
	{
		gc(int uniformIdx = glGetUniformBlockIndex(shaderProgramId, name.c_str())); // TODO add hashtable to shader to store  
		gc(glUniformBlockBinding(shaderProgramId, uniformIdx, bindingIdx));
	}


	void UpdateBlock() const override
	{
		gc(glBindBuffer(GL_UNIFORM_BUFFER, ID));
		gc(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T) * objects.size(), &objects[0]));
		gc(glBindBufferBase(GL_UNIFORM_BUFFER, bindingIdx, ID));
	}

	//Getters
	uint GetId() { return ID; }
	uint GetBindingIdx() { return bindingIdx; }

	//Fields
	std::vector<T>& data;
};

class EditUBOScope
{
public:
	const UniformBlock& block;
	void* ptr;
	EditUBOScope(const UniformBlock& block) : block(block)
	{
		ptr = glMapNamedBuffer(block.ID, GL_READ_WRITE_ARB);
	}
	~EditUBOScope()
	{
		glUnmapNamedBuffer(block.ID);
	}
};

template<typename T>
class SSBO_ObjectList
{
public:
	uint id;
	uint binding; // an index used in the shader to access this specific buffer
	AlignedVector<T,4> data;

	SSBO_ObjectList()
	{}

	SSBO_ObjectList(const std::vector<T>& objs,GLenum usage = GL_DYNAMIC_COPY, uint binding = 0) : binding(binding)
	{
		for (auto& c : objs)
			data.push_back(c);
		gc(glGenBuffers(1, &id));
		gc(glBindBuffer(GL_SHADER_STORAGE_BUFFER, id));
		gc(glBufferData(GL_SHADER_STORAGE_BUFFER, data.SizeInBytes(), (const void*)&data[0], GL_DYNAMIC_COPY));
	}

	void Bind() const 
	{
		gc(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, id));
	}

	void Unbind() const 
	{
		gc(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
	}
	void operator=(SSBO_ObjectList&& o)
	{
		id = o.id;
		binding = o.binding;
		data = std::move(o.data);
		o.id = 0;
	}
	SSBO_ObjectList(SSBO_ObjectList&& o)
	{
		id = o.id;
		binding = o.binding;
		data = std::move(o.data);
		o.id = 0;
	}

	~SSBO_ObjectList()
	{
		gc(glDeleteBuffers(1, &id));
	}
};

