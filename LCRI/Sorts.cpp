#include "Sorts.h"

//---------------------------Selection-Sort----------------------------
void SelectionSort::Sort()
{
	SetThreadDescription(GetCurrentThread(), L"Selection Sort Thread");
	std::unique_lock<std::mutex> BufferMutexGuard(BufferMutex, std::defer_lock);
	std::unique_lock<std::mutex> SwapBufferMutexGuard(SwapBufferMutex, std::defer_lock);
	InProgress = true;
	for (int i = 0; i < Size; i++)
	{
		int min, temp;
		min = i;
		for (int j = i + 1; j < Size; j++)
		{
			if (!InProgress)
			{
				BufferMutexGuard.lock();
				Buffer.clear();
				BufferMutexGuard.unlock();
				return;
			}
			sleep(milliseconds(*ms));
			BufferMutexGuard.lock();
			Buffer.push_back({ { i,  j }, min });
			BufferMutexGuard.unlock();
			if (Arr[j] < Arr[min])
			{
				min = j;
			}
		}
		SwapBufferMutexGuard.lock();
		SwapBuffer.push_back({ min, i });
		SwapBufferMutexGuard.unlock();
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

void SelectionSort::UpdateArray()
{
	engine->WaitForRenderer();
	if (!InProgress)
	{
		TempBuffer.clear();
		SwapBuffer.clear();
		Buffer.clear();
		for (int i = 0; i < Size; i++)
		{
			UpdateRectangle(i, Color::White);
		}
		return;
	}
	SwapBufferMutex.lock();
	for (unsigned int i = 0; i < SwapBuffer.size(); i++)
	{
		UpdateRectangle(SwapBuffer[i].first, Color::White);
		UpdateRectangle(SwapBuffer[i].second, Color::White);
	}
	SwapBuffer.clear();
	SwapBufferMutex.unlock();
	BufferMutex.lock();
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
	BufferMutex.unlock();
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
		SortingThread->join();
		delete SortingThread;
		SortingThread = nullptr;
	}
}

//---------------------------Merge-Sort----------------------------


MergeSort::ColorDescirption::ColorDescirption(int RangeStart, int RangeEnd, int LeftPointer, int RightPointer)
{
	this->RangeStart = RangeStart;
	this->RangeEnd = RangeEnd;
	this->LeftPointer = LeftPointer;
	this->RightPointer = RightPointer;
}

MergeSort::ColorDescirption::ColorDescirption()
{

}

void MergeSort::Merge(int s, int f)
{
	int Count = f - s + 1, Mid = (s + f) / 2;
	int lp = s, rp = Mid + 1, ap = 0;
	int* aux = new int[Count];
	std::unique_lock < std::mutex > ColorBufferGuard(ColorBufferMutex, std::defer_lock);
	std::unique_lock < std::mutex > UpdateBufferGuard(UpdateBufferMutex, std::defer_lock);
	ColorBufferGuard.lock();
	ColorBuffer.push_back(ColorDescirption(s, f, lp, rp));
	ColorBufferGuard.unlock();
	while (lp <= Mid && rp <= f && InProgress)
	{
		sleep(milliseconds(*ms));
		if (Arr[lp] <= Arr[rp])
		{
			aux[ap++] = Arr[lp++];
		}
		else
		{
			aux[ap++] = Arr[rp++];
		}
		ColorBufferGuard.lock();
		ColorBuffer.push_back(ColorDescirption(s, f, std::min(lp, Mid), std::min(rp, f)));
		ColorBufferGuard.unlock();
	}
	while (lp <= Mid && InProgress)
	{
		sleep(milliseconds(*ms));
		aux[ap++] = Arr[lp++];
		ColorBufferGuard.lock();
		ColorBuffer.push_back(ColorDescirption(s, f, std::min(lp, Mid), std::min(rp, f)));
		ColorBufferGuard.unlock();
	}
	while (rp <= f && InProgress)
	{
		sleep(milliseconds(*ms));
		aux[ap++] = Arr[rp++];
		ColorBufferGuard.lock();
		ColorBuffer.push_back(ColorDescirption(s, f, std::min(lp, Mid), std::min(rp, f)));
		ColorBufferGuard.unlock();
	}
	for (int i = 0, j = s; j <= f && InProgress; j++, i++)
	{
		sleep(milliseconds(*ms));
		Arr[j] = aux[i];
		ColorBufferGuard.lock();
		UpdateBufferGuard.lock();
		ColorBuffer.push_back(ColorDescirption(s, f, j, j));
		UpdateBuffer.push_back(j);
		ColorBufferGuard.unlock();
		UpdateBufferGuard.unlock();
	}
	delete[] aux;
}

void MergeSort::Partiton(int s, int f)
{
	static bool First = true;
	bool IAmFirst = false;
	if(First)
	{
		First = false;
		IAmFirst = true;
	}
	if (s >= f || !InProgress)
	{
		return;
	}
	int mid = (s + f) / 2;
	Partiton(s, mid);
	if(!InProgress)
	{
		return;
	}
	Partiton(mid + 1, f);
	if (!InProgress)
	{
		return;
	}
	Merge(s, f);
	if (IAmFirst)
	{
		First = true;
		InProgress = false;
	}
}

MergeSort::MergeSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr)
	     : SortVisualizer(Size, Width, Height, xoffset, yoffset, ms, Arr)
{
}

void MergeSort::UpdateArray()
{
	std::unique_lock < std::mutex > ColorBufferGuard(ColorBufferMutex, std::defer_lock);
	std::unique_lock < std::mutex > UpdateBufferGuard(UpdateBufferMutex, std::defer_lock);
	engine->WaitForRenderer();
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
	ColorBufferGuard.lock();
	if (ColorBuffer.size() != 0)
	{
		for (unsigned int i = 0; i < TempColorBuffer.size(); i++)
		{
			UpdateRectangle(TempColorBuffer[i].LeftPointer, Color::White);
			UpdateRectangle(TempColorBuffer[i].RightPointer, Color::White);
			UpdateRectangle(TempColorBuffer[i].RangeStart, Color::White);
			UpdateRectangle(TempColorBuffer[i].RangeEnd, Color::White);
		}
		TempColorBuffer.resize(ColorBuffer.size());
		for (unsigned int i = 0; i < ColorBuffer.size(); i++)
		{
			UpdateRectangle(ColorBuffer[i].LeftPointer, Color::Red);
			UpdateRectangle(ColorBuffer[i].RightPointer, Color::Red);
			UpdateRectangle(ColorBuffer[i].RangeStart, Color::Blue);
			UpdateRectangle(ColorBuffer[i].RangeEnd, Color::Blue);
			TempColorBuffer[i] = ColorBuffer[i];
		}
		ColorBuffer.clear();
	}
	ColorBufferGuard.unlock();
	UpdateBufferGuard.lock();
	for (unsigned int i = 0; i < UpdateBuffer.size(); i++)
	{
		UpdateRectangle(UpdateBuffer[i], Color::Green);
	}
	UpdateBuffer.clear();
	UpdateBufferGuard.unlock();
}

void MergeSort::StartSort()
{
	if (InProgress)
	{
		StopSort();
	}
	InProgress = true;
	SortingThread = new std::thread(&MergeSort::Partiton, this, 0, Size - 1);
}

void MergeSort::StopSort()
{
	if (InProgress)
	{
		InProgress = false;
		SortingThread->join();
		delete SortingThread;
		SortingThread = nullptr;
	}
}