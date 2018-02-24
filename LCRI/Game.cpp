#include "Engine.h"
#include "RoutineClass.h"

int Size = 1366;
int Height = VideoMode::getDesktopMode().height;
int Width = VideoMode::getDesktopMode().width;
int xoffset = 0, yoffset = 0;
float RectWidth = (float)Width / Size;

std::vector < int > Arr;
RectangleShape SuperRect;
VertexArray* RectBatch;
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

void SfDrawText()
{
	engine->Log(std::to_string(engine->GetDeltaTime()));
	if (InProgress)
	{
		ElapsedTime += engine->GetDeltaTime();
	}
	t.setString("Step time = " + std::to_string(ms) + "ms\tElapsed time = " + std::to_string(ElapsedTime) + "s\tFramerate = " + std::to_string((int)(1.0 / engine->GetDeltaTime())));
}

void UpdateRectangle(int i, Color c)
{
	float x = (float)(Arr[i] - Min) / (Max - Min);
	int k = i * 4;
	SuperRect.setFillColor(c);
	SuperRect.setSize(Vector2f(RectWidth / 1.09f, x * Height * -1));
	SuperRect.setPosition(Vector2f((RectWidth * i) + xoffset, (float)Height + yoffset));
	Vector2f Position = SuperRect.getPosition();
	Color color = SuperRect.getFillColor();
	RectBatch->operator[](k).position = SuperRect.getPoint(0) + Position;
	RectBatch->operator[](k + 1).position = SuperRect.getPoint(1) + Position;
	RectBatch->operator[](k + 2).position = SuperRect.getPoint(2) + Position;
	RectBatch->operator[](k + 3).position = SuperRect.getPoint(3) + Position;
	RectBatch->operator[](k).color = color;
	RectBatch->operator[](k + 1).color = color;
	RectBatch->operator[](k + 2).color = color;
	RectBatch->operator[](k + 3).color = color;
}

void DrawArray()
{
	engine->WaitForRenderer();
	if (!InProgress)
	{
		TempBuffer.clear();
		SwapBuffer.clear();
		Buffer.clear();
		for (unsigned int i = 0; i < Arr.size(); i++)
		{
			UpdateRectangle(i, Color::White);
		}
		engine->UnRegisterRoutine(DrawArray);
		return;
	}
	sbuf.lock();
	for (unsigned int i = 0; i < SwapBuffer.size(); i++)
	{
		UpdateRectangle(SwapBuffer[i].first, Color::White);
		UpdateRectangle(SwapBuffer[i].second, Color::White);
	}
	SwapBuffer.clear();
	sbuf.unlock();
	buf.lock();
	if (Buffer.size() != 0)
	{
		for (unsigned int i = 0; i < TempBuffer.size(); i++)
		{
			UpdateRectangle(TempBuffer[i].first.first, Color::White);
			UpdateRectangle(TempBuffer[i].first.second, Color::White);
			UpdateRectangle(TempBuffer[i].second, Color::White);
		}
		TempBuffer.resize(Buffer.size());
		for (unsigned int i = 0; i < Buffer.size(); i++)
		{
			UpdateRectangle(Buffer[i].first.first, Color::Green);
			UpdateRectangle(Buffer[i].first.second, Color::Red);
			UpdateRectangle(Buffer[i].second, Color::Blue);
			TempBuffer[i] = Buffer[i];
		}
		Buffer.clear();
	}
	buf.unlock();
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
			Buffer.push_back({ { i,  j }, min });
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
			if (InProgress)
			{
				InProgress = false;
				m.lock();
				m.unlock();
			}
			std::random_shuffle(Arr.begin(), Arr.end());
			engine->RegisterRoutine(DrawArray);
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
				engine->RegisterRoutine(DrawArray);
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
	delete[] RectBatch;
}

void Start()
{
	f.loadFromFile("arial.ttf");
	t.setFont(f);
	t.setPosition(Vector2f(0.0f, 0.0f));
	t.setCharacterSize(15);
	t.setFillColor(Color::Green);
	Arr.resize(Size);
	RectBatch = new VertexArray(Quads, Size * 4);
	for (int i = 0, k = 0; i < Size; i++, k += 4)
	{
		Arr[i] = i;
		UpdateRectangle(i, Color::White);
	}
	Min = -1;
	Max = Size;
	std::random_shuffle(Arr.begin(), Arr.end());
	engine->RegisterObject(1, &t);
	engine->RegisterObject(0, RectBatch);
	engine->RegisterRoutine(CaptureClick);
	engine->RegisterRoutine(DrawArray);
	engine->RegisterRoutine(SfDrawText);
	engine->RegisterOnClose(OnClose);
}