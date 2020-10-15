#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"
#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class b2World;
class b2Body;

// TODO 6: Create a small class that keeps a pointer to tghe b2Body
// and has a method to request the position
// then write the implementation in the .cpp
// Then make your circle creation function to return a pointer to that class


class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	// TODO 4: Move body creation to 3 functions to create circles, rectangles and chains
	void CreateCircle(float x, float y, float rad)
	{
		b2BodyDef body;
		body.type = b2_dynamicBody;
		float radius = PIXEL_TO_METERS(rad);
		body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

		b2Body* b = world->CreateBody(&body);

		b2CircleShape shape;
		shape.m_radius = radius;
		b2FixtureDef fixture;
		fixture.shape = &shape;

		b->CreateFixture(&fixture);
	}
	void CreateBox(float x, float y)
	{
		b2BodyDef body1;
		body1.type = b2_dynamicBody;

		b2Vec2 vertexs[4];
		vertexs[0].Set(0.0f, 0.0f);
		vertexs[1].Set(1.0f, 0.0f);
		vertexs[2].Set(1.0f, 1.0f);
		vertexs[3].Set(0.0f, 1.0f);

		int32 count = 4;
		body1.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
		b2Body* b = world->CreateBody(&body1);

		b2PolygonShape shape1;
		shape1.Set(vertexs,count);
		b2FixtureDef fixture1;
		fixture1.shape = &shape1;

		fixture1.density = 1.0f;
		b->CreateFixture(&fixture1);
	}
	void CreateChain(float x, float y)
	{
		b2BodyDef body;
		body.type = b2_dynamicBody;

		b2Vec2 vs[12];
		vs[0].Set(PIXEL_TO_METERS(-38.0f), PIXEL_TO_METERS(80.0f));
		vs[1].Set(PIXEL_TO_METERS(-44.0f), PIXEL_TO_METERS(-54.0f));
		vs[2].Set(PIXEL_TO_METERS(-16.0f), PIXEL_TO_METERS(-60.0f));
		vs[3].Set(PIXEL_TO_METERS(-16.0f), PIXEL_TO_METERS(-17.0f));
		vs[4].Set(PIXEL_TO_METERS(19.0f), PIXEL_TO_METERS(-19.0f));
		vs[5].Set(PIXEL_TO_METERS(19.0f), PIXEL_TO_METERS(-79.0f));
		vs[6].Set(PIXEL_TO_METERS(61.0f), PIXEL_TO_METERS(-77.0f));
		vs[7].Set(PIXEL_TO_METERS(57.0f), PIXEL_TO_METERS(73.0f));
		vs[8].Set(PIXEL_TO_METERS(17.0f), PIXEL_TO_METERS(78.0f));
		vs[9].Set(PIXEL_TO_METERS(20.0f), PIXEL_TO_METERS(16.0f));
		vs[10].Set(PIXEL_TO_METERS(-25.0f), PIXEL_TO_METERS(13.0f));
		vs[11].Set(PIXEL_TO_METERS(-9.0f), PIXEL_TO_METERS(72.0f));

		body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

		b2Body* b = world->CreateBody(&body);

		b2ChainShape chain;
		chain.CreateLoop(vs, 12);

		b2FixtureDef fixture;
		fixture.shape = &chain;

		b->CreateFixture(&fixture);
	}
	
private:

	bool debug;
	b2World* world;
};