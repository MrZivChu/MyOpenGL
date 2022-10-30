#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "stencil_testing.h"
#include "Game/TTQ.h"
#include "OpenGL/Game.h"

int main(int argc, char *argv[])
{
	Game::Init(800, 600, "Breakout");
	std::shared_ptr<MonoBehaviour> game = std::make_unique<stencil_testing>();
	Game::Run();
	return 0;
}

