#pragma once
#include "Common.h"
#include <unordered_map>
#include <map>
#include <vector>
#include "UniformBlockList.h"
class Shader
{
protected:
	int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int>m_UniformLocationCache;
public:
	static std::map<int, Shader*> shaderList;
	std::unordered_map<std::string, UniformBlock*> uniformBlocks;
	int programID;
	Shader() {}
	Shader(const std::string& filepath);
	bool Recompile();
	~Shader();
	virtual void Bind();
	void Unbind();
	const std::string& GetFilePath() { return m_FilePath; }
	//Uniform
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform1i(const std::string& name, int val);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform3f(const std::string& name, const glm::vec3& vec);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4f(const std::string& name, const glm::vec4&);
	void SetUniformMat4f(const std::string& name, const glm::mat4&);
	void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
	void SetUniformMat2f(const std::string& name, const glm::mat2& matrix);

	template<typename T>
	void AddUniformBlockList(const std::string& name, std::vector<T>& val)
	{
		uniformBlocks[name] = (new UniformBlockList<T>(name, val));
	}

	void ApplyUniformBlocks() const;

	template<typename T>
	void SetUniformBlockData(const std::string& name, const T& data)
	{
		gc(auto idx = glGetUniformBlockIndex(programID, name.c_str()));
		if (idx == GL_INVALID_INDEX) {
			std::cout << "[Warning] Uniform block " << name << " not found\n";
			return;
		}
		gc(glUniformBlockBinding(programID, idx, 0));
		gc(void* ptr = glMapBuffer(GL_UNIFORM_BLOCK, GL_WRITE_ONLY));
		if (ptr == null) {
			std::cout << "[ERROR] Acces error in uniform block " << name;
			return;
		}
		memcpy(ptr, &data, sizeof(data));
		glUnmapBuffer(GL_UNIFORM_BLOCK);
	}

protected:
	int GetUniformLocation(const std::string& name);
};

class ComputeShader : public Shader
{
public:
	ComputeShader() {};
	ComputeShader(const std::string& str);
	void Dispatch(uint groups_x, uint groups_y, uint groups_z);
};
