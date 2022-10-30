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
			//��������������Ȼbuffer����s��Ҳ����ζ�Ų�ֹһ��buffer�����������漰����һ��˫����ĸ��
			//��Ϊ���Ի�ͼ��һ����������һ���ģ���Ҫʱ�䣬�����һ���壬���ǿ��ܻῴ������滭���̣��������Ļ��˸�����⣬�������û�����Ҫ���忴������ƵĹ��̣�����Ϊ�˽��������⣬��������˫���弼�����������ڴ��������������ݣ���Ϊǰ�������ͺ󻺳�����ǰ����������չʾ��Ļ�ϵ����ݣ����󻺳������������ƣ�Ȼ��ÿһ֡��ʼ��ʱ�򣬽����������������������󻺳����ֿ��Ի��µ����ݡ�
			//���ע�͵�����������ƽ���
			glfwSwapBuffers(window);
			//��ѯIO�¼����������º�̧������ƶ���)
			//���ע�͵�����ô����������¼���������Ӧ
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