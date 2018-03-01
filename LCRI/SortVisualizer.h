#pragma once
#include "Engine.h"

class SortVisualizer
{
private:
	int Size, Height, Width, xoffset, yoffset, Min, Max, *Arr;
	float RectWidth;
	RectangleShape SuperRect;
	VertexArray *RectBatch;
	void UpdateRectangle(int index, Color color);
	virtual void Sort() = 0;
public:
	SortVisualizer(int Size, int Width, int Height, int xoffset, int yoffset);
	virtual void UpdateArray() = 0;
	virtual void StartSort() = 0;
	virtual void StopSort() = 0;
	~SortVisualizer();
};
