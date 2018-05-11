#pragma once
#include "SortVisualizer.h"

class SelectionSort : public SortVisualizer
{
private:
	void Sort();
public:
	SelectionSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr);
};

class InsertionSort : public SortVisualizer
{
private:
	void Sort();
public:
	InsertionSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr);
};

class BubbleSort : public SortVisualizer
{
private:
	void Sort();
public:
	BubbleSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr);
};

class MergeSort : public SortVisualizer
{
private:
	void Merge(int s, int f);
	void Partiton(int s, int f);
	void Sort();
public:
	MergeSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr);
};

class QuickSort : public SortVisualizer
{
private:
	int Partition(int s, int f);
	void QSort(int s, int f);
	void Sort();
public:
	QuickSort(int Size, int Width, int Height, int xoffset, int yoffset, int* ms, int* Arr);
};