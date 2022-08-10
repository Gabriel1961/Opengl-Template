#pragma once
#include <iostream>
#include <Common.h>
template<typename T>
void _print(const T& val)
{
	std::cout << val;
}
template<>
void _print<>(const glm::vec3& c)
{
	std::cout << c.x << " " << c.y << " " << c.z;
}
template<>
void _print<>(const glm::vec2& c)
{
	std::cout << c.x << " " << c.y;
}
template<>
void _print<>(const glm::vec4& c)
{
	std::cout << c.x << " " << c.y << " " << c.z << " " << c.w;
}

template<typename T>
void _printMat(const T& mat, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			std::cout << mat[i][j] << " ";
		}
		std::cout << "\n";
	}
}

template<>
void _print<>(const glm::mat4& mat)
{
	_printMat(mat, 4);
}

template<>
void _print<>(const glm::mat3& mat)
{
	_printMat(mat, 3);
}

template<>
void _print<>(const glm::mat2& mat)
{
	_printMat(mat, 2);
}
class Color;
template<>
void _print<>(const Color& c)
{
	std::cout << c.rgb.r << " " << c.rgb.g << " " << c.rgb.b;
}
template<>
void _print<>(const ARGBColor& c)
{
	std::cout << (int)c.r << " " << (int)c.g << " " << (int)c.b << " " << (int)c.a;
}
template<typename T, typename... Targs>
void _print(const T& value, Targs... Fargs) {
	_print(value);
	std::cout << ' ';
	_print(Fargs...);
}

#define print(...) _print(##__VA_ARGS__,'\n')
