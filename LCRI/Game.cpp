#include "Engine.h"
#include "RoutineClass.h"

const int Size = 100;
const int Height = 768;
const int Width = 1366;
int RandomizeRate = 0;
int xoffset = 0, yoffset = 0;

std::vector < int > Arr;
RectangleShape* rects;
RenderWindow* MainWindow;
int Min = Size, Max = -1;
float ElapsedTime = 0;
int a = -1, b = -1;

void DrawArray()
{
	Event event;
	while (MainWindow->pollEvent(event))
	{
		switch (event.type)
		{
			//Window Closed Event
		case Event::Closed:
			MainWindow->close();
			exit(0);
			break;
		}
	}
	MainWindow->clear();
	for (unsigned int i = 0; i < Arr.size(); i++)
	{
		float x = (float)(Arr[i] - Min) / (Max - Min);
		rects[i].setSize(Vector2f(rects[i].getSize().x, x * Height * -1));
		if (i == a)
		{
			rects[i].setFillColor(Color::Red);
		}
		else if (i == b)
		{
			rects[i].setFillColor(Color::Blue);
		}
		else
		{
			rects[i].setFillColor(Color::White);
		}
		MainWindow->draw(rects[i]);
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
		Min--;
		Max++;
	}
}

void Sort()
{
	for (int i = 0; i < Arr.size(); i++)
	{
		int min, temp;
		min = i;
		for (int j = i + 1; j < Arr.size(); j++)
		{
			DrawArray();
			sleep(milliseconds(2));
			a = j;
			b = min;
			if (Arr[j] < Arr[min])
			{
				min = j;
			}
		}
		temp = Arr[min];
		Arr[min] = Arr[i];
		Arr[i] = temp;
	}
	a = -1;
	b = -1;
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
	MainWindow = engine->GetWindow();
	rects = new RectangleShape[Arr.size()];
	float RectWidth = (float)Width / Arr.size();
	for (int i = 0; i < Size; i++)
	{
		Arr[i] = rand() % Size;
		Min = std::min(Min, Arr[i]);
		Max = std::max(Max, Arr[i]);
		rects[i].setSize(Vector2f(RectWidth / 1.09f, 1.0f));
		rects[i].setPosition(Vector2f((RectWidth * i) + xoffset, (float)Height + yoffset));
		engine->RegisterObject(0, &rects[i]);
	}
	Min--;
	Max++;
	engine->RegisterRoutine(DrawArray);
	engine->RegisterRoutine(Randomize);
	engine->RegisterRoutine(CaptureClick);
}