#ifndef CAMERA_H
#define CAMERA_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "MonoBehaviour.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera : MonoBehaviour
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	void Start()
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void OnCursorCallback(double xposIn, double yposIn)
	{
		if (mouseKey == 1) {
			ProcessMouseMovement(xposIn, yposIn);
		}
	}

	void OnScrollCallback(double xoffset, double yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 90.0f)
			Zoom = 90.0f;
	}

	void OnMouseCallback(int key, int xpos, int ypos)
	{
		if (mouseKey == key) { //同一个按键，表示release
			mouseKey = -1;
			firstMouse = true;
		}
		else {//表示Pressed  0表示左键按下，1表示右键按下，2表示中键按下
			mouseKey = key;
		}
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void Update(float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float velocity = MovementSpeed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			Position += Front * velocity;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			Position -= Front * velocity;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			Position -= Right * velocity;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			Position += Right * velocity;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			Position -= Up * velocity;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			Position += Up * velocity;
	}

	void ProcessMouseMovement(float xposIn, float yposIn, GLboolean constrainPitch = true)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		updateCameraVectors();
	}

private:
	float lastX = 0;
	float lastY = 0;
	bool firstMouse = true;
	int mouseKey = -1;

	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif

