#include "Common.h"
#include "Shader.h"
#include "fstream"
#include "sstream"
#include "string"
#include <filesystem>
using namespace std;
std::map<int, Shader*> Shader::shaderList;
void ShaderCompilationErrorCheck(unsigned int shaderID, const std::string& name)
{
	int CompileStatus;
	gce(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &CompileStatus));
	if (CompileStatus == GL_FALSE) {
		int LogLenght;
		int Type;
		gce(glGetShaderiv(shaderID, GL_SHADER_TYPE, &Type);
		gce(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &LogLenght));
		char* Message = (char*)alloca(sizeof(char) * LogLenght));
		gce(glGetShaderInfoLog(shaderID, LogLenght, &LogLenght, Message));
		std::cout << "[Shader Error] Failed to compile " << (Type == GL_VERTEX_SHADER ? "Vertex Shader - " : "Fragment Shader - ") << " Name : " << name << "; " << Message << '\n';
	}
}
void ShaderLinkErrorCheck(unsigned int program, const std::string& name)
{
	int ProgramLink;
	gce(glGetProgramiv(program, GL_LINK_STATUS, &ProgramLink));
	if (ProgramLink == GL_FALSE)
	{
		char Message[1024];
		int size = 1024;
		glGetProgramInfoLog(program, size, &size, Message);
		std::cout << "Failed to link program: Name:" << name << "; " << Message << endl;
	}
}
unsigned int CreateProgam(std::string filepath)
{
	gce(unsigned int program = glCreateProgram());
	gce(unsigned int vs = glCreateShader(GL_VERTEX_SHADER));
	gce(unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER));

	enum class e_Shader {
		none = -1, vs = 1, fs = 2
	}ShaderType;
	ShaderType = e_Shader::none;
	std::stringstream ssvs, ssfs;
	std::ifstream fin(filepath);
	if (!fin.is_open()) { std::cout << "[Error]: Fatal error shader : " << filepath << " not found!"; ASSERT(false); }
	std::string line;
	while (std::getline(fin, line)) {
		if (line == "#Vertex Shader")
		{
			ShaderType = e_Shader::vs; continue;
		}
		if (line == "#Fragment Shader")
		{
			ShaderType = e_Shader::fs; continue;
		}
		if (ShaderType == e_Shader::vs)
		{
			ssvs << line << '\n';
		}
		else if (ShaderType == e_Shader::fs)
		{
			ssfs << line << '\n';
		}
	}
	std::string vs_source = ssvs.str();
	std::string fs_source = ssfs.str();
	const char* vs_src = vs_source.c_str();
	const char* fs_src = fs_source.c_str();
	gce(glShaderSource(vs, 1, &vs_src, nullptr));
	gce(glShaderSource(fs, 1, &fs_src, nullptr));
	gce(glCompileShader(vs));
	gce(glCompileShader(fs));

	ShaderCompilationErrorCheck(vs, filepath);
	ShaderCompilationErrorCheck(fs, filepath);

	gce(glAttachShader(program, vs));
	gce(glAttachShader(program, fs));
	gce(glLinkProgram(program));

	ShaderLinkErrorCheck(program, filepath);

	gce(glValidateProgram(program));

	gce(glDeleteShader(vs));
	gce(glDeleteShader(fs));
	return program;
}

Shader::Shader(const std::string& filepath) : m_RendererID(0), m_FilePath(filepath)
{
	m_RendererID = CreateProgam(filepath);
	gc(glUseProgram(m_RendererID));
	programID = m_RendererID;
	shaderList.insert({ m_RendererID, this });
}
/// returns if it succeded
bool Shader::Recompile()
{
	uint newId=0;
	try {
		newId = CreateProgam(m_FilePath);
		gce(glUseProgram(newId));
	}
	catch (const exception& ex) {
		return 0;
	}
	if (newId == 0)
		return 0;

	gc(glDeleteProgram(m_RendererID));
	programID = m_RendererID = newId;
	for (auto x : uniformBlocks)
		x.second->BindUniformBlock(m_RendererID);
	m_UniformLocationCache.clear();
	return 1;
}

Shader::~Shader()
{
	shaderList.erase(m_RendererID);
	for (auto x : uniformBlocks)
		delete x.second;
	gc(glDeleteProgram(m_RendererID));
}

void Shader::Bind() 
{
	gc(glUseProgram(m_RendererID));
	ApplyUniformBlocks();
}

void Shader::Unbind() 
{
	gc(glUseProgram(0));
}
void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
	Shader::Bind(); // to prevent stack overflow if bind is overriden
	gc(glUniform2f(GetUniformLocation(name), v0, v1));
}
void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	Shader::Bind();
	gc(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}
void Shader::SetUniform3f(const std::string& name, const glm::vec3& vec)
{
	Shader::Bind();
	gc(glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z));
}
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	Shader::Bind();
	gc(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
void Shader::SetUniform4f(const std::string& name, const glm::vec4& vec)
{
	Shader::Bind();
	gc(glUniform4f(GetUniformLocation(name), vec[0], vec[1], vec[2], vec[3]));
}
void Shader::SetUniform1f(const std::string& name, float v0)
{
	Shader::Bind();
	gc(glUniform1f(GetUniformLocation(name), v0));
}
void Shader::SetUniform1i(const std::string& name, int val)
{
	Shader::Bind();
	gc(glUniform1i(GetUniformLocation(name), val));
}
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	Shader::Bind();
	gc(glUniformMatrix4fv(GetUniformLocation(name), 1, false, &matrix[0][0]));
}
void Shader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
	Shader::Bind();
	gc(glUniformMatrix3fv(GetUniformLocation(name), 1, false, &matrix[0][0]));
}
void Shader::SetUniformMat2f(const std::string& name, const glm::mat2& matrix)
{
	Shader::Bind();
	gc(glUniformMatrix2fv(GetUniformLocation(name), 1, false, &matrix[0][0]));
}

void Shader::ApplyUniformBlocks() const
{
	for (auto x : uniformBlocks)
		x.second->BindUniformBlock(programID);
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	gc(int location = glGetUniformLocation(programID, name.c_str()));
	if (location == -1)
		std::cout << "[Warning]: uniform " << name << " does not exist!\n";
	m_UniformLocationCache[name] = location;
	return location;
}
