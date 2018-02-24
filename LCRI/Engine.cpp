#include "Engine.h"

void Engine::Main()
{
	//Calling Start before entering into the window loop
	Start();
	//Main Loop
	int n = 0;
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
				Log("Closed event triggered");
				for (unsigned int i = 0; i < Close.size(); i++)
				{
					Close[i]();
				}
				//Terminate = true;
				//cv.notify_one();
				//t->join();
				//delete t;
				return;
			}
		}
		//Logic
		RoutineManager();
		//std::lock_guard<std::mutex> l(m2);
		//cv.notify_one();
		Render();
	}
}

void Engine::Render()
{
	//MainWindow->setActive(true);
	//std::unique_lock<std::mutex> ul(m1);
	//Activating Vsync
	//Log("Enabling Vsync");
	//MainWindow->setVerticalSyncEnabled(true);
	//while (!Terminate)
	//{
		//cv.wait(ul);
		//std::lock_guard < std::mutex > l(m2);
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
		//Calcualting deltaTime...
		DeltaTime = clock.restart().asSeconds();
		ElapsedTime += DeltaTime;
	//}
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
	//Setting initaial values
	ElapsedTime = 0;
	DeltaTime = 0;
	Terminate = false;
	//Intialization of window
#pragma region Logging
	ss << "Intializing window : Width = " << SCREEN_WIDTH << ", Height = " << SCREEN_HEIGHT << ", Title = " << TITLE;
	Log(ss.str());
	ss.str("");
#pragma endregion
	MainWindow = new RenderWindow(VideoMode::getDesktopMode(), TITLE, Style::Fullscreen);
	MainWindow->setVerticalSyncEnabled(true);
	//MainWindow->setActive(false);
	//t = new std::thread(&Engine::Render, this);
	//Assiging pointer to main function so that it can call it later
	//Note that this is the only way the main function can be called from outside of the class
	*MainPtr = &Engine::Main;
}

RenderWindow * Engine::GetWindow()
{
	return MainWindow;
}

void Engine::Log(std::string s)
{
	/*
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
	*/
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
	std::lock_guard < std::mutex > l(m2);
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

void Engine::RegisterOnClose(void(*routine)())
{
	Close.push_back(routine);
}

Engine::~Engine()
{
	//Deallocating variables
	Log("Deallocating MainWindow");
	delete MainWindow;
}