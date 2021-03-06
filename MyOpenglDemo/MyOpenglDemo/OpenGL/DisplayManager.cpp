#include "DisplayManager.h"

void DisplayManager::CreateWindow(int width, int height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr) {
		return;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	//调用一次来捕获glewInit()错误，所有其他错误现在都来自我们的应用程序。
	glGetError(); 

	glViewport(0, 0, width, height);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSwapInterval(0);//VSync is off, 1 if VSync on
}

void DisplayManager::RegisterKeyEvent(GLFWkeyfun cbfun)
{
	glfwSetKeyCallback(window, cbfun);
}

void DisplayManager::RegisterCursorEvent(GLFWcursorposfun cbfun)
{
	glfwSetCursorPosCallback(window, cbfun);
}

void DisplayManager::RegisterScrollEvent(GLFWscrollfun cbfun)
{
	glfwSetScrollCallback(window, cbfun);
}

void DisplayManager::RegisterFramebufferSizeEvent(GLFWframebuffersizefun cbfun)
{
	glfwSetFramebufferSizeCallback(window, cbfun);
}

GLFWwindow* DisplayManager::GetWindow()
{
	return window;
}

void DisplayManager::CloseWindow()
{
	glfwTerminate();
}
