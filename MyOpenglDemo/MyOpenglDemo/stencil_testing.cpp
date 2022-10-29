#include "stencil_testing.h"

void stencil_testing::Init()
{
	shader = ResourceManager::LoadShader("shaders/stencil_testing.vs", "shaders/stencil_testing.fs", nullptr, "shader");
	shaderSingleColor = ResourceManager::LoadShader("shaders/stencil_testing.vs", "shaders/stencil_single_color.fs", nullptr, "shaderSingleColor");

	cubeTexture = ResourceManager::LoadTexture("textures/block_solid.png", false, "cubeTexture");
	floorTexture = ResourceManager::LoadTexture("textures/background.jpg", false, "floorTexture");

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
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

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

	shader.Use();
	shader.SetInteger("texture1", 0);
}

void stencil_testing::Update(GLfloat deltaTime)
{
	processInput(DisplayManagerService::Get().GetWindow(), deltaTime);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// set uniforms
	shaderSingleColor.Use();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	shaderSingleColor.SetMatrix4("view", view);
	shaderSingleColor.SetMatrix4("projection", projection);

	shader.Use();
	shader.SetMatrix4("view", view);
	shader.SetMatrix4("projection", projection);


	// 设置关闭模板缓冲写入
	glStencilMask(0x00);

	// 绘制地板
	glBindVertexArray(planeVAO);
	floorTexture.Bind();
	shader.SetMatrix4("model", glm::mat4(1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	// 启用模板缓冲写入
	glStencilMask(0xFF);
	// 写入为1
	glStencilFunc(GL_ALWAYS, 1, 0xFF);

	glBindVertexArray(cubeVAO);
	glActiveTexture(GL_TEXTURE0);
	cubeTexture.Bind();

	// 绘制第一个Box
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	shader.SetMatrix4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 绘制第二个Box
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	shader.SetMatrix4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	// 绘制小Box
	float scale1= 0.5f;
	glStencilMask(0xFF);
	glDisable(GL_DEPTH_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(scale1));
	shader.SetMatrix4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.5f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(scale1));
	shader.SetMatrix4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shaderSingleColor.Use();
	float scale = 1.25f;
	glBindVertexArray(cubeVAO);
	cubeTexture.Bind();

	// 关闭模板缓冲写入
	glStencilMask(0x00);
	// 不等于1的地方显示
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glDisable(GL_DEPTH_TEST);

	// 绘制第一个Box边框
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(scale));
	shaderSingleColor.SetMatrix4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glStencilFunc(GL_EQUAL, 0, 0xFF);

	// 绘制第二个Box边框
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(scale));
	shaderSingleColor.SetMatrix4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	glBindVertexArray(0);
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
}

void stencil_testing::Render() {}

void stencil_testing::Quit()
{
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);
	ResourceManager::Clear();
}

void stencil_testing::processInput(GLFWwindow *window, GLfloat deltaTime)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	camera.ProcessKeyboard(window, deltaTime);
}

void stencil_testing::SetKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode) {}

void stencil_testing::SetCursorCallback(GLFWwindow * window, double xposIn, double yposIn)
{
	camera.ProcessMouseMovement(xposIn, yposIn);
}

void stencil_testing::SetScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
