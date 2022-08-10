#pragma once
#include <Renderer.h>
#include <Common.h>
#include <Quad.h>
#include "../ScreenSize.h"
static const char* SHADER_PATH = "OpenGLWrapper/PixelDisplay.shader";


class PixelDisplay : public Quad
{
public:
	Texture* tex = 0;
	ARGBColor screen[Window_Height][Window_Width];

	PixelDisplay()
		:Quad(glm::vec4(1), SHADER_PATH)
	{

	}

	void Draw() override
	{
		delete tex;
		tex = new Texture(&screen[0][0]);
		tex->Bind();
		shader->SetUniform1i("samp", 0);
		_Render();
	}
};