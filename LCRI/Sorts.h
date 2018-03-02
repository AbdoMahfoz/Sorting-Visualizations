#pragma once
#include "SortVisualizer.h"

class SelectionSort : SortVisualizer
{
private:
	std::vector< std::pair< std::pair<int, int >, int > > Buffer;
	std::vector< std::pair< std::pair<int, int >, int > > TempBuffer;
	std::vector < std::pair < int, int > > SwapBuffer;
	std::mutex BufferMutex, SwapBufferMutex;
	void Sort();
public:
	SelectionSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr);
	void UpdateArray();
	void StartSort();
	void StopSort();
};

class MergeSort : SortVisualizer
{
private:
	struct ColorDescirption
	{
	public:
		int RangeStart, RangeEnd, LeftPointer, RightPointer;
		ColorDescirption(int RangeStart, int RangeEnd, int LeftPonter, int RightPointer);
		ColorDescirption();
	};
	std::vector < ColorDescirption > ColorBuffer;
	std::vector < ColorDescirption > TempColorBuffer;
	std::vector < int > UpdateBuffer;
	std::mutex ColorBufferMutex, UpdateBufferMutex;
	void Merge(int s, int f);
	void Partiton(int s, int f);
public:
	MergeSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr);
	void UpdateArray();
	void StartSort();
	void StopSort();
};