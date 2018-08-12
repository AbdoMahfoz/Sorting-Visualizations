#include "GameObject.h"

template<class T>
bool GameObject<T>::terminate = false;
template<class T>
bool GameObject<T>::startedWorking = false;
template<class T>
int GameObject<T>::listIdentifer = -1;
template<class T>
std::mutex GameObject<T>::listIdentifierMutex;
template<class T>
std::mutex* GameObject<T>::workerThreadsMutex = nullptr;
template<class T>
std::condition_variable GameObject<T>::cv;
template<class T>
std::thread** GameObject<T>::workerThreads = nullptr;
template<class T>
std::vector<GameObject<T>*> GameObject<T>::updateList;
template<class T>
bool GameObject<T>::isRegistered = false;

template<class T>
void GameObject<T>::workerThread(int i)
{
	//Acquring our thread mutex
	std::unique_lock<std::mutex> lock(workerThreadsMutex[i]);
	int myNum;
	while (!terminate)
	{
		//Waiting for the logic thread to signal to us that we have some work to do
		cv.wait(lock);
		startedWorking[i] = true;
		do
		{
			{
				//Acquring the index to work on
				//This process is syncrhonized using the listIdentiferMutex
				std::lock_guard<std::mutex> ll(listIdentifierMutex);
				myNum = listIdentifer;
				//If list identitfer is bigger than or equal 0, it means we actually got something, so we decrement it for the next thread
				//If not, it means there is nothing for us to do so we just move on
				if (listIdentifer >= 0)
				{
					listIdentifer--;
				}
			}
			//If we have acquried an actual index, we start working...
			if (myNum >= 0)
			{
				//Copy the data from the logic instace to the rendering instance..
				updateList[myNum]->buffered[1] = updateList[myNum]->buffered[0];
				//..then mark this instance as no longer in the updateList
				updateList[myNum]->isInUpdateList = false;
			}
		} while (myNum >= 0);
	}
}

template<class T>
void GameObject<T>::cleanUp()
{
	//Wake all threads and tell them we are done here
	terminate = true;
	cv.notify_all();
	for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		//Delete them one by one
		workerThreads[i]->join();
		delete workerThreads[i];
	}
	//Clear the arrays
	delete[] workerThreads;
	delete[] workerThreadsMutex;
	delete[] startedWorking;
}

template<class T>
void GameObject<T>::update()
{
	//Set the listIdentifer to the last index of the updateList
	listIdentifer = updateList.size() - 1;
	//Wake up all worker threads
	cv.notify_all();
	//Try to acquire all of their mutecies to know if they are done or not...
	//notDoneYet flag is used to make sure that all threads responded to our call and done a full loop before returning to main thread
	bool notDoneYet = true;
	while (notDoneYet)
	{
		notDoneYet = false;
		for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
		{
			if (!startedWorking[i])
			{
				//if a thread hasnt startedWorking yet, this means we have got here before it manages
				//to pick up the condition variable call, so we are going to revisit it later
				notDoneYet = true;
			}
			else
			{
				//if a thread startedWorking, we gonna try to lock its mutex
				//if it is still working we gonna just wait for it to be done
				//if it is not we gonna quickly lock then unlock it
				std::lock_guard<std::mutex> ll(workerThreadsMutex[i]);
			}
		}
	}
	//reset the startWorking array to false
	for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		startedWorking[i] = false;
	}
	updateList.clear();
}

template<class T>
GameObject<T>::GameObject(int layer)
{
	//Checking if this template instance is set up
	if (!isRegistered)
	{
		//Logging the amount of working threads we decided to make based on the current hardware concurrency
		engine->Log("Creating " + std::to_string(std::thread::hardware_concurrency()) + " worker threads for template " + typeid(T).name());
		//Creating worker thread array based on the current hardware
		workerThreads = new std::thread*[std::thread::hardware_concurrency()];
		//Creating mutecies for the workerThreads based on current hardware
		workerThreadsMutex = new std::mutex[std::thread::hardware_concurrency()];
		startedWorking = new bool[std::thread::hardware_concurrency()];
		for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
		{
			startedWorking[i] = false;
			//Creating each workerThread and assigning them a unique number for their mutex
			workerThreads[i] = new std::thread(workerThread, i);
		}
		//Registering...
		engine->RegisterOnClose(cleanUp);
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
	//Checking if this template instance is set up
	if (!isRegistered)
	{
		//Logging the amount of working threads we decided to make based on the current hardware concurrency
		engine->Log("Creating " + std::to_string(std::thread::hardware_concurrency()) + " worker threads for template " + typeid(T).name());
		//Creating worker thread array based on the current hardware
		workerThreads = new std::thread*[std::thread::hardware_concurrency()];
		//Creating mutecies for the workerThreads based on current hardware
		workerThreadsMutex = new std::mutex[std::thread::hardware_concurrency()];
		startedWorking = new bool[std::thread::hardware_concurrency()];
		for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
		{
			startedWorking[i] = false;
			//Creating each workerThread and assigning them a unique number for their mutex
			workerThreads[i] = new std::thread(workerThread, i);
		}
		//Registering...
		engine->RegisterOnClose(cleanUp);
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