#include "Engine.h"
#include "RoutineClass.h"

const int Size = 100;
const int Height = 768;
const int Width = 1366;
int xoffset = 0, yoffset = 0;

std::vector < int > Arr;
RectangleShape* rects;
RenderWindow* MainWindow;
Font f;
Text t;
int Min = Size, Max = -1;
float ElapsedTime = 0;
int a = -1, b = -1;
int ms = 5;
bool InProgress = false;
std::mutex m;

void DrawArray()
{
	if (InProgress)
	{
		ElapsedTime += engine->GetDeltaTime();
		engine->ss << "Elapsed time = " << ElapsedTime << "s\t";
	}
	else
	{
		ElapsedTime = 0;
	}
	engine->ss << "Step time = " <<  ms << "ms";
	t.setString(engine->ss.str());
	engine->ss.str("");
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
	}
}

void Randomize()
{
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

void Sort()
{
	std::lock_guard<std::mutex> lg(m);
	InProgress = true;
	for (int i = 0; i < Arr.size(); i++)
	{
		int min, temp;
		min = i;
		for (int j = i + 1; j < Arr.size(); j++)
		{
			if (!InProgress)
			{
				a = -1;
				b = -1;
				return;
			}
			sleep(milliseconds(ms));
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
	InProgress = false;
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
			ms = std::max(0, ms - 1);
		}
		flag = false;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Left))
	{
		if (flag)
		{
			ms++;
		}
		flag = false;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Space))
	{
		if (flag)
		{
			InProgress = false;
			Randomize();
		}
		flag = false;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Return))
	{
		if (flag)
		{
			if (InProgress)
			{
				InProgress = false;
				m.lock();
				m.unlock();
			}
			else
			{
				std::thread t(Sort);
				t.detach();
			}
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
	f.loadFromFile("arial.ttf");
	t.setFont(f);
	t.setPosition(Vector2f(0.0f, 0.0f));
	t.setCharacterSize(15);
	t.setColor(Color::Green);
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
	engine->RegisterObject(1, &t);
	engine->RegisterRoutine(DrawArray);
	engine->RegisterRoutine(CaptureClick);
}