#pragma once
#include "Common.h"
#include "Texture.h"
class FrameBuffer
{
private:
	uint renderId = 0;
public:
	DepthTexture* depthTexture=0;
	FrameBuffer()
	{
		gc(glGenBuffers(1, &renderId));
	}

	void AttachDepthTexture(DepthTexture* tex,GLFWwindow * w)
	{
		delete depthTexture;
		depthTexture = tex;
		Bind();
		gc(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->GetRendeId(), 0));
		
		gc(glReadBuffer(GL_NONE)); // explicitly tell OpenGL not to render any color data
		gc(glDrawBuffer(GL_NONE));

		gc(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		
		Unbind(w);
	}

	void Bind()
	{
		gc(glViewport(0, 0, depthTexture->GetSize().x, depthTexture->GetSize().y));
		gc(glBindFramebuffer(GL_FRAMEBUFFER,renderId));
	}
	void Clear()
	{
		gc(glClear(GL_DEPTH_BUFFER_BIT));
	}

	void Unbind(GLFWwindow* w)
	{
		int Window_Width, Window_Height;
		glfwGetWindowSize(w, &Window_Width, &Window_Height);
		
		gc(glViewport(0, 0, Window_Width, Window_Height));
		gc(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	~FrameBuffer()
	{
		delete depthTexture;
		gc(glDeleteBuffers(1, &renderId));
	}
};

