#include "GameObject.h"
#include "Engine.h"

template<class T>
std::vector<GameObject<T>*> GameObject<T>::updateList;
template<class T>
bool GameObject<T>::isRegistered = false;

template<class T>
void GameObject<T>::update()
{
	//Iterate through the updateList
	for (GameObject<T>* o : updateList)
	{
		//Copy the date of the logic drawable to the rendering drawable
		o->buffered[1] = o->buffered[0];
		//Mark it as no longer in the updateList
		o->isInUpdateList = false;
	}
	//Now that everything in the updateList is updated, clear it
	updateList.clear();
}

template<class T>
GameObject<T>::GameObject(int layer)
{
	//Check if the static update function of this template instance is registered in the AfterFrame routine list
	if (!isRegistered)
	{
		engine->RegisterAfterFrameRoutine(update);
		isRegistered = true;
	}
	//Initializations...
	isInUpdateList = false;
	buffered[0] = buffered[1];
	//Register the rendering drawable in the engine
	engine->RegisterObject(layer, &buffered[1]);
}

template<class T>
GameObject<T>::GameObject(int layer, const T& o)
{
	//Check if the static update function of this template instance is registered in the AfterFrame routine list
	if (!isRegistered)
	{
		engine->RegisterAfterFrameRoutine(update);
		isRegistered = true;
	}
	//Initializations...
	isInUpdateList = false;
	buffered[0] = buffered[1] = o;
	//Register the rendering drawable in the engine
	engine->RegisterObject(layer, &buffered[1]);
}

template<class T>
T& GameObject<T>::getCoreObject()
{
	//Attempts to register this instance into the updateList only if it wasnt already in there
	if (!isInUpdateList)
	{
		updateList.push_back(this);
		isInUpdateList = true;
	}
	return buffered[0];
}

template<class T>
const T& GameObject<T>::getReadOnlyCoreObject()
{
	return buffered[0];
}