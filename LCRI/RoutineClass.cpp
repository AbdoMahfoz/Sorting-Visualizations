#include "RoutineClass.h"

bool RoutineClass::IsIntialized = false;
std::vector < RoutineClass* > RoutineClass::Instances;

void RoutineClass::CallRoutines()
{
	//If there are no objects that inheret this class, we better off telling engine not to bother calling this function
	if (Instances.size() == 0)
	{
		engine->Log("Unregistering CallRoutines function");
		engine->UnRegisterRoutine(CallRoutines);
		IsIntialized = false;
		return;
	}
	//Iterate throught all instances to call Main function in them
	for (unsigned int i = 0; i < Instances.size(); i++)
	{
		Instances[i]->Main();
	}
}

RoutineClass::RoutineClass()
{
	//Check if CallRoutines is registered and act accordingly
	if (!IsIntialized)
	{
		engine->Log("Registering RoutineClass's CallRoutine");
		engine->RegisterRoutine(CallRoutines);
		IsIntialized = true;
	}
#pragma region Logging
	engine->ss << "Registering " << this << " class as a RoutineClass";
	engine->Log(engine->ss.str());
	engine->ss.str("");
#pragma endregion
	//Push this instance to the instances vector
	Instances.push_back(this);
}

RoutineClass::~RoutineClass()
{
	//Now that this object is being destroyed, trying to invoke the main function of this instance will cause errors
	//Even if it didn't cause any errors it will still cost us unnesscary time
	//So we iterate throught instances vector till we find this instance and erase it
#pragma region Logging
	engine->ss << "Attempting to unregister RoutineClass " << this;
	engine->Log(engine->ss.str());
	engine->ss.str("");
#pragma endregion
	for (unsigned int i = 0; i < Instances.size(); i++)
	{
		if (Instances[i] == this)
		{
			Instances.erase(Instances.begin() + i);
			return;
		}
	}
	engine->Log("[Error]Object not found");
}
