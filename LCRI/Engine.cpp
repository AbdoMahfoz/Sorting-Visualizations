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
				//Signaling to the Render thread and Logging thread to hault
				Terminate = true;
				RenderCV.notify_one();
				LogCV.notify_one();
				LogThread->join();
				RenderThread->join();
				delete RenderThread;
				delete LogThread;
				//Calling all OnClose function
				for (unsigned int i = 0; i < Close.size(); i++)
				{
					Close[i]();
				}
				return;
			}
		}
		//Logic
		RoutineManager();
		//Signaling Render function to work
		RenderCV.notify_all();
	}
}

void Engine::Render()
{
	//Assigns MainWindow to this thread
	MainWindow->setActive(true);
	//Creating the lock for RenderMutex
	std::unique_lock<std::mutex> lock(RenderMutex);
	//Activating Vsync
	Log("Enabling Vsync");
	MainWindow->setVerticalSyncEnabled(true);
	while (!Terminate)
	{
		RenderCV.wait(lock);
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
	}
	//Closing Window
	MainWindow->close();
}

void Engine::RoutineManager()
{
	//Call all reserved routines
	for (unsigned int i = 0; i < Routines.size(); i++)
	{
		Routines[i]();
	}
}

void Engine::LogHelper()
{
	std::unique_lock<std::mutex> lock(LogMutex, std::defer_lock);
	while (!Terminate)
	{
		if (LogQueue.empty())
		{
			lock.lock();
			LogCV.wait(lock);
			lock.unlock();
			if (LogQueue.empty())
			{
				continue;
			}
		}
		static bool First = true;
		lock.lock();
		std::cout << "[" << ElapsedTime << "] " << LogQueue.front() << '\n';
		lock.unlock();
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
		lock.lock();
		out << "[" << ElapsedTime << "] " << LogQueue.front() << '\n';
		LogQueue.pop();
		lock.unlock();
		out.close();
	}
}

Engine::Engine(void (Engine::**MainPtr)())
{
	//Setting initaial values
	ElapsedTime = 0;
	DeltaTime = 0;
	Terminate = false;
#pragma region Logging
	ss << "Intializing window : Width = " << SCREEN_WIDTH << ", Height = " << SCREEN_HEIGHT << ", Title = " << TITLE;
	Log(ss.str());
	ss.str("");
#pragma endregion
	//Intialization of window
	MainWindow = new RenderWindow(VideoMode::getDesktopMode(), TITLE, Style::Fullscreen);
	//Deactivating Window from Main Thread
	MainWindow->setActive(false);
	//Launching RenderThread
	RenderThread = new std::thread(&Engine::Render, this);
	LogThread = new std::thread(&Engine::LogHelper, this);
	//Assiging pointer to main function so that it can call it later
	//Note that this is the only way the main function can be called from outside of the class
	*MainPtr = &Engine::Main;
}

void Engine::Log(std::string s)
{
	{
		std::lock_guard<std::mutex> lock(LogMutex);
		LogQueue.push(s);
	}
	LogCV.notify_one();
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
	//Locking RenderMutex to register an object
	std::lock_guard < std::mutex > l(RenderMutex);
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

void Engine::RegisterOnClose(void(*func)())
{
	//Pushes the function to the Close vector to be called as the game exists
	Close.push_back(func);
}

void Engine::UnRegisterOnClose(void(*func)())
{
#pragma region Logging
	ss << "Attempting to find and unregister OnClose func " << func;
	Log(ss.str());
	ss.str("");
#pragma endregion
	//Removing the routine from the vector
	for (unsigned int i = 0; i < Close.size(); i++)
	{
		if (Close[i] == func)
		{
			Log("OnClose found and erased");
			Close.erase(Close.begin() + i);
			return;
		}
	}
	Log("[Error]OnClose not found");
}

void Engine::LockRendering()
{
	RenderMutex.lock();
}

void Engine::UnlockRendering()
{
	RenderMutex.unlock();
}

Engine::~Engine()
{
	//Deallocating variables
	Log("Deallocating MainWindow");
	delete MainWindow;
}