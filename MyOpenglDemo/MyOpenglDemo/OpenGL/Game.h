#pragma once
#include "DisplayManager.h"
class Game
{
public:
	void Run(int width, int height, const char* title)
	{
		windowWidth = width;
		windowHeight = height;
		DisplayManagerService::Get().CreateWindow(width, height, title);

		glfwSetWindowUserPointer(DisplayManagerService::Get().GetWindow(), this);
		DisplayManagerService::Get().RegisterKeyEvent([](GLFWwindow* window, int key, int scancode, int action, int mode) {
			Game* game = (Game*)(glfwGetWindowUserPointer(DisplayManagerService::Get().GetWindow()));
			if (game) {
				game->SetKeyCallback(window, key, scancode, action, mode);
			}
		});
		DisplayManagerService::Get().RegisterCursorEvent([](GLFWwindow* window, double xpos, double ypos) {
			Game* game = (Game*)(glfwGetWindowUserPointer(DisplayManagerService::Get().GetWindow()));
			if (game) {
				game->SetCursorCallback(window, xpos, ypos);
			}
		});
		DisplayManagerService::Get().RegisterScrollEvent([](GLFWwindow* window, double xoffset, double yoffset) {
			Game* game = (Game*)(glfwGetWindowUserPointer(DisplayManagerService::Get().GetWindow()));
			if (game) {
				game->SetScrollCallback(window, xoffset, yoffset);
			}
		});
		DisplayManagerService::Get().RegisterFramebufferSizeEvent([](GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		});

		Init();
		while (!glfwWindowShouldClose(DisplayManagerService::Get().GetWindow()))
		{
			GLfloat currentFrame = glfwGetTime();
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			Update(deltaTime);
			Render();

			//交换缓冲区（既然buffer加了s，也就意味着不止一个buffer，所以这里涉及到了一个双缓冲的概念）
			//因为电脑绘图是一个个像素逐一画的，需要时间，如果单一缓冲，我们可能会看到具体绘画过程，会造成屏幕闪烁等问题，而我们用户不需要具体看到你绘制的过程，所以为了解决这个问题，这里用了双缓冲技术，用两个内存区域来保存数据，分为前缓冲区和后缓冲区，前缓冲区用于展示屏幕上的内容，而后缓冲区就用来绘制，然后每一帧开始的时候，将两个缓冲区交换，这样后缓冲区又可以画新的内容。
			//如果注释掉，将不会绘制界面
			glfwSwapBuffers(DisplayManagerService::Get().GetWindow());
			//轮询IO事件（按键按下和抬起，鼠标移动等)
			//如果注释掉，那么按键，鼠标事件都不会响应
			glfwPollEvents();
		}
		DisplayManagerService::Get().CloseWindow();
		Quit();
	}

protected:
	GLuint windowWidth;
	GLuint windowHeight;

private:
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	virtual void Init() = 0;
	virtual void Update(GLfloat deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Quit() = 0;

	virtual void SetKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;
	virtual void SetCursorCallback(GLFWwindow* window, double xpos, double ypos) = 0;
	virtual void SetScrollCallback(GLFWwindow* window, double xoffset, double yoffset) = 0;
};

