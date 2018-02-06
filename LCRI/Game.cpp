#include "Engine.h"

RectangleShape s;
RectangleShape x;
bool ReverseX = false, ReverseY = false;
bool IReverseX = false, IReverseY = false;

void Animate()
{
	x.move(16.0f + (-32.0f * IReverseX), 16.0f + (-32.0f * IReverseY));
	if (x.getPosition().x >= SCREEN_WIDTH - 50 || x.getPosition().x <= 50)
	{
		IReverseX = !IReverseX;
	}
	if (x.getPosition().y >= SCREEN_HEIGHT - 50 || x.getPosition().y <= 50)
	{
		IReverseY = !IReverseY;
	}
	s.move(16.0f + (-32.0f * ReverseX), 16.0f + (-32.0f * ReverseY));
	if (s.getPosition().x >= SCREEN_WIDTH - 50 || s.getPosition().x <= 50)
	{
		ReverseX = !ReverseX;
	}
	if (s.getPosition().y >= SCREEN_HEIGHT - 50 || s.getPosition().y <= 50)
	{
		ReverseY = !ReverseY;
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
