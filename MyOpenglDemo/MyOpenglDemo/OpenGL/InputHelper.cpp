#include "InputHelper.h"
#include "DisplayManager.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

std::vector<InputHelper::KeyCallback> InputHelper::keyCallbacks;
std::vector<InputHelper::CursorCallback> InputHelper::cursorCallbacks;
std::vector<InputHelper::ScrollCallback> InputHelper::scrollCallbacks;
std::vector<InputHelper::MouseCallback> InputHelper::mouseCallbacks;

void InputHelper::Init()
{
	GLFWwindow* window = DisplayManagerService::Get().GetWindow();
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
		for (auto it : keyCallbacks)
		{
			it(key, scancode, action, mode);
		}
	});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		for (auto it : cursorCallbacks)
		{
			it(xpos, ypos);
		}
	});
	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
		for (auto it : scrollCallbacks)
		{
			it(xoffset, yoffset);
		}
	});
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int key, int xpos, int ypos) {
		for (auto it : mouseCallbacks)
		{
			it(key, xpos, ypos);
		}
	});
}

void InputHelper::AttachKeyCallback(KeyCallback callback)
{
	keyCallbacks.emplace_back(callback);
}

void InputHelper::AttachCursorCallback(CursorCallback callback)
{
	cursorCallbacks.emplace_back(callback);
}

void InputHelper::AttachScrollCallback(ScrollCallback callback)
{
	scrollCallbacks.emplace_back(callback);
}

void InputHelper::AttachMouseCallback(MouseCallback callback)
{
	mouseCallbacks.emplace_back(callback);
}