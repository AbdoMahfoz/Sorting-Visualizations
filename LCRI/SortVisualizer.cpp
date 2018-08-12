#include "SortVisualizer.h"

SortVisualizer::ColorDescription::ColorDescription()
{
	for (int i = 0; i < 5; i++)
	{
		WhiteIndex[i] = -1;
		RedIndex[i] = -1;
		BlueIndex[i] = -1;
		GreenIndex[i] = -1;
	}
}

SortVisualizer::SortVisualizer(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr)
{
	this->Size = Size;
	this->Height = Height;
	this->Width = Width;
	this->xoffset = xoffset;
	this->yoffset = yoffset;
	this->ms = ms;
	this->Arr = Arr;
	InProgress = false;
	Max = Size;
	Min = -1;
	RectWidth = (float)Width / Size;
	RectBatch = new GameObject<VertexArray>(0, VertexArray(Quads, 4 * Size));
}

void SortVisualizer::Colorize(ColorDescription cd)
{
	for (int i = 0; i < 5; i++)
	{
		if (cd.WhiteIndex[i] != -1)
		{
			UpdateRectangle(cd.WhiteIndex[i], Color::White);
		}
		if (cd.BlueIndex[i] != -1)
		{
			UpdateRectangle(cd.BlueIndex[i], Color::Blue);
		}
		if (cd.RedIndex[i] != -1)
		{
			UpdateRectangle(cd.RedIndex[i], Color::Red);
		}
		if (cd.GreenIndex[i] != -1)
		{
			UpdateRectangle(cd.GreenIndex[i], Color::Green);
		}
	}
}

void SortVisualizer::DeColorize(ColorDescription cd)
{
	for (int i = 0; i < 5; i++)
	{
		if (cd.BlueIndex[i] != -1)
		{
			UpdateRectangle(cd.BlueIndex[i], Color::White);
		}
		if (cd.RedIndex[i] != -1)
		{
			UpdateRectangle(cd.RedIndex[i], Color::White);
		}
		if (cd.GreenIndex[i] != -1)
		{
			UpdateRectangle(cd.GreenIndex[i], Color::White);
		}
	}
}

void SortVisualizer::UpdateRectangle(int i, Color c)
{
	float x = (float)(Arr[i] - Min) / (Max - Min);
	int k = i * 4;
	if (SuperRect.getSize().y != x * Height * -1)
	{
		SuperRect.setSize(Vector2f(RectWidth, x * Height * -1));
	}
	SuperRect.setPosition(Vector2f((RectWidth * i) + xoffset, (float)Height + yoffset));
	Vector2f Position = SuperRect.getPosition();
	RectBatch->getCoreObject()[k].position = SuperRect.getPoint(0) + Position;
	RectBatch->getCoreObject()[k + 1].position = SuperRect.getPoint(1) + Position;
	RectBatch->getCoreObject()[k + 2].position = SuperRect.getPoint(2) + Position;
	RectBatch->getCoreObject()[k + 3].position = SuperRect.getPoint(3) + Position;
	RectBatch->getCoreObject()[k].color = c;
	RectBatch->getCoreObject()[k + 1].color = c;
	RectBatch->getCoreObject()[k + 2].color = c;
	RectBatch->getCoreObject()[k + 3].color = c;
}

void SortVisualizer::UpdateArray()
{
	if (!InProgress)
	{
		TempColorBuffer.clear();
		UpdateBuffer.clear();
		ColorBuffer.clear();
		for (int i = 0; i < Size; i++)
		{
			UpdateRectangle(i, Color::White);
		}
		return;
	}
	UpdateBufferMutex.lock();
	for (unsigned int i = 0; i < UpdateBuffer.size(); i++)
	{
		UpdateRectangle(UpdateBuffer[i], Color::White);
	}
	UpdateBuffer.clear();
	UpdateBufferMutex.unlock();
	ColorBufferMutex.lock();
	if (ColorBuffer.size() != 0)
	{
		for (unsigned int i = 0; i < TempColorBuffer.size(); i++)
		{
			DeColorize(TempColorBuffer[i]);
		}
		TempColorBuffer.resize(ColorBuffer.size());
		for (unsigned int i = 0; i < ColorBuffer.size(); i++)
		{
			Colorize(ColorBuffer[i]);
			TempColorBuffer[i] = ColorBuffer[i];
		}
		ColorBuffer.clear();
	}
	ColorBufferMutex.unlock();
}

bool SortVisualizer::IsInProgress()
{
	return InProgress;
}

void SortVisualizer::StartSort()
{
	if (InProgress)
	{
		StopSort();
	}
	InProgress = true;
	SortingThread = new std::thread(&SortVisualizer::Sort, this);
}

void SortVisualizer::StopSort()
{
	if (InProgress)
	{
		InProgress = false;
		SortingThread->join();
		delete SortingThread;
		SortingThread = nullptr;
	}
}

SortVisualizer::~SortVisualizer()
{
	delete RectBatch;
}