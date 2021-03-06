#include "Engine.h"
#include "Sorts.h"

Font f;
Text t;
float ElapsedTime = 0;
int Size = 100000;
const int AlgoCount = 1;
int ms = 0, *Arr[AlgoCount];
bool InProgress = false;
std::vector < SortVisualizer* > Sorts;

void SfDrawText()
{
	if (InProgress)
	{
		ElapsedTime += engine->GetDeltaTime();
	}
	t.setString("Step time = " + std::to_string(ms) + "ms\tElapsed time = " + std::to_string(ElapsedTime)
		+ "s\tFramerate = " + std::to_string((int)(1.0 / engine->GetDeltaTime())));
}

void Randomize()
{
	std::random_shuffle(Arr[0], Arr[0] + Size);
	for (int i = 1; i < AlgoCount; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			Arr[i][j] = Arr[0][j];
		}
	}
}

void Update()
{
	bool flag = false;
	std::thread** threadPool = new std::thread*[Sorts.size()];
	for (unsigned int i = 0; i < Sorts.size(); i++)
	{
		threadPool[i] = new std::thread(&SortVisualizer::UpdateArray, Sorts[i]);
		if (Sorts[i]->IsInProgress())
		{
			flag = true;			
		}
	}
	for (unsigned int i = 0; i < Sorts.size(); i++)
	{
		threadPool[i]->join();
		delete threadPool[i];
	}
	delete[] threadPool;
	if (!flag || !InProgress)
	{
		InProgress = false;
		engine->UnRegisterRoutine(Update);
	}
}

void CaptureClick()
{
	static bool flag = true;
	if (Keyboard::isKeyPressed(Keyboard::Key::Right))
	{
		if (flag)
		{
			ms++;
		}
		flag = false;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Left))
	{
		if (flag)
		{
			ms = std::max(0, ms - 1);
		}
		flag = false;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Space))
	{
		if (flag)
		{
			if (InProgress)
			{
				for (unsigned int i = 0; i < Sorts.size(); i++)
				{
					Sorts[i]->StopSort();
				}
				InProgress = false;
			}
			Randomize();
			engine->RegisterRoutine(Update);
		}
		flag = false;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Return))
	{
		if (flag)
		{
			if (InProgress)
			{
				for (unsigned int i = 0; i < Sorts.size(); i++)
				{
					Sorts[i]->StopSort();
				}
				InProgress = false;
			}
			else
			{
				ElapsedTime = 0;
				for (unsigned int i = 0; i < Sorts.size(); i++)
				{
					Sorts[i]->StartSort();
				}
				InProgress = true;
				engine->RegisterRoutine(Update);
			}
		}
		flag = false;
	}
	else
	{
		flag = true;
	}
}

void OnClose()
{
	for (unsigned int i = 0; i < Sorts.size(); i++)
	{
		Sorts[i]->StopSort();
		delete Sorts[i];
	}
	for (int i = 0; i < AlgoCount; i++)
	{
		delete[] Arr[i];
	}
}

void Start()
{
	f.loadFromFile("arial.ttf");
	t.setFont(f);
	t.setPosition(Vector2f(0.0f, 0.0f));
	t.setCharacterSize(15);
	t.setFillColor(Color::Green);
	for (int j = 0; j < AlgoCount; j++)
	{
		Arr[j] = new int[Size];
		for (int i = 0; i < Size; i++)
		{
			Arr[j][i] = i;
		}
	}
	int width = VideoMode::getDesktopMode().width, height = VideoMode::getDesktopMode().height;
	Sorts.push_back(new QuickSort(Size, width, height, 0, 0, &ms, Arr[0]));
	//Sorts.push_back(new QuickSort(Size, width, height / 2, 0, height / 2, &ms, Arr[1]));
	/*
	Sorts.push_back(new SelectionSort(Size, (width / 3) - 20, height / 2, 0, 0, &ms, Arr[0]));
	Sorts.push_back(new InsertionSort(Size, (width / 3) - 20, height / 2, width / 3, 0, &ms, Arr[1]));
	Sorts.push_back(new BubbleSort(Size, (width / 3) - 20, height / 2, 2 * width / 3, 0, &ms, Arr[2]));
	Sorts.push_back(new MergeSort(Size, (width / 2) - 20, height / 2, 0, height / 2, &ms, Arr[3]));
	Sorts.push_back(new QuickSort(Size, (width / 2) - 20, height / 2, width / 2, height / 2, &ms, Arr[4]));
	*/
	Randomize();
	engine->RegisterObject(1, &t);
	engine->RegisterRoutine(SfDrawText);
	engine->RegisterRoutine(Update);
	engine->RegisterRoutine(CaptureClick);
	engine->RegisterOnClose(OnClose);
}