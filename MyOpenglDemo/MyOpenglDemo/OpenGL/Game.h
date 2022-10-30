#pragma once

#include "DisplayManager.h"
#include "InputHelper.h"
#include "EngineEventHandler.h"

class Game
{
private:
	static double lastFrame;
public:
	Game() = delete;
	static void Init(int width, int height, const char* title)
	{
		DisplayManagerService::Get().CreateWindow(width, height, title);
		InputHelperService::Get().Init();
	}

	static void Run()
	{
		for (auto& it : EngineEventHandler::GetStartCallbacks())
		{
			it();
		}
		GLFWwindow* window = DisplayManagerService::Get().GetWindow();
		while (!glfwWindowShouldClose(window))
		{
			double currentFrame = glfwGetTime();
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			for (auto& it : EngineEventHandler::GetUpdateCallbacks())
			{
				it(deltaTime);
			}
			//交换缓冲区（既然buffer加了s，也就意味着不止一个buffer，所以这里涉及到了一个双缓冲的概念）
			//因为电脑绘图是一个个像素逐一画的，需要时间，如果单一缓冲，我们可能会看到具体绘画过程，会造成屏幕闪烁等问题，而我们用户不需要具体看到你绘制的过程，所以为了解决这个问题，这里用了双缓冲技术，用两个内存区域来保存数据，分为前缓冲区和后缓冲区，前缓冲区用于展示屏幕上的内容，而后缓冲区就用来绘制，然后每一帧开始的时候，将两个缓冲区交换，这样后缓冲区又可以画新的内容。
			//如果注释掉，将不会绘制界面
			glfwSwapBuffers(window);
			//轮询IO事件（按键按下和抬起，鼠标移动等)
			//如果注释掉，那么按键，鼠标事件都不会响应
			glfwPollEvents();
		}
		for (auto& it : EngineEventHandler::GetQuitCallbacks())
		{
			it();
		}
		DisplayManagerService::Get().CloseWindow();
	}
};
double Game::lastFrame = 0.0f;