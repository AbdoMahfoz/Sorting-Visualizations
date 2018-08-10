#pragma once
#include <vector>

//Template class that encaspulates any Drawable object
//Allows user to alter the Drawable object without waiting for the renderer
template<class T>
class GameObject
{
private:
	//Contains GameObject instances that the Drawable of which has been modified
	static std::vector<GameObject*> updateList;

	//A boolean to check if the template function of this class is registered in the AfterFrame routine list
	static bool isRegistered;

	//An AfterFrame routine that copies Drawable data from logic drawable to rendering drawable
	static void update();

	//A flag used to not push the same instance into the updateList twice
	bool isInUpdateList;

	//Two instances of the Drawable, one for the logic thread and one for the rendering thread
	T buffered[2];
public:
	//Creates a drawable via its default constructor
	GameObject(int layer);

	//Creates a drawable that is equivelant to the given instance
	GameObject(int layer, const T&);

	//Returns the encapsulated drawable and registers this instance in the updateList
	//This means that after the frame ends the logic drawable will be copied over to the rendering drawable
	//Use only if you are going to make changes to the drawable
	T& getCoreObject();

	//Returns the encapsulated drawable without registering this in the updateList
	//Use only if you are not going to modify the drawable
	const T& getReadOnlyCoreObject();
};
