#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include "ModuleInput.h"

// TODO 1: Include Box 2 header and library

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
	ModulePhysics::world = new b2World(ModulePhysics::gravity);
}

// Destructor
ModulePhysics::~ModulePhysics()
{
	delete ModulePhysics::world;
	ModulePhysics::world = nullptr;
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it


	// TODO 4: Create a a big static circle as "ground"
	b2BodyDef body_Def;
	body_Def.type = b2_staticBody;
	//body_Def.position.Set(PIXELS_TO_METERS(490), PIXELS_TO_METERS(425));
	body_Def.position.Set(PIXELS_TO_METERS(SCREEN_WIDTH / 2), PIXELS_TO_METERS(SCREEN_HEIGHT / 2));
	b2Body* body = ModulePhysics::world->CreateBody(&body_Def);

	b2CircleShape shape;
	shape.m_radius = PIXELS_TO_METERS(200);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	body->CreateFixture(&fixture);

	
	
	


	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)
	ModulePhysics::world->Step(1.0f / 60.0f, 8, 3);
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		b2BodyDef body_Mov;
		body_Mov.type = b2_dynamicBody;
		body_Mov.position.Set(PIXELS_TO_METERS(App->input->GetMouseX()), PIXELS_TO_METERS(App->input->GetMouseY()));
		body_Mov.gravityScale = -10.0f;
		body_Mov.active = true;

		b2Body* bodyMov = ModulePhysics::world->CreateBody(&body_Mov);

		b2CircleShape shape2;
		shape2.m_radius = PIXELS_TO_METERS(20);
		b2FixtureDef fixture2;
		fixture2.shape = &shape2;
		bodyMov->CreateFixture(&fixture2);
	}

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
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

					b2CircleShape* shape2 = (b2CircleShape*)f->GetShape();
				}
				break;

				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}


	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!

	return true;
}
