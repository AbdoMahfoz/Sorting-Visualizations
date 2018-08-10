#include "Engine.h"
#include "GameObject.cpp"

GameObject<RectangleShape> *gS, *gX;
bool ReverseX = false, ReverseY = false;
bool IReverseX = true, IReverseY = true;

void Animate()
{
	engine->SetTitle("Framerate = " + std::to_string(1.0f/ engine->GetDeltaTime()));
	gX->getCoreObject().move(256.0f * engine->GetDeltaTime() + (-512.0f * engine->GetDeltaTime() * IReverseX),
		256.0f * engine->GetDeltaTime() + (-512.0f * engine->GetDeltaTime() * IReverseY));
	gS->getCoreObject().move(256.0f * engine->GetDeltaTime() + (-512.0f * engine->GetDeltaTime() * ReverseX),
		256.0f * engine->GetDeltaTime() + (-512.0f * engine->GetDeltaTime() * ReverseY));
	if (gX->getReadOnlyCoreObject().getPosition().x >= SCREEN_WIDTH - 50)
	{
		IReverseX = true;
	}
	else if (gX->getReadOnlyCoreObject().getPosition().x <= 50)
	{
		IReverseX = false;
	}
	if (gX->getReadOnlyCoreObject().getPosition().y >= SCREEN_HEIGHT - 50)
	{
		IReverseY = true;
	}
	else if (gX->getReadOnlyCoreObject().getPosition().y <= 50)
	{
		IReverseY = false;
	}
	if (gS->getReadOnlyCoreObject().getPosition().x >= SCREEN_WIDTH - 50)
	{
		ReverseX = true;
	}
	else if (gS->getReadOnlyCoreObject().getPosition().x <= 50)
	{
		ReverseX = false;
	}
	if (gS->getReadOnlyCoreObject().getPosition().y >= SCREEN_HEIGHT - 50)
	{
		ReverseY = true;
	}
	else if (gS->getReadOnlyCoreObject().getPosition().y <= 50)
	{
		ReverseY = false;
	}
}

void CleanUpGameObjects()
{
	delete gX, gS;
}

void Start()
{
	RectangleShape s, x;
	//Setting values for rectangle s
	s.setFillColor(Color::Green);
	s.setPosition(150, 150);
	s.setSize(Vector2f(100, 100));
	s.setOrigin(Vector2f(50, 50));
	gS = new GameObject<RectangleShape>(0, s);
	//Setting values for rectangle x
	x.setFillColor(Color::Magenta);
	x.setPosition(500, 150);
	x.setSize(Vector2f(100, 100));
	x.setOrigin(Vector2f(50, 50));
	gX = new GameObject<RectangleShape>(0, x);
	//Registering Animation routine and OnClose
	engine->RegisterRoutine(Animate);
	engine->RegisterOnClose(CleanUpGameObjects);
}