#pragma once

#include "OpenGL/Game.h"
#include "OpenGL/Camera.h"
#include "OpenGL/ResourceManager.h"

class stencil_testing :public Game
{
private:
	Camera camera = (glm::vec3(0.0f, 0.0f, 3.0f));
	unsigned int cubeVAO, cubeVBO;
	unsigned int planeVAO, planeVBO;
	Shader shader;
	Shader shaderSingleColor;
	Texture2D  cubeTexture;
	Texture2D floorTexture;

	void processInput(GLFWwindow *window, GLfloat deltaTime);

	// Í¨¹ý Game ¼Ì³Ð
	virtual void Init() override;
	virtual void Update(GLfloat deltaTime) override;
	virtual void Render() override;
	virtual void Quit() override;
	virtual void SetKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode) override;
	virtual void SetCursorCallback(GLFWwindow * window, double xposIn, double yposIn) override;
	virtual void SetScrollCallback(GLFWwindow * window, double xoffset, double yoffset) override;
};

