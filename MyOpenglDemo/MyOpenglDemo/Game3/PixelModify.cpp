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

	// 创建对象一般四步骤：
	//					1、生成 Gen  
	//					2、绑定 Bind(id)  
	//					3、设置属性
	//					4、解绑 Bind(0)
	// 使用的时候：
	//					绑定 Bind(id)  [Shader除外，使用：glUseProgram(id)]
	// 销毁对象：
	//					删除 Delete(id)

	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);

	glGenBuffers(1, &planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(12);
	glVertexAttribPointer(12, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glGenBuffers(1, &planeEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	shaderTexture.SetFloat("width", textureImg1.GetWidth(), true);
	shaderTexture.SetFloat("height", textureImg1.GetHeight(), true);

	//如果只有一张图片的话，就没必要获取图片索引了
	//如果超过一张图片的话，那么需要获取图片的索引了，获取方式如下：
	shaderTexture.SetInteger("texture0", 6, true);
	shaderTexture.SetInteger("texture1", 9, true);


	// VAO必须是第一个bind(0) ！！！
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(7);
	glDisableVertexAttribArray(12);
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