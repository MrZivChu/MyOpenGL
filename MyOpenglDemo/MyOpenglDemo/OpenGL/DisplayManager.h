#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class DisplayManager
{
public:
	static DisplayManager& Get()
	{
		static DisplayManager instance;
		return  instance;
	}

	static void CreateWindow(int width, int height, const char* title, GLFWkeyfun cbfun);
	static void CloseWindow();

	static GLFWwindow* window;
};

