#pragma once
#include "Engine.h"

class SortVisualizer
{
protected:
	int Size, Height, Width, xoffset, yoffset, Min, Max, *Arr, *ms;
	float RectWidth;
	RectangleShape SuperRect;
	VertexArray *RectBatch;
	void UpdateRectangle(int index, Color color);
public:
	SortVisualizer(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr);
	virtual void UpdateArray() = 0;
	virtual void StartSort() = 0;
	virtual void StopSort() = 0;
	inline virtual bool IsInProgress() = 0;
	~SortVisualizer();
};
