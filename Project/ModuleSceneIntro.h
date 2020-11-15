#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	p2List<PhysBody*> LeftFlipper;
	p2List<PhysBody*> RightFlipper;

	PhysBody* col1;
	PhysBody* col2;

	PhysBody* sensor;
	bool sensed;
	bool canJump = true;
	int count = 0;
	bool onScreen = true;
	bool onPosition = true;


	SDL_Texture* ball;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* back;
	SDL_Texture* kn;
	SDL_Texture* leftFlip;
	SDL_Texture* rightFlip;

	uint bonus_fx;
	uint back_fx;
	uint coin_fx;
	p2Point<int> ray;
	bool ray_on;
};
