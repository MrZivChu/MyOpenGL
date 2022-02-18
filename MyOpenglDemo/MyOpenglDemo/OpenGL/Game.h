#pragma once
#include "DisplayManager.h"
class Game
{
public:
	int width_;
	int height_;
	const char* title_ = nullptr;
	GLFWkeyfun cbfun_;

	Game(int width, int height, const char* title, GLFWkeyfun cbfun);

	void Run()
	{
		Initalize();
		DisplayManager::Get().CreateWindow(width_, height_, title_, cbfun_);
		LoadContent();
		while (!glfwWindowShouldClose(DisplayManager::window))
		{
			GLfloat currentFrame = glfwGetTime();
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			Update(deltaTime);

			glfwPollEvents();
			 
			Render();
		}
		DisplayManager::Get().CloseWindow();
	}

	virtual void Initalize();
	virtual void LoadContent();

	virtual void Update(GLfloat deltaTime);
	virtual void Render();

private:
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
};

