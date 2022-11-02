#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Game1/Game/TTQ.h"
#include "Game2/stencil_testing.h"
#include "Game3/PixelModify.h"
#include "OpenGL/Game.h"

int main(int argc, char *argv[])
{
	Game::Init(800, 800, "Demo");
	std::shared_ptr<MonoBehaviour> game = std::make_unique<PixelModify>();
	Game::Run();
	return 0;
}

