#pragma once

#include "Camera.h"
#include "ResourceManager.h"
#include "MonoBehaviour.h"

class stencil_testing :public MonoBehaviour
{
private:
	Camera camera = (glm::vec3(0.0f, 0.0f, 3.0f));
	unsigned int cubeVAO, cubeVBO;
	unsigned int planeVAO, planeVBO;
	Shader shaderTexture;
	Shader shaderColor;
	Texture2D  cubeTexture;
	Texture2D floorTexture;
	int windowWidth = 0;
	int windowHeight = 0;

	virtual void Start() override;
	virtual void Update(GLfloat deltaTime) override;
	virtual void Quit() override;
};

