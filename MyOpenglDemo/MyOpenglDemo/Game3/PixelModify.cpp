#include "PixelModify.h"

void PixelModify::Start()
{
	shaderTexture = ResourceManager::LoadShader("Game3/shaders/vert.vert", "Game3/shaders/frag.frag", nullptr, "shaderTexture");
	textureImg1 = ResourceManager::LoadTexture("Game3/textures/1.jpeg", false, "floorTexture1");
	textureImg2 = ResourceManager::LoadTexture("Game3/textures/2.png", true, "floorTexture2");
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glGenBuffers(1, &planeEBO);
	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(7);

	glVertexAttribPointer(12, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(12);

	shaderTexture.SetFloat("width", textureImg1.GetWidth(), true);
	shaderTexture.SetFloat("height", textureImg1.GetHeight(), true);

	//如果只有一张图片的话，就没必要获取图片索引了
	//如果超过一张图片的话，那么需要获取图片的索引了，获取方式如下：
	shaderTexture.SetInteger("texture0", 6, true);
	shaderTexture.SetInteger("texture1", 9, true);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PixelModify::Update(float deltaTime)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shaderTexture.Use();
	glBindVertexArray(planeVAO);

	//如果只有一张图片的话，那么直接 textureImg.Bind() 即可，也没必要glActiveTexture了
	//超过一张图片的话，需要根据索引进行设置，设置方式如下：
	//绑定第一张图片
	glActiveTexture(GL_TEXTURE6);
	textureImg1.Bind();	
	//绑定第二张图片
	glActiveTexture(GL_TEXTURE9);
	textureImg2.Bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void PixelModify::Quit()
{
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &planeEBO);
	ResourceManager::Clear();
}