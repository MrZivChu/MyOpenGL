/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Game/TTQ.h"
#include "OpenGL/ResourceManager.h"
#include "OpenGL/DisplayManager.h"

int main(int argc, char *argv[])
{
	std::shared_ptr<Game> game = std::make_unique<TTQ>();
	game->Run(800, 600, "Breakout");
	return 0;
}

