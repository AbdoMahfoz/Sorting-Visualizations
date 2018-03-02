#pragma once
#include "Engine.h"

class SortVisualizer
{
protected:
	struct ColorDescription
	{
	public:
		int WhiteIndex[5], BlueIndex[5], RedIndex[5], GreenIndex[5];
		ColorDescription();
	};
	int Size, Height, Width, xoffset, yoffset, Min, Max, *Arr, *ms;
	float RectWidth;
	RectangleShape SuperRect;
	VertexArray *RectBatch;
	std::thread *SortingThread;
	std::vector < ColorDescription > TempColorBuffer;
	std::vector < ColorDescription > ColorBuffer;
	std::vector < int > UpdateBuffer;
	std::mutex ColorBufferMutex, UpdateBufferMutex;
	bool InProgress;
	void Colorize(ColorDescription cd);
	void DeColorize(ColorDescription cd);
	void UpdateRectangle(int index, Color color);
	virtual void Sort() = 0;
public:
	SortVisualizer(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr);
	void UpdateArray();
	bool IsInProgress();
	void StartSort();
	void StopSort();
	~SortVisualizer();
};
