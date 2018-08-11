#include "Engine.h"

void Engine::Main()
{
	//Calling Start before entering into the window loop
	Start();
	//Main Loop
	int n = 0;
	//Start clock from here
	clock.restart();
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
		//Call all reserved routines
		for (unsigned int i = 0; i < Routines.size(); i++)
		{
			Routines[i]();
		}
		//Waiting for the renderer to finish it's current frame
		{
			std::lock_guard<std::mutex> lock(RenderMutex);
		}
		//Runnning AfterFrame Routines
		std::thread** threadPool = new std::thread*[AfterFrameRoutines.size()];
		for (unsigned int i = 0; i < AfterFrameRoutines.size(); i++)
		{
			threadPool[i] = new std::thread(AfterFrameRoutines[i]);
		}
		for (unsigned int i = 0; i < AfterFrameRoutines.size(); i++)
		{
			threadPool[i]->join();
			delete threadPool[i];
		}
		delete threadPool;
		//Calcualting deltaTime...
		DeltaTime = clock.restart().asSeconds();
		ElapsedTime += DeltaTime;
		//Signaling for the render to start working
		RenderCV.notify_one();
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
		//Getting into sleep state; waiting for the Main thread to finish
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
	}
	//Closing Window
	MainWindow->close();
}

void Engine::LogHelper()
{
	//A Lock for synchronoizing Queue access
	std::unique_lock<std::mutex> lock(LogMutex, std::defer_lock);
	while (!Terminate)
	{
		//Sleeping when we have nothing to do
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
		//A flag for knowing if the file has been opened before
		static bool First = true;
		//Output to Console
		lock.lock();
		std::cout << "[" << ElapsedTime << "] " << LogQueue.front() << '\n';
		//Output to file
		std::ofstream out;
		//Openeing file
		if (First)
		{
			out.open("log.txt");
			First = false;
		}
		else
		{
			out.open("log.txt", std::ios::app);
		}
		//Writing to file
		out << "[" << ElapsedTime << "] " << LogQueue.front() << '\n';
		//Clear the entry we just logged
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
	Log("Intializing window : Width = " + std::to_string(SCREEN_WIDTH) + ", Height = " + std::to_string(SCREEN_HEIGHT)
		+ ", Title = " + TITLE);
	//Intialization of window
	MainWindow = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), TITLE);
	//Deactivating Window from Main Thread
	MainWindow->setActive(false);
	//Launching RenderThread
	RenderThread = new std::thread(&Engine::Render, this);
	LogThread = new std::thread(&Engine::LogHelper, this);
	//Assiging pointer to main function so that it can call it later
	//Note that this is the only way the main function can be called from outside of the class
	*MainPtr = &Engine::Main;
}

void Engine::SetTitle(const std::string& title)
{
	MainWindow->setTitle(title);
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
	//Logging...
	ss << "Registering object in address " << Object << " into layer " << Layer;
	Log(ss.str());
	ss.str("");
	//Locking RenderMutex to register an object
	std::lock_guard < std::mutex > l(RenderMutex);
	//Registering object into specified layer
	Objects[Layer].push_back(Object);
}

void Engine::UnRegisterObject(int Layer, Drawable* Object)
{
	//Logging... 
	ss << "Attempting to remove object " << Object << " from layer " << Layer;
	Log(ss.str());
	ss.str("");
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
	//Logging....
	ss << "Registering routine " << routine;
	Log(ss.str());
	ss.str("");
	//Registering routine
	Routines.push_back(routine);
}

void Engine::RegisterAfterFrameRoutine(void(*routine)())
{
	//Logging....
	ss << "Registering routine " << routine;
	Log(ss.str());
	ss.str("");
	//Registering routine
	AfterFrameRoutines.push_back(routine);
}

void Engine::UnRegisterRoutine(void(*routine)())
{
	ss << "Attempting to find and unregister routine " << routine;
	Log(ss.str());
	ss.str("");
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
	ss << "Attempting to find and unregister OnClose func " << func;
	Log(ss.str());
	ss.str("");
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

void Engine::WaitForRenderer()
{
	//Waiting for the renderer by locking and unlocking RenderMutex
	std::lock_guard<std::mutex> lock(RenderMutex);
}

Engine::~Engine()
{
	//Deallocating variables
	Log("Deallocating MainWindow");
	delete MainWindow;
}