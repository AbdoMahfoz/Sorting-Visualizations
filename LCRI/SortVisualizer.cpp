#include "SortVisualizer.h"

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
	RectBatch = new VertexArray(Quads, 4 * Size);
	engine->RegisterObject(0, RectBatch);
}

void SortVisualizer::UpdateRectangle(int i, Color c)
{
	float x = (float)(Arr[i] - Min) / (Max - Min);
	int k = i * 4;
	SuperRect.setSize(Vector2f(RectWidth / 1.09f, x * Height * -1));
	SuperRect.setPosition(Vector2f((RectWidth * i) + xoffset, (float)Height + yoffset));
	Vector2f Position = SuperRect.getPosition();
	RectBatch->operator[](k).position = SuperRect.getPoint(0) + Position;
	RectBatch->operator[](k + 1).position = SuperRect.getPoint(1) + Position;
	RectBatch->operator[](k + 2).position = SuperRect.getPoint(2) + Position;
	RectBatch->operator[](k + 3).position = SuperRect.getPoint(3) + Position;
	RectBatch->operator[](k).color = c;
	RectBatch->operator[](k + 1).color = c;
	RectBatch->operator[](k + 2).color = c;
	RectBatch->operator[](k + 3).color = c;
}

bool SortVisualizer::IsInProgress()
{
	return InProgress;
}

SortVisualizer::~SortVisualizer()
{
	delete RectBatch;
}