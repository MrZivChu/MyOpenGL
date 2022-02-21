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
			game->SetKeyCallback(window, key, scancode, action, mode);
		});
		DisplayManagerService::Get().RegisterCursorEvent([](GLFWwindow* window, double xpos, double ypos) {
			Game* game = (Game*)(glfwGetWindowUserPointer(DisplayManagerService::Get().GetWindow()));
			game->SetCursorCallback(window, xpos, ypos);
		});
		DisplayManagerService::Get().RegisterScrollEvent([](GLFWwindow* window, double xoffset, double yoffset) {
			Game* game = (Game*)(glfwGetWindowUserPointer(DisplayManagerService::Get().GetWindow()));
			game->SetScrollCallback(window, xoffset, yoffset);
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

			glfwPollEvents();

			Render();
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

