#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <Windows.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TITLE "Test"

using namespace sf;

//A class containing the engine
class Engine
{
private:
	//The window in which the game is displayed
	RenderWindow *MainWindow;
	
	//Clock for calculaing deltaTime
	Clock clock;

	//Actual Value of deltaTime
	float DeltaTime;

	//The amount of time that has passed since the beginning of the game
	long double ElapsedTime;

	//The main loop function
	void Main();

	//Array of vectors containing the objects
	//each vector is a layer, the goal is to separate objects
	//into various layer drawn atop of each other
	std::vector < Drawable* > Objects[7];

	//A vector for holding all routines
	//routines are functions that the engine run every frame
	std::vector < void(*)() > Routines;

	std::vector < void(*)() > Close;

	std::condition_variable cv;
	std::mutex m1, m2;

	std::thread *t;

	bool Terminate;

	//The rendering function
	void Render();

	//The functions that manage running routines sequentially
	void RoutineManager();
public:
	//Dummy stringstream for logging purpose
	std::stringstream ss;

	//Default constructor
	Engine(void (Engine::**MainPtr)());

	RenderWindow* GetWindow();

	//function used for logging
	void Log(std::string s);

	//Function for reading deltatime because it is a read only variable
	float GetDeltaTime();

	//Function for registering objects for drawing
	void RegisterObject(int Layer, Drawable* Object);

	//Function for deregistering previously registered object to stop renderer from rendering it
	void UnRegisterObject(int Layer, Drawable* Object);

	//Function for registering routines
	void RegisterRoutine(void(*routine)());

	//Function for deregistering previosuly registered routine to stop LogicManager from invoking it
	void UnRegisterRoutine(void(*routine)());

	void RegisterOnClose(void(*routine)());

	//Default Deconstructor for deallocating pointers
	~Engine();
};

//Called once at the beginning of the game
void Start();
//A Global pointer to be accessed by any source file that includes Engine.h
extern Engine *engine;