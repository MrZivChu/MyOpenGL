#include "DisplayManager.h"

void DisplayManager::CreateWindow(int width, int height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr) {
		return;
	}

	glfwMakeContextCurrent(window);

	//有些显卡的驱动程序不能正确给出所支持的扩展信息，导致GLEW不能正确获取某些函数的入口地址
	//解决：先glewExperimental = GL_TRUE 再 glewInit()
	glewExperimental = GL_TRUE;
	glewInit();
	//调用一次来捕获glewInit()错误，所有其他错误现在都来自我们的应用程序。
	glGetError();

	glViewport(0, 0, width, height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSwapInterval(0);//VSync is off, 1 if VSync on

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});

}

GLFWwindow* DisplayManager::GetWindow()
{
	return window;
}

void DisplayManager::CloseWindow()
{
	glfwTerminate();
}
