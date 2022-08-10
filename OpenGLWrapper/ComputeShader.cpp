#include "Shader.h"
#include "Common.h"
#include "Shader.h"
#include "fstream"
#include "sstream"
#include "string"
#include <filesystem>
using namespace std;
void ShaderCompilationErrorCheck(unsigned int shaderID, const string& name);
void ShaderLinkErrorCheck(unsigned int shaderID, const string& name);

/// <summary>
/// Reads all text from the specified path
/// </summary>
/// <param name="path">path to the file you want to read</param>
/// <param name="out">null terminated string with the text</param>
/// <returns></returns>
void GetAllTextFromFile(const std::string& path, char*& out)
{
	std::ifstream fin(path);
	filesystem::path file(path);
	int fsize = std::filesystem::file_size(file);
	out = new char[fsize + 1];
	fin.get(out, fsize, 0);
	out[fsize] = 0;
}



/// <summary>
/// Creates a compute shader from path
/// </summary>
ComputeShader::ComputeShader(const std::string& path) :Shader()
{
	char* src;
	GetAllTextFromFile(path, src);
	gc(uint computeShader = glCreateShader(GL_COMPUTE_SHADER));
	gc(glShaderSource(computeShader, 1, &src, NULL));
	gc(glCompileShader(computeShader));

	ShaderCompilationErrorCheck(computeShader, path);

	m_RendererID = glCreateProgram();
	programID = m_RendererID;
	gc(glAttachShader(m_RendererID, computeShader));
	gc(glLinkProgram(m_RendererID));

	ShaderLinkErrorCheck(m_RendererID, path);
	glDeleteShader(computeShader);
	delete[] src;
}

void ComputeShader::Dispatch(uint groups_x, uint groups_y, uint groups_z)
{
	Bind();
	glDispatchCompute(groups_x, groups_y, groups_z);
}
