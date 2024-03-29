

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H
#include <vector>
#include <tuple>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameObject.h"
#include "GameLevel.h"
#include "PowerUp.h"
#include "../OpenGL/MonoBehaviour.h"

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// Represents the four possible (collision) directions
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};
// Defines a Collision typedef that represents collision data
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>

															   // Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100, 20);
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);
// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;

// TTQ holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class TTQ : public MonoBehaviour
{
public:
	// TTQ state
	GameState              State = GameState::GAME_ACTIVE;
	GLboolean              Keys[1024];
	GLboolean              KeysProcessed[1024];
	std::vector<GameLevel> Levels;
	std::vector<PowerUp>   PowerUps;
	GLuint                 Level = 0;
	GLuint                 Lives = 3;
	// Constructor/Destructor
	~TTQ();
	// Initialize game state (load all shaders/textures/levels)
	void Start();
	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	virtual void OnKeyCallback(int key, int scancode, int action, int mode) override;
	void Quit();
	void DoCollisions();
	// Reset
	void ResetLevel();
	void ResetPlayer();
	// Powerups
	void SpawnPowerUps(GameObject &block);
	void UpdatePowerUps(GLfloat dt);
	int windowWidth = 0;
	int windowHeight = 0;
};

#endif

