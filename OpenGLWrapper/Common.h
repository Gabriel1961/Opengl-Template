#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <assert.h>
#include "AlignedVector.h"
#include <Action/Action.h>
#include <memory>
#define endl '\n'
#define pi 3.14159265359f
#define null 0
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
#define ASSERT(x) if (!(x)) assert(false)
#define gc(x) GLClearError();\
    x;\
    ASSERT(GLCheckError())
#define gce(x)GLClearError();\
    x;\
    if(GLCheckError()==0){throw exception();}
static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
template<typename T>
inline float Det(const T& a, const T& b)
{
	return a.x * b.y - a.y * b.x;
}

static bool GLCheckError()
{
	while (GLenum error = glGetError())
	{

		std::cout << "[OpenGL Error] ";
		switch (error) {
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM : An unacceptable value is specified for an enumerated argument.";
			break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_OPERATION : A numeric argument is out of range.";
			break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION : The specified operation is not allowed in the current state.";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION : The framebuffer object is not complete.";
			break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY : There is not enough memory left to execute the command.";
			break;
		case GL_STACK_UNDERFLOW:
			std::cout << "GL_STACK_UNDERFLOW : An attempt has been made to perform an operation that would cause an internal stack to underflow.";
			break;
		case GL_STACK_OVERFLOW:
			std::cout << "GL_STACK_OVERFLOW : An attempt has been made to perform an operation that would cause an internal stack to overflow.";
			break;
		default:
			std::cout << "Unrecognized error" << error;
		}
		std::cout << endl;
		return false;
	}
	return true;
}
struct ARGBColor
{
	ARGBColor() {}
	ARGBColor(uchar a, uchar r, uchar g, uchar b)
		:a(a), r(r), b(b), g(g) {}
	uchar r = 255, g = 255, b = 255, a = 255;
};

struct Color
{
	//Constructors
	Color() {}
	Color(float r, float g, float b,float a=1) :rgba(r, g, b,a) {}
	Color(glm::vec3 color) :rgba(color,1) {}
	Color(glm::vec4 color) :rgba(color) {}
	static Color FromRgb(uchar r, uchar g, uchar b)
	{
		return Color(r / 255.f, g / 255.f, b / 255.f);
	}

	//Static funcs
	static Color Black() { return Color(0, 0, 0); };
	static Color White() { return Color(1, 1, 1); };
	static Color Red() { return Color(1, 0, 0); };
	static Color Green() { return Color(0, 1, 0); };
	static Color Blue() { return Color(0, 0, 1); };
	static Color Random() {
		return Color(
			(float)rand() / (float)RAND_MAX,
			(float)rand() / (float)RAND_MAX,
			(float)rand() / (float)RAND_MAX);
	}

	//Methods
	void GammaCorrect()
	{
		rgb = sqrt(rgb);
	}

	// Operators
	Color operator*(float f)const
	{
		return this->rgb * f;
	}

	operator ARGBColor() const
	{
		return ARGBColor(255, uchar(rgb.r * 255), uchar(rgb.g * 255), uchar(rgb.b * 255));
	}

	Color operator+(const Color& c)const
	{
		return Color(rgb + c.rgb);
	}

	Color operator*(const Color& c)const
	{
		return { c.rgb * rgb };
	}

	// Public Fields
	union
	{
		glm::vec4 rgba = {1,1,1,1};
		glm::vec3 rgb;
	};
};
