#include "Engine.h"

void Engine::Main()
{
	//Calling Start before entering into the window loop
	Start();
	//Main Loop
	while (MainWindow->isOpen())
	{
		//Handling Events...
		Event event;
		while (MainWindow->pollEvent(event))
		{
			switch (event.type)
			{
				//Window Closed Event
			case Event::Closed:
				MainWindow->close();
				break;
			}
		}
		//Logic
		RoutineManager();
		//Rendering
		Render();
	}
}

void Engine::Render()
{
	//Clear the window
	MainWindow->clear(Color::Black);
	//Main Rendering loop
	//iterates throught all registered objects and draw them layer by layer
	for (int i = 0; i < 7; i++)
	{
		for (unsigned int j = 0; j < Objects[i].size(); j++)
		{
			MainWindow->draw(*Objects[i][j]);
		}
	}
	//Display the drawn contents;
	MainWindow->display();
}

void Engine::RoutineManager()
{
	//Call all reserved routines
	for (unsigned int i = 0; i < Routines.size(); i++)
	{
		Routines[i]();
	}
}

Engine::Engine(void (Engine::**MainPtr)())
{
	//Intialization of window
	MainWindow = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), TITLE);
	//Activating Vsync
	MainWindow->setVerticalSyncEnabled(true);
	//Assiging pointer to main function so that it can call it later
	//Note that this is the only way the main function can be called from outside of the class
	*MainPtr = &Engine::Main;
}

void Engine::RegisterObject(int Layer, Drawable* Object)
{
	//Registering object into specified layer
	Objects[Layer].push_back(Object);
}

void Engine::UnRegisterObject(int Layer, Drawable* Object)
{
	//Removing the object from the layer in which it resides
	for (unsigned int i = 0; i < Objects[Layer].size(); i++)
	{
		if (Objects[Layer][i] == Object)
		{
			Objects[Layer].erase(Objects[Layer].begin() + i);
			return;
		}
	}
}

void Engine::RegisterRoutine(void(*routine)())
{
	//Registering routine
	Routines.push_back(routine);
}

void Engine::UnRegisterRoutine(void(*routine)())
{
	//Removing the routine from the vector
	for (unsigned int i = 0; i < Routines.size(); i++)
	{
		if (Routines[i] == routine)
		{
			Routines.erase(Routines.begin() + i);
			return;
		}
	}
}

Engine::~Engine()
{
	//Deallocating variables
	delete MainWindow;
}