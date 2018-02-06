#include "Engine.h"

using namespace sf;

Engine* engine;

int main()
{
	void (Engine::*ptr)() = nullptr;
	engine = new Engine(&ptr);
	(engine->*ptr)();
	delete engine;
	return 0;
}