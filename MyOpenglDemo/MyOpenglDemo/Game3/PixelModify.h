#pragma once
#include "ResourceManager.h"
#include "MonoBehaviour.h"

class PixelModify :public MonoBehaviour
{
public:
	unsigned int planeVAO;
	unsigned int  planeVBO;
	unsigned int  planeEBO;
	Shader shaderTexture;
	Texture2D textureImg;

	void Start();
	void Update(float deltaTime);
	void Quit();
};

