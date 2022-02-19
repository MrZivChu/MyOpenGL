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

	void CreateWindow(int width, int height, const char* title);
	void CloseWindow();
	void RegisterKeyEvent(GLFWkeyfun cbfun);
	GLFWwindow* GetWindow();

private:
	GLFWwindow* window = nullptr;
};

