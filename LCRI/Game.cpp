#include "Engine.h"
#include "RoutineClass.h"

const int Size = 100000;
int RandomizeRate = 60;

std::vector < int > Arr;
RectangleShape rect;
int Min = Size, Max = -1;
float ElapsedTime = 0;
float RectWidth = (float)VideoMode::getDesktopMode().width / Size;
RenderWindow* MainWindow;

void DrawArray()
{
	float RectHeight = (float)VideoMode::getDesktopMode().height;
	MainWindow->clear();
	for (unsigned int i = 0; i < Arr.size(); i++)
	{
		float x = (float)(Arr[i] - Min) / (Max - Min);
		rect.setSize(Vector2f(RectWidth / 1.09f, x * RectHeight * -1));
		rect.setPosition(Vector2f(RectWidth * i, RectHeight));
		MainWindow->draw(rect);
	}
	MainWindow->display();
}

void Randomize()
{
	if (RandomizeRate == 0)
	{
		ElapsedTime = 0;
		return;
	}
	ElapsedTime += engine->GetDeltaTime();
	if (ElapsedTime > 1.0f / RandomizeRate)
	{
		ElapsedTime = 0;
		Min = Size;
		Max = -1;
		for (unsigned int i = 0; i < Arr.size(); i++)
		{
			Arr[i] = rand() % Size;
			Min = std::min(Min, Arr[i]);
			Max = std::max(Max, Arr[i]);
		}
	}
}

void Sort()
{
	for (unsigned int i = 0; i < Arr.size(); i++)
	{
		Arr[i] = i;
	}
	Min = -1;
	Max = Arr.size();
}

void CaptureClick()
{
	static bool flag = true;
	if (Keyboard::isKeyPressed(Keyboard::Key::Right))
	{
		if (flag)
		{
			RandomizeRate++;
		}
		flag = false;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Left))
	{
		if (flag)
		{
			RandomizeRate = std::max(0, RandomizeRate - 1);
		}
		flag = false;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Space))
	{
		if (flag)
		{
			RandomizeRate = 0;
		}
		flag = false;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Return))
	{
		if (flag)
		{
			RandomizeRate = 0;
			Sort();
		}
		flag = false;
	}
	else
	{
		flag = true;
	}
}

void Start()
{
	MainWindow = engine->GetWindow();
	Arr.resize(Size);
	for (unsigned int i = 0; i < Size; i++)
	{
		Arr[i] = rand() % Size;
		Min = std::min(Min, Arr[i]);
		Max = std::max(Max, Arr[i]);
	}
	Min--;
	Max++;
	engine->RegisterRoutine(DrawArray);
	engine->RegisterRoutine(Randomize);
	engine->RegisterRoutine(CaptureClick);
}