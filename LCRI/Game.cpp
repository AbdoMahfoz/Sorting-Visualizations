#include "Engine.h"
#include "RoutineClass.h"

const int Size = 100;
int RandomizeRate = 0;

std::vector < int > Arr;
RectangleShape* rects;
int Min = Size, Max = -1;
float ElapsedTime = 0;

void DrawArray()
{
	int MaxRectHeight = VideoMode::getDesktopMode().height;
	for (int i = 0; i < Arr.size(); i++)
	{
		float x = (float)(Arr[i] - Min) / (Max - Min);
		rects[i].setSize(Vector2f(rects[i].getSize().x, x * MaxRectHeight * -1));
	}
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
		for (int i = 0; i < Arr.size(); i++)
		{
			Arr[i] = rand() % Size;
			Min = std::min(Min, Arr[i]);
			Max = std::max(Max, Arr[i]);
		}
	}
}

void Sort()
{
	for (int i = 0; i < Arr.size(); i++)
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
	Arr.resize(Size);
	rects = new RectangleShape[Arr.size()];
	float RectWidth = (float)VideoMode::getDesktopMode().width / Arr.size();
	for (int i = 0; i < Size; i++)
	{
		Arr[i] = rand() % Size;
		Min = std::min(Min, Arr[i]);
		Max = std::max(Max, Arr[i]);
		rects[i].setSize(Vector2f(RectWidth / 1.09, 1.0f));
		rects[i].setPosition(Vector2f((RectWidth * i), VideoMode::getDesktopMode().height));
		engine->RegisterObject(0, &rects[i]);
	}
	Min--;
	Max++;
	engine->RegisterRoutine(DrawArray);
	engine->RegisterRoutine(Randomize);
	engine->RegisterRoutine(CaptureClick);
}