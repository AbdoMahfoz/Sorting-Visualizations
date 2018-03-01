#include "Engine.h"
#include "RoutineClass.h"
#include "Sorts.h"

Font f;
Text t;
float ElapsedTime = 0;
int Size = 100;
int ms = 5, *Arr[5];
bool InProgress = false;
std::vector < SortVisualizer* > Sorts;

void SfDrawText()
{
	engine->Log(std::to_string(engine->GetDeltaTime()));
	if (InProgress)
	{
		ElapsedTime += engine->GetDeltaTime();
	}
	t.setString("Step time = " + std::to_string(ms) + "ms\tElapsed time = " + std::to_string(ElapsedTime)
		+ "s\tFramerate = " + std::to_string((int)(1.0 / engine->GetDeltaTime())));
}

void Randomize()
{
	for (int i = 0; i < 5; i++)
	{
		std::random_shuffle(Arr[i], Arr[i] + Size);
	}
}

void Update()
{
	bool flag = false;
	for (int i = 0; i < Sorts.size(); i++)
	{
		Sorts[i]->UpdateArray();
		if (Sorts[i]->IsInProgress())
		{
			flag = true;
		}
	}
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
				for (int i = 0; i < Sorts.size(); i++)
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
				for (int i = 0; i < Sorts.size(); i++)
				{
					Sorts[i]->StopSort();
				}
				InProgress = false;
			}
			else
			{
				ElapsedTime = 0;
				for (int i = 0; i < Sorts.size(); i++)
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
	for (int i = 0; i < Sorts.size(); i++)
	{
		Sorts[i]->StopSort();
		delete Sorts[i];
	}
	for (int i = 0; i < 5; i++)
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
	for (int j = 0; j < 5; j++)
	{
		Arr[j] = new int[Size];
		for (int i = 0; i < Size; i++)
		{
			Arr[j][i] = i;
		}
	}
	Sorts.push_back((SortVisualizer*)new SelectionSort(Size, (VideoMode::getDesktopMode().width / 3) - 20, VideoMode::getDesktopMode().height / 2, 0, 0, &ms, Arr[0]));
	Sorts.push_back((SortVisualizer*)new SelectionSort(Size, (VideoMode::getDesktopMode().width / 3) - 20, VideoMode::getDesktopMode().height / 2, VideoMode::getDesktopMode().width / 3, 0, &ms, Arr[1]));
	Sorts.push_back((SortVisualizer*)new SelectionSort(Size, (VideoMode::getDesktopMode().width / 3) - 20, VideoMode::getDesktopMode().height / 2, 2 * VideoMode::getDesktopMode().width / 3, 0, &ms, Arr[2]));
	Sorts.push_back((SortVisualizer*)new SelectionSort(Size, (VideoMode::getDesktopMode().width / 2) - 20, VideoMode::getDesktopMode().height / 2, 0, VideoMode::getDesktopMode().height / 2, &ms, Arr[3]));
	Sorts.push_back((SortVisualizer*)new SelectionSort(Size, (VideoMode::getDesktopMode().width / 2) - 20, VideoMode::getDesktopMode().height / 2, VideoMode::getDesktopMode().width / 2, VideoMode::getDesktopMode().height / 2, &ms, Arr[4]));
	Randomize();
	engine->RegisterObject(1, &t);
	engine->RegisterRoutine(SfDrawText);
	engine->RegisterRoutine(Update);
	engine->RegisterRoutine(CaptureClick);
	engine->RegisterOnClose(OnClose);
}