#include "Sorts.h"

void SelectionSort::Sort()
{
	SetThreadDescription(GetCurrentThread(), L"Selection Sort Thread");
	std::lock_guard<std::mutex> lg(m);
	std::unique_lock<std::mutex> ul(buf, std::defer_lock);
	std::unique_lock<std::mutex> sbul(sbuf, std::defer_lock);
	InProgress = true;
	for (unsigned int i = 0; i < Size; i++)
	{
		int min, temp;
		min = i;
		for (unsigned int j = i + 1; j < Size; j++)
		{
			if (!InProgress)
			{
				ul.lock();
				Buffer.clear();
				ul.unlock();
				return;
			}
			sleep(milliseconds(*ms));
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

SelectionSort::SelectionSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr)
			 : SortVisualizer(Size, Width, Height, xoffset, yoffset, ms, Arr)
{

}

inline bool SelectionSort::IsInProgress()
{
	return InProgress;
}

void SelectionSort::UpdateArray()
{
	engine->WaitForRenderer();
	if (!InProgress)
	{
		TempBuffer.clear();
		SwapBuffer.clear();
		Buffer.clear();
		for (unsigned int i = 0; i < Size; i++)
		{
			UpdateRectangle(i, Color::White);
		}
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

void SelectionSort::StartSort()
{
	if (InProgress)
	{
		StopSort();
	}
	SortingThread = new std::thread(&SelectionSort::Sort, this);
}

void SelectionSort::StopSort()
{
	if (InProgress)
	{
		InProgress = false;
		m.lock();
		m.unlock();
		SortingThread->join();
		delete SortingThread;
		SortingThread = nullptr;
	}
}