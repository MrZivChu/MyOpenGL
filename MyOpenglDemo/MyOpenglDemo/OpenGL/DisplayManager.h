#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class DisplayManager
{
public:
	void CreateWindow(int width, int height, const char* title);
	void CloseWindow();
	void RegisterKeyEvent(GLFWkeyfun cbfun);
	void RegisterCursorEvent(GLFWcursorposfun cbfun);
	void RegisterScrollEvent(GLFWscrollfun cbfun);
	void RegisterFramebufferSizeEvent(GLFWframebuffersizefun cbfun);
	GLFWwindow* GetWindow();

private:
	GLFWwindow* window = nullptr;
};

class  DisplayManagerService
{
public:
	static DisplayManager& Get()
	{
		static DisplayManager instance;
		return  instance;
	}
};

