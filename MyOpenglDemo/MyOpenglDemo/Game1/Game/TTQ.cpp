

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <algorithm>
#include <sstream>

#include <irrKlang.h>
using namespace irrklang;

#include "TTQ.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "PostProcessor.h"
#include "InputHelper.h"
#include "TextRenderer.h"
#include "GameObject.h"
#include "BallObject.h"
#include "ParticleGenerator.h"


// TTQ-related State data
SpriteRenderer    *Renderer;
GameObject        *Player;
BallObject        *Ball;
ParticleGenerator *Particles;
PostProcessor     *Effects;
ISoundEngine      *SoundEngine = createIrrKlangDevice();
GLfloat            ShakeTime = 0.0f;
TextRenderer      *Text;

TTQ::~TTQ()
{
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
	delete Text;
	SoundEngine->drop();
}

void TTQ::Start()
{
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	// Load shaders
	ResourceManager::LoadShader("Game1/shaders/sprite.vert", "Game1/shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("Game1/shaders/particle.vert", "Game1/shaders/particle.frag", nullptr, "particle");
	ResourceManager::LoadShader("Game1/shaders/post_processing.vert", "Game1/shaders/post_processing.frag", nullptr, "postprocessing");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->windowWidth), static_cast<GLfloat>(this->windowHeight), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("Game1/textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("Game1/textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("Game1/textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("Game1/textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("Game1/textures/paddle.png", GL_TRUE, "paddle");
	ResourceManager::LoadTexture("Game1/textures/particle.png", GL_TRUE, "particle");
	ResourceManager::LoadTexture("Game1/textures/powerup_speed.png", GL_TRUE, "powerup_speed");
	ResourceManager::LoadTexture("Game1/textures/powerup_sticky.png", GL_TRUE, "powerup_sticky");
	ResourceManager::LoadTexture("Game1/textures/powerup_increase.png", GL_TRUE, "powerup_increase");
	ResourceManager::LoadTexture("Game1/textures/powerup_confuse.png", GL_TRUE, "powerup_confuse");
	ResourceManager::LoadTexture("Game1/textures/powerup_chaos.png", GL_TRUE, "powerup_chaos");
	ResourceManager::LoadTexture("Game1/textures/powerup_passthrough.png", GL_TRUE, "powerup_passthrough");
	// Set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->windowWidth, this->windowHeight);
	Text = new TextRenderer(this->windowWidth, this->windowHeight);
	Text->Load("Game1/fonts/OCRAEXT.TTF", 24);
	// Load levels
	GameLevel one;
	one.Load("Game1/levels/one.lvl", this->windowWidth, this->windowHeight * 0.5);
	GameLevel two;
	two.Load("Game1/levels/two.lvl", this->windowWidth, this->windowHeight * 0.5);
	GameLevel three;
	three.Load("Game1/levels/three.lvl", this->windowWidth, this->windowHeight * 0.5);
	GameLevel four;
	four.Load("Game1/levels/four.lvl", this->windowWidth, this->windowHeight * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;
	// Configure game objects
	glm::vec2 playerPos = glm::vec2(this->windowWidth / 2 - PLAYER_SIZE.x / 2, this->windowHeight - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
	// Audio
	SoundEngine->play2D("Game1/audio/breakout.mp3", GL_TRUE);
}

void TTQ::Update(GLfloat dt)
{
	ProcessInput(dt);

	// Update objects
	Ball->Move(dt, this->windowWidth);
	// Check for collisions
	this->DoCollisions();
	// Update particles
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
	// Update PowerUps
	this->UpdatePowerUps(dt);
	// Reduce shake time
	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
			Effects->Shake = GL_FALSE;
	}
	// Check loss condition
	if (Ball->Position.y >= this->windowHeight) // Did ball reach bottom edge?
	{
		--this->Lives;
		// Did the player lose all his lives? : TTQ over
		if (this->Lives == 0)
		{
			this->ResetLevel();
			this->State = GAME_MENU;
		}
		this->ResetPlayer();
	}
	// Check win condition
	if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
	{
		this->ResetLevel();
		this->ResetPlayer();
		Effects->Chaos = GL_TRUE;
		this->State = GAME_WIN;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN)
	{
		// Begin rendering to postprocessing quad
		Effects->BeginRender();
		// Draw background
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->windowWidth, this->windowHeight), 0.0f);
		// Draw level
		this->Levels[this->Level].Draw(*Renderer);
		// Draw player
		Player->Draw(*Renderer);
		// Draw PowerUps
		for (PowerUp &powerUp : this->PowerUps)
			if (!powerUp.Destroyed)
				powerUp.Draw(*Renderer);
		// Draw particles	
		Particles->Draw();
		// Draw ball
		Ball->Draw(*Renderer);
		// End rendering to postprocessing quad
		Effects->EndRender();
		// Render postprocessing quad
		Effects->Render(glfwGetTime());
		// Render text (don't include in postprocessing)
		std::stringstream ss; ss << this->Lives;
		Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}
	if (this->State == GAME_MENU)
	{
		Text->RenderText("Press ENTER to start", 250.0f, this->windowHeight / 2, 1.0f);
		Text->RenderText("Press W or S to select level", 245.0f, this->windowHeight / 2 + 20.0f, 0.75f);
	}
	if (this->State == GAME_WIN)
	{
		Text->RenderText("You WON!!!", 320.0f, this->windowHeight / 2 - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->windowHeight / 2, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
	}
}


void TTQ::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_MENU)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
		{
			this->Level = (this->Level + 1) % 4;
			this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
		{
			if (this->Level > 0)
				--this->Level;
			else
				this->Level = 3;
			this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
		}
	}
	if (this->State == GAME_WIN)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			Effects->Chaos = GL_FALSE;
			this->State = GAME_MENU;
		}
	}
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// Move playerboard
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0)
			{
				Player->Position.x -= velocity;
				if (Ball->Stuck)
					Ball->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->windowWidth - Player->Size.x)
			{
				Player->Position.x += velocity;
				if (Ball->Stuck)
					Ball->Position.x += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = GL_FALSE;
	}
}

void TTQ::ResetLevel()
{
	if (this->Level == 0)this->Levels[0].Load("levels/one.lvl", this->windowWidth, this->windowHeight * 0.5f);
	else if (this->Level == 1)
		this->Levels[1].Load("levels/two.lvl", this->windowWidth, this->windowHeight * 0.5f);
	else if (this->Level == 2)
		this->Levels[2].Load("levels/three.lvl", this->windowWidth, this->windowHeight * 0.5f);
	else if (this->Level == 3)
		this->Levels[3].Load("levels/four.lvl", this->windowWidth, this->windowHeight * 0.5f);

	this->Lives = 3;
}

void TTQ::ResetPlayer()
{
	// Reset player/ball stats
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->windowWidth / 2 - PLAYER_SIZE.x / 2, this->windowHeight - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
	// Also disable all active powerups
	Effects->Chaos = Effects->Confuse = GL_FALSE;
	Ball->PassThrough = Ball->Sticky = GL_FALSE;
	Player->Color = glm::vec3(1.0f);
	Ball->Color = glm::vec3(1.0f);
}


// PowerUps
GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);

void TTQ::UpdatePowerUps(GLfloat dt)
{
	for (PowerUp &powerUp : this->PowerUps)
	{
		powerUp.Position += powerUp.Velocity * dt;
		if (powerUp.Activated)
		{
			powerUp.Duration -= dt;

			if (powerUp.Duration <= 0.0f)
			{
				// Remove powerup from list (will later be removed)
				powerUp.Activated = GL_FALSE;
				// Deactivate effects
				if (powerUp.Type == "sticky")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
					{	// Only reset if no other PowerUp of type sticky is active
						Ball->Sticky = GL_FALSE;
						Player->Color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "pass-through")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
					{	// Only reset if no other PowerUp of type pass-through is active
						Ball->PassThrough = GL_FALSE;
						Ball->Color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "confuse")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
					{	// Only reset if no other PowerUp of type confuse is active
						Effects->Confuse = GL_FALSE;
					}
				}
				else if (powerUp.Type == "chaos")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
					{	// Only reset if no other PowerUp of type chaos is active
						Effects->Chaos = GL_FALSE;
					}
				}
			}
		}
	}
	// Remove all PowerUps from vector that are destroyed AND !activated (thus either off the map or finished)
	// Note we use a lambda expression to remove each PowerUp which is destroyed and not activated
	this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
		[](const PowerUp &powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
	), this->PowerUps.end());
}

GLboolean ShouldSpawn(GLuint chance)
{
	GLuint random = rand() % chance;
	return random == 0;
}
void TTQ::SpawnPowerUps(GameObject &block)
{
	if (ShouldSpawn(75)) // 1 in 75 chance
		this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed")));
	if (ShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")));
	if (ShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
	if (ShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase")));
	if (ShouldSpawn(15)) // Negative powerups should spawn more often
		this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
	if (ShouldSpawn(15))
		this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")));
}

void ActivatePowerUp(PowerUp &powerUp)
{
	// Initiate a powerup based type of powerup
	if (powerUp.Type == "speed")
	{
		Ball->Velocity *= 1.2;
	}
	else if (powerUp.Type == "sticky")
	{
		Ball->Sticky = GL_TRUE;
		Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == "pass-through")
	{
		Ball->PassThrough = GL_TRUE;
		Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == "pad-size-increase")
	{
		Player->Size.x += 50;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effects->Chaos)
			Effects->Confuse = GL_TRUE; // Only activate if chaos wasn't already active
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effects->Confuse)
			Effects->Chaos = GL_TRUE;
	}
}

GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
	// Check if another PowerUp of the same type is still active
	// in which case we don't disable its effect (yet)
	for (const PowerUp &powerUp : powerUps)
	{
		if (powerUp.Activated)
			if (powerUp.Type == type)
				return GL_TRUE;
	}
	return GL_FALSE;
}


// Collision detection
GLboolean CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 closest);

void TTQ::DoCollisions()
{
	for (GameObject &box : this->Levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			if (std::get<0>(collision)) // If collision is true
			{
				// Destroy block if not solid
				if (!box.IsSolid)
				{
					box.Destroyed = GL_TRUE;
					this->SpawnPowerUps(box);
					SoundEngine->play2D("audio/bleep.mp3", GL_FALSE);
				}
				else
				{   // if block is solid, enable shake effect
					ShakeTime = 0.05f;
					Effects->Shake = GL_TRUE;
					SoundEngine->play2D("audio/solid.wav", GL_FALSE);
				}
				// Collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (!(Ball->PassThrough && !box.IsSolid)) // don't do collision resolution on non-solid bricks if pass-through activated
				{
					if (dir == LEFT || dir == RIGHT) // Horizontal collision
					{
						Ball->Velocity.x = -Ball->Velocity.x; // Reverse horizontal velocity
															  // Relocate
						GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
						if (dir == LEFT)
							Ball->Position.x += penetration; // Move ball to right
						else
							Ball->Position.x -= penetration; // Move ball to left;
					}
					else // Vertical collision
					{
						Ball->Velocity.y = -Ball->Velocity.y; // Reverse vertical velocity
															  // Relocate
						GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
						if (dir == UP)
							Ball->Position.y -= penetration; // Move ball bback up
						else
							Ball->Position.y += penetration; // Move ball back down
					}
				}
			}
		}
	}

	// Also check collisions on PowerUps and if so, activate them
	for (PowerUp &powerUp : this->PowerUps)
	{
		if (!powerUp.Destroyed)
		{
			// First check if powerup passed bottom edge, if so: keep as inactive and destroy
			if (powerUp.Position.y >= this->windowHeight)
				powerUp.Destroyed = GL_TRUE;

			if (CheckCollision(*Player, powerUp))
			{	// Collided with player, now activate powerup
				ActivatePowerUp(powerUp);
				powerUp.Destroyed = GL_TRUE;
				powerUp.Activated = GL_TRUE;
				SoundEngine->play2D("audio/powerup.wav", GL_FALSE);
			}
		}
	}

	// And finally check collisions for player pad (unless stuck)
	Collision result = CheckCollision(*Ball, *Player);
	if (!Ball->Stuck && std::get<0>(result))
	{
		// Check where it hit the board, and change velocity based on where it hit the board
		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
		GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
		GLfloat percentage = distance / (Player->Size.x / 2);
		// Then move accordingly
		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		//Ball->Velocity.y = -Ball->Velocity.y;
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); // Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
																					// Fix sticky paddle
		Ball->Velocity.y = -1 * abs(Ball->Velocity.y);

		// If Sticky powerup is activated, also stick ball to paddle once new velocity vectors were calculated
		Ball->Stuck = Ball->Sticky;

		SoundEngine->play2D("audio/bleep.wav", GL_FALSE);
	}
}

GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
	// Collision x-axis?
	GLboolean collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// Collision y-axis?
	GLboolean collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// Collision only if on both axes
	return collisionX && collisionY;
}

Collision CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
	// Get center point circle first 
	glm::vec2 center(one.Position + one.Radius);
	// Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
	// Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Now that we know the the clamped values, add this to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// Now retrieve vector between center circle and closest point AABB and check if length < radius
	difference = closest - center;

	if (glm::length(difference) < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	else
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

// Calculates which direction a vector is facing (N,E,S or W)
Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

void TTQ::OnKeyCallback(int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			Keys[key] = GL_FALSE;
	}
}

void TTQ::Quit()
{
	ResourceManager::Clear();
}

