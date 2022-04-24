#ifndef MERGE_SORT
#define MERGE_SORT
#include <thread>
#include <vector>
#include <iostream>

int* quick(int* & arrSegment, int start, int end)
{
	if(start<end)
	{
	int temp;
	int pivot = arrSegment[end];
	int i=start-1;
	for(int j=start; j<end; j++)
	{
		if(arrSegment[j]<pivot)
		{
			i++;
			temp = arrSegment[i];
			arrSegment[i] = arrSegment[j];
			arrSegment[j] = temp;
		}
	}
	temp = arrSegment[i+1];
	arrSegment[i+1] = arrSegment[end];
	arrSegment[end] = temp;
	int pi = i+1;
	quick(arrSegment, start, pi-1);
	quick(arrSegment, pi+1, end);
	}

	return arrSegment;
}

void merge(int* & A, int b, int m, int e)
{
	//Quit out of the merge if ends are the same
	if(b>=e) return;	

	//This for loop accomplishes L = A[b..m]
	const int LSize = m-b+1;
	int* L = new int[LSize];
	for(int i=0; i<LSize; i++)
	{
		L[i]=A[b+i];
	}

	//This for loop accomplishes R = A[m+1..e]
	const int RSize = e-m;
	int* R = new int[RSize];
	for(int i=0; i<RSize; i++)
	{
		R[i]=A[m+1+i];
	}
	int i=0; int j=0; int k=b;
	for(k=b; k<=e; k++)
	{
		if(i<LSize && j<RSize)
		{
			if(L[i]<=R[j])
			{
				A[k]=L[i];
				i++;
			}
			else
			{
				A[k]=R[j];
				j++;
			}
		}
		else
		{
			if(i<LSize)
			{
				A[k]=L[i];
				i++;
			}
			else
			{
				A[k]=R[j];
				j++;
			}
		}
	}

	delete[] L;
	delete[] R;
}

void merged_quick_sort(int *arr, int size, int threads)
{
	//Thread Size Allocation
	int* threadSizes = new int[threads];
	for(int i=0; i<threads; i++)
	{
		if(i<size%threads) 
		{
			threadSizes[i] = size/threads+1;
		}
		else threadSizes[i] = size/threads;
	}

	//Create Threads
	thread* pool = new thread[threads];
	int currentStart = 0;
	for(int i=0; i<threads; i++)
	{
		pool[i] = thread(quick, ref(arr), currentStart, currentStart+threadSizes[i]-1);
		currentStart+=threadSizes[i];
	}

	//Join Threads
	for(int i=0; i<threads; i++)
	{
		pool[i].join();
	}

	//Merge Array Sequentially
	int currentEnd=threadSizes[0];
	for(int i=0; i<threads-1; i++)
	{
		merge(arr, 0, currentEnd-1, currentEnd+threadSizes[i+1]-1);
		currentEnd+=threadSizes[i+1];
	}
	//Delete Extra Arrays
	delete[] threadSizes;
	delete[] pool;
}

#endif