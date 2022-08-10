#pragma once
#include "../../OpenGLWrapper/Common.h"
#include <Action/Action.h>

using namespace EventSystem;
class Input
{
public:
	class Mouse
	{
	public:
		static bool LeftPressed,RightPressed;
		static Action<double, double> MouseMove, MouseScroll;
		static double Y, X, PrevX, PrevY, SpeedX, SpeedY;
		static void ButtonCallBack(GLFWwindow* window, int button, int action, int mods);

		static void PositionCallBack(GLFWwindow* window, const double Xpos, const double Ypos);

		static void ScrollCallBack(GLFWwindow* window, double offsetX, double offsetY);
	};
	class Keyboard
	{
	public:
		/// <summary>
		/// Activates on keypress 
		/// int key, int action
		/// </summary>
		static Action<int, int> KeyDown;
		static void KeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};


	static void Init(GLFWwindow* window);
};
