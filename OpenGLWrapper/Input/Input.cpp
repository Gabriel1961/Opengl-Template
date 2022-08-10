#include "Input.h"
double Input::Mouse::Y = 0, Input::Mouse::X = 0, Input::Mouse::PrevX = 0, Input::Mouse::PrevY = 0, Input::Mouse::SpeedX = 0, Input::Mouse::SpeedY = 0;
bool Input::Mouse::LeftPressed;
bool Input::Mouse::RightPressed;
Action<double, double> Input::Mouse::MouseMove;
Action<double, double> Input::Mouse::MouseScroll;
Action<int, int> Input::Keyboard::KeyDown;
void Input::Keyboard::KeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	KeyDown.Call(key, action);
}

void Input::Init(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwSetCursorPosCallback(window, Mouse::PositionCallBack);
	glfwSetMouseButtonCallback(window, Mouse::ButtonCallBack);
	glfwSetKeyCallback(window, Keyboard::KeyPressCallback);
	glfwSetScrollCallback(window, Mouse::ScrollCallBack);
}

void Input::Mouse::ButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) LeftPressed = true;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) LeftPressed = false;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) RightPressed = true;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) RightPressed= false;
}

void Input::Mouse::PositionCallBack(GLFWwindow* window, const double Xpos, const double Ypos)
{
	PrevY = Y;
	PrevX = X;
	int Window_Height, Window_Width;
	glfwGetWindowSize(window,&Window_Height, &Window_Width);
	Y = -(Ypos / (Window_Height / 2.) - 1);
	X = Xpos / (Window_Width / 2.) - 1;
	SpeedX = X - PrevX;
	SpeedY = Y - PrevY;
	MouseMove.Call(SpeedX, SpeedY);
}

void Input::Mouse::ScrollCallBack(GLFWwindow* window, double offsetX, double offsetY)
{
	MouseScroll(offsetX, offsetY);
}
