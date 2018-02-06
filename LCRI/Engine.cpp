#include "Engine.h"

void Engine::Main()
{
	//Calling Start before entering into the window loop
	Start();
	//Main Loop
	int n = 0;
	while (MainWindow->isOpen())
	{
		//Calcualting deltaTime...
		DeltaTime = clock.restart().asSeconds();
		ElapsedTime += DeltaTime;
		//Handling Events...
		Event event;
		while (MainWindow->pollEvent(event))
		{
			switch (event.type)
			{
				//Window Closed Event
			case Event::Closed:
				Log("Closed event triggered");
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

void Engine::Log(std::string s)
{
	static bool First = true;
	std::cout << "[" << ElapsedTime << "] " << s << '\n';
	std::ofstream out;
	if (First)
	{
		out.open("log.txt");
		First = false;
	}
	else
	{
		out.open("log.txt", std::ios::app);
	}
	out << "[" << ElapsedTime << "] " << s << '\n';
	out.close();
}

Engine::Engine(void (Engine::**MainPtr)())
{
	//Setting initaial values
	ElapsedTime = 0;
	DeltaTime = 0;
	//Intialization of window
#pragma region Logging
	ss << "Intializing window : Width = " << SCREEN_WIDTH << ", Height = " << SCREEN_HEIGHT << ", Title = " << TITLE;
	Log(ss.str());
	ss.str("");
	#pragma endregion
	MainWindow = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), TITLE);
	//Activating Vsync
	Log("Enabling Vsync");
	MainWindow->setVerticalSyncEnabled(true);
	//Assiging pointer to main function so that it can call it later
	//Note that this is the only way the main function can be called from outside of the class
	*MainPtr = &Engine::Main;
}

float Engine::GetDeltaTime()
{
	return DeltaTime;
}

void Engine::RegisterObject(int Layer, Drawable* Object)
{
#pragma region Logging
	ss << "Registering object in address " << Object << " into layer " << Layer;
	Log(ss.str());
	ss.str("");
#pragma endregion
	//Registering object into specified layer
	Objects[Layer].push_back(Object);
}

void Engine::UnRegisterObject(int Layer, Drawable* Object)
{
#pragma region Logging
	ss << "Attempting to remove object " << Object << " from layer " << Layer;
	Log(ss.str());
	ss.str("");
#pragma endregion
	//Removing the object from the layer in which it resides
	for (unsigned int i = 0; i < Objects[Layer].size(); i++)
	{
		if (Objects[Layer][i] == Object)
		{
			Log("Object found and being removed");
			Objects[Layer].erase(Objects[Layer].begin() + i);
			return;
		}
	}
	Log("[Error]Object not found");
}

void Engine::RegisterRoutine(void(*routine)())
{
#pragma region
	ss << "Registering routine " << routine;
	Log(ss.str());
	ss.str("");
#pragma endregion
	//Registering routine
	Routines.push_back(routine);
}

void Engine::UnRegisterRoutine(void(*routine)())
{
#pragma region Logging
	ss << "Attempting to find and unregister routine " << routine;
	Log(ss.str());
	ss.str("");
#pragma endregion
	//Removing the routine from the vector
	for (unsigned int i = 0; i < Routines.size(); i++)
	{
		if (Routines[i] == routine)
		{
			Log("Routine found and erased");
			Routines.erase(Routines.begin() + i);
			return;
		}
	}
	Log("[Error]Routine not found");
}

Engine::~Engine()
{
	//Deallocating variables
	Log("Deallocating MainWindow");
	delete MainWindow;
}