#include "Engine.h"
#include "RoutineClass.h"

RectangleShape s;
RectangleShape x;
bool ReverseX = false, ReverseY = false;
bool IReverseX = true, IReverseY = true;

void Animate()
{
	//Moving objects based on their respective booleans
	//Each boolean decides the direction in which the object will slide
	//engine->ss << "Framerate = " << (1.0f / engine->GetDeltaTime());
	//engine->Log(engine->ss.str());
	//engine->ss.str("");
	engine->WaitForRenderer();
	x.move(256.0f * engine->GetDeltaTime() + (-512.0f * engine->GetDeltaTime() * IReverseX),
		256.0f * engine->GetDeltaTime() + (-512.0f * engine->GetDeltaTime() * IReverseY));
	s.move(256.0f * engine->GetDeltaTime() + (-512.0f * engine->GetDeltaTime() * ReverseX),
		256.0f * engine->GetDeltaTime() + (-512.0f * engine->GetDeltaTime() * ReverseY));
	//A series of if condition to check if our object hit the boarder of the window
	if (x.getPosition().x >= SCREEN_WIDTH - 50)
	{
		IReverseX = true;
	}
	else if (x.getPosition().x <= 50)
	{
		IReverseX = false;
	}
	if (x.getPosition().y >= SCREEN_HEIGHT - 50)
	{
		IReverseY = true;
	}
	else if (x.getPosition().y <= 50)
	{
		IReverseY = false;
	}
	if (s.getPosition().x >= SCREEN_WIDTH - 50)
	{
		ReverseX = true;
	}
	else if (s.getPosition().x <= 50)
	{
		ReverseX = false;
	}
	if (s.getPosition().y >= SCREEN_HEIGHT - 50)
	{
		ReverseY = true;
	}
	else if (s.getPosition().y <= 50)
	{
		ReverseY = false;
	}
}

void Start()
{
	//Setting values for rectangle s
	s.setFillColor(Color::Green);
	s.setPosition(150, 150);
	s.setSize(Vector2f(100, 100));
	s.setOrigin(Vector2f(50, 50));
	//Setting values for rectangle x
	x.setFillColor(Color::Magenta);
	x.setPosition(500, 150);
	x.setSize(Vector2f(100, 100));
	x.setOrigin(Vector2f(50, 50));
	//Registering objects and Animation routine
	engine->RegisterObject(0, &s);
	engine->RegisterRoutine(Animate);
	engine->RegisterObject(0, &x);
}