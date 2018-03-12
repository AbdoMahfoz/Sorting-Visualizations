#include "Engine.h"
//#include <Windows.h>

Engine* engine;

void Intialize()
{
	void (Engine::*ptr)() = nullptr;
	engine = new Engine(&ptr);
	(engine->*ptr)();
	delete engine;
}

/*int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
	Intialize();
	return 0;
}
*/

int main()
{
	Intialize();
	return 0;
}