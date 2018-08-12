#include "Engine.h"

//Template class that encaspulates any Drawable object
//Allows user to alter the Drawable object without waiting for the renderer
template<class T>
class GameObject
{
private:
	//A boolean for workerThreads to know when should they terminate
	static bool terminate, *startedWorking;

	//Pointer to an array of pointers pointing to workerThreads
	static std::thread** workerThreads;

	//Mutex for accessing list identifier and array of mutecies for workerThreads
	static std::mutex listIdentifierMutex, *workerThreadsMutex; 
	
	//Condition variable for workerThreads to sleep on
	static std::condition_variable cv;

	//Used by the workerThread to know what index in the updateList to work on
	static int listIdentifer;

	//The workerThread function
	static void workerThread(int);

	//Cleans up the threading mess when the program is terminating
	static void cleanUp();

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
