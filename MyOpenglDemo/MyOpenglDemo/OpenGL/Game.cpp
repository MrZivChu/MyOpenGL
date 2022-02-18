#include "Game.h"

Game::Game(int width, int height, const char* title, GLFWkeyfun cbfun)
{
	width_ = width;
	height_ = height;
	title_ = title;
	cbfun_ = cbfun;
}
