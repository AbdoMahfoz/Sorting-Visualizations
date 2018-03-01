#pragma once
#include "SortVisualizer.h"

class SelectionSort : SortVisualizer
{
private:
	std::thread *SortingThread;
	std::vector< std::pair< std::pair<int, int >, int > > Buffer;
	std::vector< std::pair< std::pair<int, int >, int > > TempBuffer;
	std::vector < std::pair < int, int > > SwapBuffer;
	bool InProgress = false;
	std::mutex m, buf, sbuf;
	void Sort();
public:
	SelectionSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr);
	inline bool IsInProgress();
	void UpdateArray();
	void StartSort();
	void StopSort();
};