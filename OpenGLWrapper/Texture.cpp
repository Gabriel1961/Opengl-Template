#include "Texture.h"
#include "Common.h"
#include "Shader.h"
#include "vendor/stb_image/stb_image.h"

//#define TEXTURE_DEBUG
#ifdef TEXTURE_DEBUG
#define DBG(x) x
#else
#define DBG(x)
#endif // DEBUG

static std::unordered_map<std::string, TextureRef> textures;

using std::cout;
/// <summary>
/// format GL_RGBA8
/// </summary>
/// <param name="_FilePath"></param>
Texture::Texture(const std::string& _FilePath)
{
	if (textures.find(_FilePath) != textures.end())
	{
		auto& ref = textures[_FilePath];
		*this = ref.ref; // Ref count increased in copy constructor
		return;
	}
	else
	{ // Do normal init 
		m_FilePath = _FilePath;
		DBG(std::cout << "Loaded " << m_FilePath << " " << m_RendererID << endl);
		if (_FilePath.find(".bmp") == _FilePath.npos)
			stbi_set_flip_vertically_on_load(1);
		uchar* tempBuf = stbi_load(_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
		if (!tempBuf ) {
			cout << "Texture file not found\n";
			assert(false);
		}
		gc(glGenTextures(1, &m_RendererID));
		gc(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		gc(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempBuf));
		gc(glBindTexture(GL_TEXTURE_2D, 0));
		if (tempBuf)
		{
			stbi_image_free(tempBuf);
		}

		textures[_FilePath].refCount = 0;
		textures[_FilePath].ref = *this;
	}
}

Texture::Texture(const Texture& o)
	: m_Height(o.m_Height), m_Width(o.m_Width), m_RendererID(o.m_RendererID), m_BPP(o.m_BPP), m_FilePath(o.m_FilePath),type(o.type)
{
	textures[m_FilePath].refCount++;
	DBG(cout << "Cache hit " << m_FilePath << " " << m_RendererID << endl);
}

/// <summary>
/// Generate arbitrary size buffer
/// Format : GL_RGBA32F
/// </summary>
Texture::Texture(uint heigth, uint witdth, GLenum antiAliasing, GLenum format)
	:m_Height(heigth), m_Width(witdth)
{
	gc(glActiveTexture(GL_TEXTURE0));
	gc(glGenTextures(1, &m_RendererID));
	gc(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	gc(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, witdth, heigth, 0, GL_RGBA, GL_FLOAT,
		NULL));
	gc(glBindImageTexture(0, m_RendererID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F));
}
/// <summary>
/// Format ARGB
/// </summary>
Texture::Texture(ARGBColor* buffer,GLFWwindow* w, GLenum antiAliasing)
{
	glfwGetWindowSize(w,&m_Height, &m_Width);
	
	uchar* tempBuffer = (unsigned char*)buffer;
	gc(glGenTextures(1, &m_RendererID));
	gc(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	gc(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempBuffer));
	gc(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture()
{
	gc(glDeleteTextures(1, &m_RendererID));
}

Texture& Texture::operator=(const Texture& o)
{
	if (m_FilePath.empty() == false)
		TextureRef::Decrement(m_FilePath);
	m_Height = (o.m_Height), m_Width = (o.m_Width), m_RendererID = (o.m_RendererID), m_BPP = (o.m_BPP), m_FilePath = (o.m_FilePath), type = (o.type);
	textures[m_FilePath].refCount++;
	DBG(cout << "Cache hit " << m_FilePath << " " << m_RendererID << endl);
	return *this;
}

Texture::Texture(Texture&& o) noexcept
{
	m_Height = o.m_Height;
	m_Width = o.m_Width;
	m_BPP = o.m_BPP;
	m_RendererID = o.m_RendererID;

	o.m_RendererID = 0;
}

void Texture::SetScalingMode(GLenum wrapS, GLenum wrapT)
{
	Bind();
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));
	gc(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT));
}

void Texture::Bind(unsigned int slot) const
{
	gc(glActiveTexture(GL_TEXTURE0 + slot));
	gc(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	gc(glBindTexture(GL_TEXTURE_2D, 0));
}

inline void TextureRef::Decrement(const std::string& path)
{
	/// <summary>
	/// TODO FIX THIS GARBAGE
	/// </summary>
	/// <param name="path"></param>
	/// 
	return;
	std::unordered_map<std::string, TextureRef>::iterator xref;
	xref = textures.find(path);
	if (--(*xref).second.refCount == 0)
	{
		gc(glDeleteTextures(1, &(*xref).second.ref.m_RendererID));
		DBG(std::cout << "Erased " << (*xref).second.ref.m_FilePath << " " << (*xref).second.ref.m_RendererID << endl);
		textures.erase(xref);
	}
}

const ivec2& DepthTexture::GetSize()const { return size; }

const uint& DepthTexture::GetRendeId()const { return renderId; }
