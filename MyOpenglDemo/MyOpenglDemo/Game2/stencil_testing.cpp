#include "stencil_testing.h"
#include "DisplayManager.h"

void stencil_testing::Start()
{
	shaderTexture = ResourceManager::LoadShader("Game2/shaders/stencil.vert", "Game2/shaders/texture.frag", nullptr, "shaderTexture");
	shaderColor = ResourceManager::LoadShader("Game2/shaders/stencil.vert", "Game2/shaders/color.frag", nullptr, "shaderColor");
	cubeTexture = ResourceManager::LoadTexture("Game2/textures/block_solid.png", false, "cubeTexture");
	floorTexture = ResourceManager::LoadTexture("Game2/textures/background.jpg", false, "floorTexture");
	float cubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	float planeVertices[] = {
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};
	// cube VAO
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	// plane VAO
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	glfwGetWindowSize(window, &windowWidth, &windowHeight);
}

void stencil_testing::Update(float deltaTime)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glStencilMask(0xFF);//只有启用模板缓冲写入，才能清除模板缓冲内容
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	shaderColor.Use();
	shaderColor.SetMatrix4("view", view);
	shaderColor.SetMatrix4("projection", projection);

	shaderTexture.Use();
	shaderTexture.SetMatrix4("view", view);
	shaderTexture.SetMatrix4("projection", projection);
	//***************************************************
	glDisable(GL_STENCIL_TEST);// 禁用模板缓冲
	glEnable(GL_CULL_FACE);//启用剔除功能
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);//启用深度测试
	glDepthFunc(GL_LESS);

	shaderTexture.Use();// 绘制地板
	glBindVertexArray(planeVAO);
	floorTexture.Bind();
	shaderTexture.SetMatrix4("model", glm::mat4(1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//***************************************************
	glEnable(GL_STENCIL_TEST);// 启用模板缓冲
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//替换值
	//glStencilMask(0xFF);//启用写入
	glStencilFunc(GL_ALWAYS, 1, 0xFF);//写入1
	glDisable(GL_CULL_FACE);//关闭剔除功能,双面
	glEnable(GL_DEPTH_TEST);//启用深度测试
	glDepthFunc(GL_LESS);

	shaderTexture.Use();// 绘制第一个Box
	glBindVertexArray(cubeVAO);
	cubeTexture.Bind();
	shaderTexture.SetMatrix4("model", glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//***************************************************
	//glStencilMask(0x00);//禁用写入
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glDisable(GL_CULL_FACE);//关闭剔除功能,双面
	glDisable(GL_DEPTH_TEST);//关闭深度测试

	shaderColor.Use();// 绘制第一个Box边框
	glBindVertexArray(cubeVAO);
	shaderColor.SetMatrix4("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f)), glm::vec3(1.25f)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//***************************************************
	glStencilMask(0xFF);//只有启用模板缓冲写入，才能清除模板缓冲内容
	glClear(GL_STENCIL_BUFFER_BIT);
	//***************************************************
	glEnable(GL_STENCIL_TEST);// 启用模板缓冲
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);//启用写入
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glDisable(GL_CULL_FACE);//关闭剔除功能,双面
	glEnable(GL_DEPTH_TEST);//启用深度测试
	glDepthFunc(GL_LESS);

	shaderTexture.Use();// 绘制第二个Box
	glBindVertexArray(cubeVAO);
	cubeTexture.Bind();
	shaderTexture.SetMatrix4("model", glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//***************************************************
	glStencilMask(0x00);//禁用写入
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glDisable(GL_CULL_FACE);//关闭剔除功能,双面
	glDisable(GL_DEPTH_TEST);//关闭深度测试

	shaderColor.Use();// 绘制第二个Box边框
	glBindVertexArray(cubeVAO);
	shaderColor.SetMatrix4("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.25f)));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void stencil_testing::Quit()
{
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);
	ResourceManager::Clear();
}
