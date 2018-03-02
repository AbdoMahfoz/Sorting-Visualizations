#include "Sorts.h"

//---------------------------Selection-Sort----------------------------
void SelectionSort::Sort()
{
	SetThreadDescription(GetCurrentThread(), L"Selection Sort Thread");
	std::unique_lock<std::mutex> BufferMutexGuard(ColorBufferMutex, std::defer_lock);
	std::unique_lock<std::mutex> UpdateBufferMutexGuard(UpdateBufferMutex, std::defer_lock);
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
				ColorBuffer.clear();
				BufferMutexGuard.unlock();
				return;
			}
			sleep(milliseconds(*ms));
			BufferMutexGuard.lock();
			ColorDescription cd;
			cd.GreenIndex[0] = i;
			cd.RedIndex[0] = j;
			cd.BlueIndex[0] = min;
			ColorBuffer.push_back(cd);
			BufferMutexGuard.unlock();
			if (Arr[j] < Arr[min])
			{
				min = j;
			}
		}
		UpdateBufferMutexGuard.lock();
		UpdateBuffer.push_back(i);
		UpdateBuffer.push_back(min);
		UpdateBufferMutexGuard.unlock();
		temp = Arr[min];
		Arr[min] = Arr[i];
		Arr[i] = temp;
	}
	InProgress = false;
	ColorBuffer.clear();
}

SelectionSort::SelectionSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr)
	: SortVisualizer(Size, Width, Height, xoffset, yoffset, ms, Arr)
{
}


//---------------------------Insertion-Sort----------------------------

void InsertionSort::Sort()
{
	InProgress = true;
	std::unique_lock < std::mutex > ColorBufferLock(ColorBufferMutex, std::defer_lock);
	std::unique_lock < std::mutex > UpdaetBufferLock(UpdateBufferMutex, std::defer_lock);
	for (int i = 0; i < Size; i++)
	{
		for (int j = i - 1; j >= 0; j--)
		{
			if (!InProgress)
			{
				return;
			}
			ColorBufferLock.lock();
			ColorDescription cd;
			cd.RedIndex[0] = j + 1;
			cd.GreenIndex[0] = i;
			ColorBuffer.push_back(cd);
			ColorBufferLock.unlock();
			sleep(milliseconds(*ms));
			if (Arr[j + 1] < Arr[j])
			{
				UpdaetBufferLock.lock();
				std::swap(Arr[j + 1], Arr[j]);
				UpdateBuffer.push_back(j);
				UpdateBuffer.push_back(j + 1);
				UpdaetBufferLock.unlock();
			}
			else
			{
				break;
			}
		}
	}
	InProgress = false;
}

InsertionSort::InsertionSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr)
	: SortVisualizer(Size, Width, Height, xoffset, yoffset, ms, Arr)
{
}

//---------------------------Bubble-Sort----------------------------

void BubbleSort::Sort()
{
	std::unique_lock < std::mutex > ColorBufferLock(ColorBufferMutex, std::defer_lock);
	std::unique_lock < std::mutex > UpdateBufferLock(UpdateBufferMutex, std::defer_lock);
	InProgress = true;
	bool Running = true;
	while (Running)
	{
		Running = false;
		for (int i = 0; i < Size - 1; i++)
		{
			if (!InProgress)
			{
				return;
			}
			sleep(milliseconds(*ms));
			if (Arr[i] > Arr[i + 1])
			{
				ColorBufferLock.lock();
				ColorDescription cd;
				cd.RedIndex[0] = i;
				cd.RedIndex[1] = i + 1;;
				ColorBuffer.push_back(cd);
				ColorBufferLock.unlock();
				Running = true;
				UpdateBufferLock.lock();
				UpdateBuffer.push_back(i);
				UpdateBuffer.push_back(i + 1);
				std::swap(Arr[i], Arr[i + 1]);
				UpdateBufferLock.unlock();
			}
			else
			{
				ColorBufferLock.lock();
				ColorDescription cd;
				cd.GreenIndex[0] = i;
				cd.GreenIndex[1] = i + 1;;
				ColorBuffer.push_back(cd);
				ColorBufferLock.unlock();
			}
		}
	}
	InProgress = false;
}

BubbleSort::BubbleSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr)
	: SortVisualizer(Size, Width, Height, xoffset, yoffset, ms, Arr)
{
}

//---------------------------Merge-Sort----------------------------

void MergeSort::Merge(int s, int f)
{
	int Count = f - s + 1, Mid = (s + f) / 2;
	int lp = s, rp = Mid + 1, ap = 0;
	int* aux = new int[Count];
	std::unique_lock < std::mutex > ColorBufferGuard(ColorBufferMutex, std::defer_lock);
	std::unique_lock < std::mutex > UpdateBufferGuard(UpdateBufferMutex, std::defer_lock);
	ColorBufferGuard.lock();
	ColorDescription cd;
	cd.BlueIndex[0] = s;
	cd.BlueIndex[1] = f;
	cd.RedIndex[0] = lp;
	cd.RedIndex[1] = rp;
	ColorBuffer.push_back(cd);
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
		ColorDescription cd;
		cd.BlueIndex[0] = s;
		cd.BlueIndex[1] = f;
		cd.RedIndex[0] = std::min(lp, Mid);
		cd.RedIndex[1] = std::min(rp, f - 1);
		ColorBuffer.push_back(cd);
		ColorBufferGuard.unlock();
	}
	while (lp <= Mid && InProgress)
	{
		sleep(milliseconds(*ms));
		aux[ap++] = Arr[lp++];
		ColorBufferGuard.lock();
		ColorDescription cd;
		cd.BlueIndex[0] = s;
		cd.BlueIndex[1] = f;
		cd.RedIndex[0] = std::min(lp, Mid);
		cd.RedIndex[1] = std::min(rp, f - 1);
		ColorBuffer.push_back(cd);
		ColorBufferGuard.unlock();
	}
	while (rp <= f && InProgress)
	{
		sleep(milliseconds(*ms));
		aux[ap++] = Arr[rp++];
		ColorBufferGuard.lock();
		ColorDescription cd;
		cd.BlueIndex[0] = s;
		cd.BlueIndex[1] = f;
		cd.RedIndex[0] = std::min(lp, Mid);
		cd.RedIndex[1] = std::min(rp, f - 1);
		ColorBuffer.push_back(cd);
		ColorBufferGuard.unlock();
	}
	for (int i = 0, j = s; j <= f && InProgress; j++, i++)
	{
		sleep(milliseconds(*ms));
		Arr[j] = aux[i];
		ColorBufferGuard.lock();
		UpdateBufferGuard.lock();
		ColorDescription cd;
		cd.BlueIndex[0] = s;
		cd.BlueIndex[1] = f;
		cd.GreenIndex[0] = j;
		ColorBuffer.push_back(cd);
		UpdateBuffer.push_back(j);
		ColorBufferGuard.unlock();
		UpdateBufferGuard.unlock();
	}
	delete[] aux;
}

void MergeSort::Partiton(int s, int f)
{
	if (s >= f || !InProgress)
	{
		return;
	}
	int mid = (s + f) / 2;
	Partiton(s, mid);
	if (!InProgress)
	{
		return;
	}
	Partiton(mid + 1, f);
	if (!InProgress)
	{
		return;
	}
	Merge(s, f);
}

void MergeSort::Sort()
{
	InProgress = true;
	Partiton(0, Size - 1);
	InProgress = false;
}

MergeSort::MergeSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr)
	: SortVisualizer(Size, Width, Height, xoffset, yoffset, ms, Arr)
{
}