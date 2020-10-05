#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include "Box2D/Box2D/Box2D.h"
// TODO 1: Include Box 2 header and library

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it

	b2Vec2 Gravity(0.0f, -10.0f);	
	ModulePhysics::world = new b2World (Gravity);
	b2World *tonto = ModulePhysics::world;

	// TODO 4: Create a a big static circle as "ground"
	b2BodyDef ground;
	ground.position.Set(0.0f, -10.0f);

	b2World tonto(Gravity);
	b2Body *groundBody = tonto->CreateBody(&ground); 

	b2CircleShape groundCircle;

	groundCircle.m_p.Set(2.0f, 3.0f);
	groundCircle.m_radius = 0.5f;

	groundBody->CreateFixture(&groundCircle, 0.0f);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)
	float32 timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	b2World* tonto = ModulePhysics::world;
	/*
	for (int32 i = 0; i < 60; ++i)
	{
		tonto->Step(timeStep, velocityIterations, positionIterations);
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();
		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}
	*/

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	/*
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}*/

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!

	return true;
}


