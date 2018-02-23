#include "Engine.h"
#include "RoutineClass.h"

const int Size = 100;
const int BufferLimit = 100;
int Height = VideoMode::getDesktopMode().height;
int Width = VideoMode::getDesktopMode().width;
int xoffset = 0, yoffset = 0;

std::vector < int > Arr;
RectangleShape* rects;
RenderWindow* MainWindow;
Font f;
Text t;
int Min = Size, Max = -1;
float ElapsedTime = 0;
std::vector< std::pair< std::pair<int, int >, int > > Buffer;
std::vector< std::pair< std::pair<int, int >, int > > TempBuffer;
std::vector < std::pair < int, int > > SwapBuffer;
int ms = 5;
bool InProgress = false;
std::mutex m, buf, sbuf;

void DrawArray()
{
	if (InProgress)
	{
		ElapsedTime += engine->GetDeltaTime();
	}
	else
	{
		TempBuffer.clear();
	}
	engine->ss << "Elapsed time = " << ElapsedTime << "s\t" << "Step time = " << ms << "ms";
	t.setString(engine->ss.str());
	engine->ss.str("");
	for (unsigned int i = 0; i < Arr.size(); i++)
	{
		if (!InProgress)
		{
			float x = (float)(Arr[i] - Min) / (Max - Min);
			rects[i].setSize(Vector2f(rects[i].getSize().x, x * Height * -1));
		}
		rects[i].setFillColor(Color::White);
	}
	sbuf.lock();
	for (unsigned int i = 0; i < SwapBuffer.size(); i++)
	{
		Vector2f temp = rects[SwapBuffer[i].first].getPosition();
		rects[SwapBuffer[i].first].setPosition(rects[SwapBuffer[i].second].getPosition());
		rects[SwapBuffer[i].second].setPosition(temp);
		std::swap(rects[SwapBuffer[i].first], rects[SwapBuffer[i].second]);
	}
	SwapBuffer.clear();
	sbuf.unlock();
	buf.lock();
	if (Buffer.size() == 0)
	{
		buf.unlock();
		for (unsigned int i = 0; i < TempBuffer.size(); i++)
		{
			rects[TempBuffer[i].first.first].setFillColor(Color::Green);
			rects[TempBuffer[i].first.second].setFillColor(Color::Red);
			rects[TempBuffer[i].second].setFillColor(Color::Blue);
		}
	}
	else
	{
		TempBuffer.resize(Buffer.size());
		for (unsigned int i = 0; i < Buffer.size(); i++)
		{
			rects[Buffer[i].first.first].setFillColor(Color::Green);
			rects[Buffer[i].first.second].setFillColor(Color::Red);
			rects[Buffer[i].second].setFillColor(Color::Blue);
			TempBuffer[i] = Buffer[i];
		}
		buf.unlock();
		Buffer.clear();
	}
}

void Sort()
{
	std::lock_guard<std::mutex> lg(m);
	std::unique_lock<std::mutex> ul(buf, std::defer_lock);
	std::unique_lock<std::mutex> sbul(sbuf, std::defer_lock);
	InProgress = true;
	for (unsigned int i = 0; i < Arr.size(); i++)
	{
		int min, temp;
		min = i;
		for (unsigned int j = i + 1; j < Arr.size(); j++)
		{
			if (!InProgress)
			{
				ul.lock();
				Buffer.clear();
				ul.unlock();
				return;
			}
			sleep(milliseconds(ms));
			ul.lock();
			if (Buffer.size() < BufferLimit)
			{
				Buffer.push_back({ { i,  j }, min });
			}
			ul.unlock();
			if (Arr[j] < Arr[min])
			{
				min = j;
			}
		}
		sbul.lock();
		SwapBuffer.push_back({ min, i });
		sbul.unlock();
		temp = Arr[min];
		Arr[min] = Arr[i];
		Arr[i] = temp;
	}
	InProgress = false;
	Buffer.clear();
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
			InProgress = false;
			std::random_shuffle(Arr.begin(), Arr.end());
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
				ElapsedTime = 0;
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

void OnClose()
{
	InProgress = false;
	m.lock();
	m.unlock();
}

void Start()
{
	f.loadFromFile("arial.ttf");
	t.setFont(f);
	t.setPosition(Vector2f(0.0f, 0.0f));
	t.setCharacterSize(15);
	t.setFillColor(Color::Green);
	Arr.resize(Size);
	MainWindow = engine->GetWindow();
	rects = new RectangleShape[Arr.size()];
	float RectWidth = (float)Width / Arr.size();
	for (int i = 0; i < Size; i++)
	{
		Arr[i] = i;
		rects[i].setSize(Vector2f(RectWidth / 1.09f, 1.0f));
		rects[i].setPosition(Vector2f((RectWidth * i) + xoffset, (float)Height + yoffset));
		engine->RegisterObject(0, &rects[i]);
	}
	Min = -1;
	Max = Size;
	std::random_shuffle(Arr.begin(), Arr.end());
	engine->RegisterObject(1, &t);
	engine->RegisterRoutine(DrawArray);
	engine->RegisterRoutine(CaptureClick);
	engine->RegisterOnClose(OnClose);
}