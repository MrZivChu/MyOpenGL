#pragma once

#include "OpenGL/Camera.h"
#include "OpenGL/ResourceManager.h"
#include "OpenGL/MonoBehaviour.h"

class stencil_testing :public MonoBehaviour
{
private:
	Camera camera = (glm::vec3(0.0f, 0.0f, 3.0f));
	unsigned int cubeVAO, cubeVBO;
	unsigned int planeVAO, planeVBO;
	Shader shader;
	Shader shaderSingleColor;
	Texture2D  cubeTexture;
	Texture2D floorTexture;

	virtual void Start() override;
	virtual void Update(GLfloat deltaTime) override;
	virtual void Quit() override;
};

