#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
template<class T>
class Heap
{
private:
	struct Data
	{
		T code;
		int count;
	};
	Data** Heap_Array;
	int last;

public:

	// Constructor
	Heap();
	//Hash(int);
	~Heap();

	//
	void reheapup(int index);
	bool insertheap(T dataIn);
	T getTop();
	void printHeap();//testing
};

template <class T>
Heap<T>::Heap()
{
	Heap_Array = new Data*[100];
	for (int i = 0; i < 100; i++)
	{
		Heap_Array[i] = new Data;
		Heap_Array[i]->count = 0;
		Heap_Array[i]->code = "";
	}

	last = 0;
}
template<class T>
Heap<T>::~Heap()
{
	delete[] Heap_Array;
}
template<class T>
void Heap<T>::reheapup(int index)
{
	int parent;

	if (index != 0)
	{
		parent = index - 1;
		if (Heap_Array[index]->count > Heap_Array[parent]->count)
		{
			Data *temp;
			temp = Heap_Array[index];
			Heap_Array[index] = Heap_Array[parent];
			Heap_Array[parent] = temp;
			reheapup(parent);
		}
	}

}

template <class T>
bool Heap<T>::insertheap(T dataIn)
{
	
	if (last == 100)
		return false;
	bool inserted = false;
	for (int i = 0; i < last; i++)
	{
		if (Heap_Array[i]->code == dataIn)
		{
			Heap_Array[i]->count++;
			inserted = true;
			reheapup(i);
		}
	}

	if (!inserted){
		Heap_Array[last]->code = dataIn;
		Heap_Array[last]->count = 1;
		last++;
		reheapup(last - 1);
	}


	return true;

}
template<class T>
T Heap<T>::getTop()
{
		return Heap_Array[0]->code;
}
template<class T>
void Heap<T>::printHeap()
{
	for (int i = 0; i < last ; i++)
		cout << Heap_Array[i]->code << "   " << Heap_Array[i]->count << endl;
}
#endif