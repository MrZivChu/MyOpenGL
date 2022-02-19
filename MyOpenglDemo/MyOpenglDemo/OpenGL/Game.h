#pragma once
#include "DisplayManager.h"
class Game
{
public:
	Game(int width, int height, const char* title)
	{
		width_ = width;
		height_ = height;
		title_ = title;
	}

	void Run()
	{
		DisplayManager::Get().CreateWindow(width_, height_, title_);
		Init();
		glfwSetWindowUserPointer(DisplayManager::Get().GetWindow(), this);
		DisplayManager::Get().RegisterKeyEvent([](GLFWwindow* window, int key, int scancode, int action, int mode) {
			Game* game = (Game*)(glfwGetWindowUserPointer(DisplayManager::Get().GetWindow()));
			game->ProcessInput(window, key, scancode, action, mode);
		});
		while (!glfwWindowShouldClose(DisplayManager::Get().GetWindow()))
		{
			GLfloat currentFrame = glfwGetTime();
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			Update(deltaTime);

			glfwPollEvents();

			Render();
		}
		DisplayManager::Get().CloseWindow();
		Quit();
	}
private:
	int width_;
	int height_;
	const char* title_ = nullptr;
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	virtual void Init() = 0;
	virtual void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;

	virtual void Update(GLfloat deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Quit() = 0;

};

