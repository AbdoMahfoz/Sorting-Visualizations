#include "Engine.h"

RectangleShape s;
RectangleShape x;
bool ReverseX = false, ReverseY = false;
bool IReverseX = true, IReverseY = true;

void Animate()
{
	x.move(256.0f * engine->GetDeltaTime() + (-512.0f * engine->GetDeltaTime() * IReverseX), 256.0f * engine->GetDeltaTime() + (-512.0f * engine->GetDeltaTime() * IReverseY));
	s.move(256.0f * engine->GetDeltaTime() + (-512.0f * engine->GetDeltaTime() * ReverseX), 256.0f * engine->GetDeltaTime() + (-512.0f * engine->GetDeltaTime() * ReverseY));
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

//Called once at the beginning of the game
void Start()
{
	s.setFillColor(Color::Green);
	s.setPosition(150, 150);
	s.setSize(Vector2f(100, 100));
	s.setOrigin(Vector2f(50, 50));
	engine->RegisterObject(0, &s);
	engine->RegisterRoutine(Animate);
	x.setFillColor(Color::Magenta);
	x.setPosition(500, 150);
	x.setSize(Vector2f(100, 100));
	x.setOrigin(Vector2f(50, 50));
	engine->RegisterObject(0, &x);
}
