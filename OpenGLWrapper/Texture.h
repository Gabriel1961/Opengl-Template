#pragma once
#include <string>
#include "Common.h"
#include <unordered_map>
using namespace glm;
class Texture
{
private:
	int m_Height, m_Width, m_BPP;
public:
	std::string m_FilePath = "";
	unsigned int m_RendererID;

	std::string type = "texture_diffuse";

	Texture(const std::string& _FilePath);
	Texture(const Texture& o);
	Texture() {}
	Texture(uint heigth, uint witdth, GLenum antiAliasing = GL_NEAREST, GLenum format = GL_RGBA32F);
	Texture(ARGBColor* buffer, GLFWwindow* w, GLenum antiAliasing = GL_NEAREST);
	~Texture();

	Texture& operator=(const Texture& o);
	Texture(Texture&& o) noexcept;

	void SetScalingMode(GLenum wrapS, GLenum wrapT);
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	int  GetHeight()const { return m_Height; }
	int  GetWidth()const { return m_Width; }
};
struct TextureRef
{
	Texture ref;
	int refCount;
	static void Decrement(const std::string& path);
};
class DepthTexture
{
	uint renderId;
	ivec2 size;
public:
	const ivec2& GetSize()const;
	const uint& GetRendeId()const;
	DepthTexture(ivec2 size) :size(size)
	{
		gc(glGenFramebuffers(1, &renderId));

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	}
	~DepthTexture()
	{
		gc(glDeleteBuffers(1, &renderId));
	}
};