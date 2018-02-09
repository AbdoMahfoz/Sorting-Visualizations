#pragma once
#include "Engine.h"

//A class that enables routiens within classes
//when inhereted, it will treat a "void Main()" function as a routine
class RoutineClass
{
private:
	//A flag to see if CallRoutines was registered as a routine or no
	static bool IsIntialized;

	//A vector that keeps track of all children to call the "Main" function in them later
	static std::vector < RoutineClass* > Instances;

	//The function that gets registered as a routine only to call all "Main" function within it's childs
	static void CallRoutines();
protected:
	//The Main function that becomes a routine once overloaded
	virtual void Main() = 0;

	//Default constructor; handles registering instance and checking if CallRoutines is registered
	RoutineClass();

	//Deconstructor; unregisters this instance from Instances vector
	~RoutineClass();
};

