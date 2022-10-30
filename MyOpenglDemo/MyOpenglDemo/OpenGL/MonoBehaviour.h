#pragma once

#include <stdio.h>
#include "EngineEventHandler.h"
#include "InputHelper.h"
#include "DisplayManager.h"

class MonoBehaviour
{
public:
	GLFWwindow *window = nullptr;

	MonoBehaviour() {
		window = DisplayManagerService::Get().GetWindow();

		EngineEventHandler::AttachStartCallback(std::bind(&MonoBehaviour::Start, this));
		EngineEventHandler::AttachUpdateCallback(std::bind(&MonoBehaviour::Update, this, std::placeholders::_1));
		EngineEventHandler::AttachQuitCallback(std::bind(&MonoBehaviour::Quit, this));

		InputHelperService::Get().AttachKeyCallback(std::bind(&MonoBehaviour::OnKeyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
		InputHelperService::Get().AttachCursorCallback(std::bind(&MonoBehaviour::OnCursorCallback, this, std::placeholders::_1, std::placeholders::_2));
		InputHelperService::Get().AttachScrollCallback(std::bind(&MonoBehaviour::OnScrollCallback, this, std::placeholders::_1, std::placeholders::_2));
		InputHelperService::Get().AttachMouseCallback(std::bind(&MonoBehaviour::OnMouseCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	};
private:
	virtual void Start() {};
	virtual void Update(float deltaTime) {};
	virtual void Quit() {};

	virtual void OnKeyCallback(int key, int scancode, int action, int mode) {};
	virtual void OnCursorCallback(double xpos, double ypos) {};
	virtual void OnScrollCallback(double xoffset, double yoffset) {};
	virtual void OnMouseCallback(int key, int xpos, int ypos) {};
};

