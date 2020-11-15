#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	ball = App->textures->Load("assets/ball.png");
	kn = App->textures->Load("assets/knkro.png");
	box = App->textures->Load("assets/crate.png");
	rick = App->textures->Load("assets/rick_head.png");
	bonus_fx = App->audio->LoadFx("assets/bonus.wav");
	coin_fx = App->audio->LoadFx("assets/coin.wav");
	back = App->textures->Load("assets/mapa.png");
	leftFlip = App->textures->Load("assets/palancabona.png");
	rightFlip = App->textures->Load("assets/palancabona.png");
	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//sensor = App->physics->CreateRectangleSensor(0 , 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	int mapa[48] = {
		268, 86,
		750, 86,
		750, 705,
		702, 705,
		702, 637,
		599, 675,
		590, 667,
		701, 627,
		701, 171,
		708, 162,
		720, 170,
		720, 694,
		743, 694,
		743, 136,
		738, 123,
		727, 109,
		717, 103,
		703, 97,
		687, 93,
		277, 93,
		277, 627,
		388, 668,
		370, 679,
		267, 637
	};

	int pent[10] = {
		615, 484,
		665, 491,
		674, 542,
		630, 566,
		593, 530
	};
	// Pivot 0, 0
	int pent2[10] = {
		342, 479,
		389, 498,
		386, 549,
		337, 562,
		310, 518
	};
	// Pivot 0, 0
	int line1[8] = {
		505, 327,
		586, 396,
		588, 393,
		507, 325
	};

	// Pivot 0, 0
	int line2[4] = {
		693, 325,
		612, 394
	};
	// Pivot 0, 0
	int circ1[18] = {
		306, 299,
		309, 310,
		314, 322,
		323, 335,
		332, 345,
		344, 355,
		355, 361,
		367, 365,
		375, 367
	};
	// Pivot 0, 0
	/*int circ2[20] = {
		409, 367,
		416, 366,
		425, 363,
		438, 357,
		448, 350,
		457, 343,
		465, 333,
		472, 322,
		478, 310,
		480, 299
	};*/

	// Pivot 0.000000, 0.000000

	App->physics->CreateChain(-5, -75, mapa, 48, b2_staticBody);
	App->physics->CreateChain(-5, -75, pent, 10, b2_staticBody);
	App->physics->CreateChain(-5, -75, line2, 4, b2_staticBody);
	App->physics->CreateChain(-5, -75, line1, 8, b2_staticBody);
	App->physics->CreateChain(-5, -75, pent2, 10, b2_staticBody);

	App->physics->CreateChain(-5, -75, circ1, 18, b2_staticBody);
//	App->physics->CreateChain(-5, -75, circ2, 20, b2_staticBody);

	App->physics->CreateCircle(416, 175, 17,b2_staticBody);
	App->physics->CreateCircle(354, 115, 17,b2_staticBody);
//	App->physics->CreateCircle(483, 115, 17,b2_staticBody);

	circles.add(App->physics->CreateCircle(720,600, 10, b2_dynamicBody));
	circles.getLast()->data->listener = this;

	App->physics->CreateCircle(595, 253, 15, b2_staticBody);
	App->physics->CreateCircle(595, 216, 15, b2_staticBody);

	LeftFlipper.add(App->physics->lFlip(390, 666, 80, 20, 383, 610));
	LeftFlipper.getLast()->data->listener = this;

	RightFlipper.add(App->physics->rFlip(592, 666, 80, 20, 585, 605));
	RightFlipper.getLast()->data->listener = this;

	col1 = App->physics->CreateCircleSensor(595, 253, 15, b2_staticBody);
	col2 = App->physics->CreateCircleSensor(595, 216, 15, b2_staticBody);
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(back, -5, -75, NULL, NULL, NULL);
	b2Vec2 force(0, -200);
	b2Vec2 speed(0, 0);
	b2Vec2 position = circles.getFirst()->data->body->GetPosition();

	if (position.y < 20)
	{
		onScreen = true;
	}
	else
	{
		onScreen = false;
	}
	if (onScreen == false)
	{
		circles.clear();
		circles.add(App->physics->CreateCircle(720, 600, 10, b2_dynamicBody));
		circles.getLast()->data->listener = this;
		canJump = true;
		count = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		count++;
	}
	if (count == 1)
	{
		canJump = true;
	}
	else
	{
		canJump = false;
	}
	if (circles.getFirst()->data->body->GetLinearVelocity() == speed)
	{
		canJump = true;
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && canJump && circles.getFirst()->data->body->GetLinearVelocity() == speed)
	{
		circles.getFirst()->data->body->ApplyForce(force, circles.getFirst()->data->body->GetPosition(), true);
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10, b2_dynamicBody));
		circles.getLast()->data->listener = this;
	}
	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		LeftFlipper.getFirst()->data->body->ApplyTorque(-2000.0f, true);
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		RightFlipper.getFirst()->data->body->ApplyTorque(2000.0f, true);
	}

	
	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		//ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
			App->renderer->Blit(ball, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT)
	{
		p2List_item<PhysBody*>* k = circles.getFirst();
		while (k != NULL)
		{
			int x, y;
			k->data->GetPosition(x, y);
			App->renderer->Blit(kn, x, y, NULL, 1.0f, k->data->GetRotation());
			k = k->next;
		}
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = RightFlipper.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rightFlip, x-6, y-7, NULL, 1.0f, c->data->GetRotation()+180);
		c = c->next;
	}

	c = LeftFlipper.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(leftFlip, x - 6, y - 7, NULL, 1.0f, c->data->GetRotation() - 180);
		c = c->next;
	}
	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;
	App->audio->PlayFx(coin_fx);

	if (bodyB == col1 || bodyB == col2)
	{
		b2Vec2 lv(circles.getFirst()->data->body->GetLinearVelocity());

		lv.x = -lv.x * 2;
		lv.y = -lv.y * 2;

		circles.getFirst()->data->body->ApplyForce(lv, circles.getFirst()->data->body->GetPosition(), true);

	}
}